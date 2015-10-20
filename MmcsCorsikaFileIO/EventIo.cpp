#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

#include "MmcsCorsikaRunHeader.h"
#include "MmcsCorsikaEventHeader.h"
#include "EventIo.h"
using std::cout;
using std::endl;
using std::string;
using std::vector;

namespace EventIo{

Header::Header(std::istream& f, bool top_level) {
        FirstFour first_four;
        if (top_level) {
            f.read((char*)&first_four, sizeof(FirstFour) );
            is_sync = check_if_sync(first_four.sync);
        }
        else{ 
            // sub level headers do not have the 'sync' field.
            // sub level headers are therefore always considered, synced.
            f.read(
                (char*)&(first_four.type), 
                sizeof(FirstFour)-sizeof(int32_t) 
                );
            is_sync = true; 
        }
        
        if (!is_sync) {
            std::stringstream out;
            out << "In File/Function/Line:" << __FILE__;
            out << "/" << __func__ << "/" << __LINE__ << endl;
            out << "Header 'sync' field not correct: " << std::hex;
            out << first_four.sync << endl;
            throw TracerException(out.str());
        }
        
        id = first_four.id;
        TypeInfo type_info(first_four.type);
        LengthInfo length_info(first_four.length);
        
        type = type_info.type;
        version = type_info.version;
        user = type_info.user;
        extended = type_info.extended;
        only_sub_objects = length_info.only_sub_objects;

        if (!type_info.extended)
        {
            length = length_info.length;
        }
        else
        {
            int32_t extended;
            f.read((char*)&extended, sizeof(int32_t));
            length = extend_length(extended, length_info);
        }
        
    }

int64_t Header::extend_length(int32_t extended, const LengthInfo length_info){
    int64_t ext, len;

    ext = extended;
    ext &= 0xfff;

    len = length_info.length;

    len &= (ext<<12);
    return len;
}


bool Header::check_if_sync(int32_t _sync){
    int32_t sync = -736130505;
    return (_sync == sync);
}


Header::TypeInfo::TypeInfo(int32_t _type)
{
    type = _type & 0xffff;
    version = (_type & 0xfff00000) >> 20;
    user = bool(_type & (1<<16));
    extended = bool(_type & (1<<17));
}

Header::LengthInfo::LengthInfo(int32_t _length)
{
    only_sub_objects = bool(_length & 1<<30);    
    // bit 31 of length is reserved
    length = _length &  0x3fffffff;
}


std::string Header::get_print() {
    std::stringstream out;
    out << "HEADER" << endl;
    out << "------" << endl;
    out << "bool is_sync " << is_sync << endl;
    out << "int32_t type " << type << endl;
    out << "int32_t version " << version << endl;
    out << "bool user " << user << endl;
    out << "bool extended " << extended << endl;
    out << "bool only_sub_objects " << only_sub_objects << endl;
    out << "int64_t length " << length << endl;
    out << "int32_t id " << id << endl;
    return out.str();
}


MmcsCorsikaRunHeader make_run_header_from_stream(
    std::istream& f) 
{
    // read the first integer to get the size
    int32_t n;
    f.read((char*)&n, sizeof(n));
    if (n != 273){
        std::stringstream out;
        out << "In File:" << __FILE__ << endl;
        out << "in function:" << __func__ << endl;
        out << "Line:" << __LINE__ << endl;
        out << "First integer was not 273 but n=" << n << endl;
        throw TracerException(out.str());
    }
    
    // read the sub_block from file.
    std::vector<float> block(273);

    f.read((char*)block.data(), block.size()*sizeof(float));
    
    MmcsCorsikaRunHeader run_header(block);

    return run_header;
}

MmcsCorsikaEventHeader make_event_header_form_stream(
    std::istream& f, 
    const Header& head) 
{
    // read the first integer to get the size
    int32_t n;
    f.read((char*)&n, sizeof(n));
    if (n != 273){
        std::stringstream out;
        out << "In File:" << __FILE__ << endl;
        out << "in function:" << __func__ << endl;
        out << "Line:" << __LINE__ << endl;
        out << "First integer was not 273 but n=" << n << endl;
        throw TracerException(out.str());
    }

    // read the sub_block from file.
    std::vector<float> block(273);
    f.read((char*)block.data(), block.size()*sizeof(float));
    
    MmcsCorsikaEventHeader run_header(block);

    return run_header;
}

std::string make_input_card_from_stream(
    std::istream& f, 
    const Header& head)
{
    char * input_card = new char[head.length+1];
    cout << "head.length:" << head.length << "\n";
    f.read(input_card, head.length);
    input_card[head.length] = 0;

    std::string foo;

    for(size_t i=0; i<head.length; i++){
        foo.push_back(input_card[i]);
    }

    delete[] input_card;

    return foo;
}


std::vector<TelPos> make_telescope_positions(
    std::istream& f, 
    const Header& head)
{
    int32_t ntel;
    f.read((char*)&ntel, sizeof(ntel));

    int number_of_following_arrays = int((head.length - 4) / ntel /4);

    if (number_of_following_arrays != 4){
        std::stringstream out;
        out << "In File:" << __FILE__ << endl;
        out << "in function:" << __func__ << endl;
        out << "Line:" << __LINE__ << endl;
        out << " number_of_following_arrays is:" ;
        out <<  number_of_following_arrays << endl;
        throw TracerException(out.str());
    }
    std::vector<TelPos> telescope_positions(ntel);

    f.read(
        (char*)telescope_positions.data(), 
        telescope_positions.size()*sizeof(TelPos));

    return telescope_positions;
}

std::vector<TelOffset> make_telescope_offsets_from_stream(
    std::istream& f, 
    const Header& head)
{
    int length_first_two = 4 + 4;
    int32_t narray;
    f.read((char*)&narray, sizeof(narray));

    float toff;
    f.read((char*)&toff, sizeof(toff));

    vector<float> xoff(narray);
    vector<float> yoff(narray);
    vector<float> weight(narray);
    vector<TelOffset> telescope_offsets(narray);

    f.read((char*)xoff.data(), xoff.size()*sizeof(float));
    f.read((char*)yoff.data(), yoff.size()*sizeof(float));

    int number_of_following_arrays = int((head.length - length_first_two) / narray /4);
    switch (number_of_following_arrays)
    {
        case 2:
            // do nothing, we already read the 2 arrays xoff and yoff.
            break;
        case 3:
            f.read((char*)weight.data(), weight.size()*sizeof(float));
            break;
        default:
            std::stringstream out;
            out << "In File:" << __FILE__ << endl;
            out << "in function:" << __func__ << endl;
            out << "Line:" << __LINE__ << endl;
            out << "number_of_following_arrays is " << number_of_following_arrays << endl;
            out << "but only 2 or 3 are allowed." << endl;
            throw TracerException(out.str());
    }
    
    int i=0;
    for(auto tel_off=telescope_offsets.begin(); tel_off != telescope_offsets.end(); tel_off++, i++)
    {
        tel_off->toff = toff;
        tel_off->xoff = xoff[i];
        tel_off->yoff = xoff[i];
        tel_off->weight = weight[i];
    }            
    return telescope_offsets;
}


vector<vector<float> > make_photons_from_stream(
    std::istream& f, 
    const Header& head)
{
    if(!head.only_sub_objects)
    {
        std::stringstream out;
        out << "In File:" << __FILE__ << endl;
        out << "in function:" << __func__ << endl;
        out << "Line:" << __LINE__ << endl;
        out << "Type 1204 ususally has only sub objects, this one has not!!" << endl;
        throw TracerException(out.str());
    }
    Header subhead(f, false);
    BunchHeader b_head;
    f.read((char*)&b_head, sizeof(BunchHeader));

    const int fields_per_photon = 8;
    const bool is_compact = bool(subhead.version/1000 == 1);
    const int element_size = is_compact? 2 : 4;

    char *buf = new char[b_head.n_bunches * fields_per_photon * element_size];
    f.read(buf, b_head.n_bunches* fields_per_photon * element_size);

    vector<vector<float> > bunches;
    bunches.resize(b_head.n_bunches);
    int row_id = 0;

    if(is_compact)
    {
        for(auto row=bunches.begin(); row!=bunches.end(); row++, row_id++)
        {
            row->resize(fields_per_photon);
            for(size_t i=0; i<fields_per_photon; i++)
                (*row)[i] = float(((int16_t*)buf)[row_id*8 + i]);
        }                
    }
    else
    {
        for(auto row=bunches.begin(); row!=bunches.end(); row++, row_id++)
        {
            row->resize(8);
            for(size_t i=0; i<fields_per_photon; i++)
                (*row)[i] = float(((float*)buf)[row_id*8 + i]);
        }   
    }

    if (is_compact)
    {
        for(auto row=bunches.begin(); row!=bunches.end(); row++, row_id++)
        {
            row->resize(8);
            (*row)[0] *= 0.1;     
            (*row)[1] *= 0.1;     
            (*row)[2] /= 30000.;  
            (*row)[3] /= 30000.;  
            (*row)[4] *= 0.1;
            (*row)[5] = pow(10, (*row)[5]) * 0.001;  
            (*row)[6] *= 0.01;    
            //(*row)[7] *= 1.;
        }
    }

    delete[] buf;
    
    return bunches;    
}
        

} //namespace EventIo