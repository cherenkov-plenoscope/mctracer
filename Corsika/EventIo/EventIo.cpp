#include <sstream>
#include <cmath>
#include "EventIo.h"

namespace EventIo {
//------------------------------------------------------------------------------
Header::Header(istream& f, bool top_level) {
    size_t _start_pos = f.tellg();
    FirstFour first_four;
    if (top_level) {
        f.read((char*)&first_four, sizeof(FirstFour) );
        is_sync = check_if_sync(first_four.sync);
    }else { 
        // sub level headers do not have the 'sync' field.
        // sub level headers are therefore always considered, synced.
        f.read((char*)&(first_four.type), sizeof(FirstFour)-sizeof(int32_t));
        is_sync = true; 
    }
    
    if (!is_sync) {
        stringstream out;
        out << "In File/Function/Line:" << __FILE__;
        out << " / " << __func__ << " / " << __LINE__ << endl;
        out << "Header 'sync' field not correct: " << std::hex;
        out << first_four.sync << endl;
        out << "f.tell(): " << _start_pos << "\n";
        f.seekg(_start_pos, f.beg);
        throw NoSyncFoundException(out.str());
    }
    
    id = first_four.id;
    TypeInfo type_info(first_four.type);
    LengthInfo length_info(first_four.length);
    
    type = type_info.type;
    version = type_info.version;
    user = type_info.user;
    extended = type_info.extended;
    only_sub_objects = length_info.only_sub_objects;

    if (!type_info.extended) {
        length = length_info.length;
    }else {
        int32_t extended;
        f.read((char*)&extended, sizeof(int32_t));
        length = extend_length(extended, length_info);
    }        
}
//------------------------------------------------------------------------------
int64_t Header::extend_length(int32_t extended, const LengthInfo length_info) {
    int64_t ext, len;

    ext = extended;
    ext &= 0xfff;

    len = length_info.length;

    len &= (ext<<12);
    return len;
}
//------------------------------------------------------------------------------
bool Header::check_if_sync(int32_t _sync) {
    int32_t sync = -736130505;
    return (_sync == sync);
}
//------------------------------------------------------------------------------
Header::TypeInfo::TypeInfo(int32_t _type) {
    type = _type & 0xffff;
    version = (_type & 0xfff00000) >> 20;
    user = bool(_type & (1<<16));
    extended = bool(_type & (1<<17));
}
//------------------------------------------------------------------------------
Header::LengthInfo::LengthInfo(int32_t _length) {
    only_sub_objects = bool(_length & 1<<30);    
    // bit 31 of length is reserved
    length = _length &  0x3fffffff;
}
//------------------------------------------------------------------------------
string Header::str() {
    stringstream out;
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
//------------------------------------------------------------------------------
array<float, 273> make_corsika_273float_sub_block_form_stream(istream& f) {
    // read the first integer to get the size
    int32_t n;
    f.read((char*)&n, sizeof(n));
    if (n != 273) {
        stringstream info;
        info << __FILE__ << " " << __LINE__ <<"\n";
        info << __func__ << "()\n";
        info << "Expected first int32 to be 273 but actual it is " << n << "\n";
        throw TracerException(info.str());
    }
    // read the sub_block from file.
    array<float, 273> block;
    f.read((char*)block.data(), block.size()*sizeof(float));
    return block;
}
//------------------------------------------------------------------------------
array<float, 273> make_run_end_from_stream(istream& f) {
    // read the first integer to get the size
    int32_t n;
    f.read((char*)&n, sizeof(n));
    if (n != 3){
        stringstream out;
        out << "In File:" << __FILE__ << endl;
        out << "in function:" << __func__ << endl;
        out << "Line:" << __LINE__ << endl;
        out << "First integer was not 273 but n=" << n << endl;
        throw TracerException(out.str());
    }
    // read the sub_block from file.
    array<float, 273> block;
    f.read((char*)block.data(), block.size()*sizeof(float));
    return block;
}
//------------------------------------------------------------------------------
string make_input_card_from_stream(istream& f, const Header& head) {
    vector<char> input_card(head.length+1);
    f.read(input_card.data(), head.length);
    input_card[head.length] = 0;

    string input_card_text;

    for(size_t i=0; i<head.length; i++)
        input_card_text.push_back(input_card[i]);

    return input_card_text;
}
//------------------------------------------------------------------------------
vector<TelPos> make_telescope_positions(istream& f, const Header& head) {
    int32_t ntel;
    f.read((char*)&ntel, sizeof(ntel));

    int number_of_following_arrays = int((head.length - 4) / ntel /4);

    if (number_of_following_arrays != 4){
        stringstream out;
        out << "In File:" << __FILE__ << endl;
        out << "in function:" << __func__ << endl;
        out << "Line:" << __LINE__ << endl;
        out << " number_of_following_arrays is:" ;
        out <<  number_of_following_arrays << endl;
        throw TracerException(out.str());
    }
    vector<TelPos> telescope_positions(ntel);

    f.read(
        (char*)telescope_positions.data(), 
        telescope_positions.size()*sizeof(TelPos));

    return telescope_positions;
}
//------------------------------------------------------------------------------
string TelOffset::str() const{
    stringstream out;
    out << "TelOffset\n";
    out << "---------\n";
    out << "toff " << toff << endl;
    out << "xoff " << xoff << endl;
    out << "yoff " << yoff << endl;
    out << "weight " << weight << endl;
    return out.str();
}
//------------------------------------------------------------------------------
vector<TelOffset> make_telescope_offsets_from_stream(
    istream& f, 
    const Header& head
) {
    int length_first_two = 4 + 4;
    int32_t narray;
    f.read((char*)&narray, sizeof(narray));

    float toff;
    f.read((char*)&toff, sizeof(toff));

    vector<float> xoff(narray);
    vector<float> yoff(narray);
    vector<float> weight(narray, 1.0); // we initialize
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
            stringstream out;
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
        tel_off->yoff = yoff[i];
        tel_off->weight = weight[i];
    }            
    return telescope_offsets;
}
//------------------------------------------------------------------------------
vector<array<float, 8>> make_photons_from_stream(istream& f) {
    Header subhead(f, false);
    if (subhead.type != 1205) {
        int header_length = subhead.extended ? 4 : 3;
        f.seekg(header_length*-4, f.cur);
        //TODO: put useful text.
        stringstream info;
        info << __FILE__ << " " << __LINE__ <<"\n";
        throw WrongTypeException(info.str());
    }

    BunchHeader b_head;
    f.read((char*)&b_head, sizeof(BunchHeader));

    const bool is_compact = bool(subhead.version/1000 == 1);
    const int element_size = is_compact? 2 : 4;

    vector<char> buff(b_head.n_bunches*8*element_size);
    f.read(buff.data(), b_head.n_bunches*8*element_size);

    vector<array<float, 8>> bunches(b_head.n_bunches);

    if(is_compact)
        for(size_t row=0; row<bunches.size(); row++)
            for(size_t field=0; field<8; field++)
                bunches.at(row).at(field) = 
                    float(((int16_t*)buff.data())[row*8 + field]);             
    else
        for(size_t row=0; row<bunches.size(); row++)
            for(size_t field=0; field<8; field++)
                bunches.at(row).at(field) = 
                    float(((float*)buff.data())[row*8 + field]); 

    if(is_compact)
        for(size_t row=0; row<bunches.size(); row++) {
            bunches[row][0] *= 0.1;     
            bunches[row][1] *= 0.1;     
            bunches[row][2] /= 30000.;  
            bunches[row][3] /= 30000.;  
            bunches[row][4] *= 0.1;
            bunches[row][5] = pow(10, bunches[row][5] * 0.001);  
            bunches[row][6] *= 0.01;    
            //bunches[row][7] *= 1.;
        }

    return bunches;    
}
//------------------------------------------------------------------------------
bool Run::has_still_events_left()const {
    return !this->run_end_found;
}
//------------------------------------------------------------------------------
Run::Run(string path):
    run_end_found(false)
{
    this->path = path;
    f.open(path);
    if(!f.is_open()) {
        stringstream info;
        info << __FILE__ " " << __LINE__ << "\n";
        info << "Can not open file: " << path << "\n";
        throw TracerException(info.str());
    }

    this->__read_run_header();

    this->_current_photon_data = this->_next();
}
//------------------------------------------------------------------------------
void Run::__read_run_header() {
    this->__get_header(1200);
    this->header.raw = make_corsika_273float_sub_block_form_stream(this->f);

    auto header_2 = this->__get_header(1212);
    this->header.input_card = make_input_card_from_stream(this->f, header_2);

    auto header_3 = this->__get_header(1201);
    this->header.tel_pos = make_telescope_positions(this->f, header_3);
}
//------------------------------------------------------------------------------
void Run::__read_event_header() {
    this->__get_header(1202);
    this->current_event_header.raw = make_corsika_273float_sub_block_form_stream(this->f);

    auto header_2 = this->__get_header(1203);
    this->current_event_header.telescope_offsets = make_telescope_offsets_from_stream(this->f, header_2);
}
//------------------------------------------------------------------------------
void Run::__read_event_end() {
    this->__get_header(1209);
    this->current_event_end = make_corsika_273float_sub_block_form_stream(this->f);
}
//------------------------------------------------------------------------------
void Run::__read_run_end() {
    this->__get_header(1210);
    this->end = make_run_end_from_stream(this->f);
}
//------------------------------------------------------------------------------
Header Run::__get_header(int expect_type) {   
    Header header(this->f);
    if(header.type != expect_type) {
        int header_length = header.extended ? 5 : 4;
        this->f.seekg(header_length*-4, this->f.cur);
        //TODO: put useful text.
        stringstream info;
        info << __FILE__ << " " << __LINE__ <<"\n";
        info << __func__ << "()\n";
        info << "while reading file: " << path << "\n";
        info << "Expected header type: " << expect_type;
        info << " but actual it is " << header.type << "\n";
        throw WrongTypeException(info.str());
    }
    return header;
}
//------------------------------------------------------------------------------
Event Run::next_event() {
    Event event;

    event.header = this->current_event_header;
    event.photons = this->_current_photon_data;
    
    this->_current_photon_data = this->_next();

    return event;
}
//------------------------------------------------------------------------------
vector<array<float, 8>> Run::_next() {
    bool something_found = false;
    while(!this->run_end_found) {
        something_found = false;
        try
        {
            return make_photons_from_stream(this->f);
            something_found = true;
        }
        catch (WrongTypeException& e) { /*nothing to do*/ }

        try
        {
            this->__get_header(1204);
            something_found = true;
        }
        catch (WrongTypeException& e) { /*nothing to do*/ }
        catch (NoSyncFoundException& e) { /*nothing to do*/ }


        try
        {
            this->__read_event_header();
            something_found = true;
        }
        catch (WrongTypeException& e) { /*nothing to do*/ }
        catch (NoSyncFoundException& e) { /*nothing to do*/ }

        try
        {
            this->__read_event_end();
            something_found = true;            
        }
        catch (WrongTypeException& e) { /*nothing to do*/ }
        catch (NoSyncFoundException& e) { /*nothing to do*/ }

        try
        {
            this->__read_run_end();
            something_found = true;
            this->run_end_found = true;
        }
        catch (WrongTypeException& e) { /*nothing to do*/ }
        catch (NoSyncFoundException& e) { /*nothing to do*/ }

        if(!something_found) {
            throw TracerException("Not a single valid structure found in file.");
        }
    }

    vector<array<float, 8>> dummy;
    return dummy;
}
//------------------------------------------------------------------------------
} //namespace EventIo