#include "Tools.h"
#include <sstream>
using std::stringstream;

namespace Corsika {

    string get_run_header_print(const array<float, 273> &rh) {
        
        stringstream out;
        out << "------------------------------------------------------------\n";
        out << "  1 " << float2str(rh[1-1]) << "\n";
        out << "  2 " << "run number " << rh[2-1] << "\n";
        out << "  3 " << "date of begin run " << rh[3-1] << " yyyymmdd\n";
        out << "  4 " << "version of program " << rh[4-1] << " yyyymmdd\n";
        out << "------------------------------------------------------------\n";
        out << "  5 " << "number of observation levels " << rh[5-1] << "\n";
        return out.str();
    }

    string get_event_header_print(const array<float, 273> &eh) {
        
        stringstream out;
        out << "------------------------------------------------------------\n";
        out << "  1 " << float2str(eh[1-1]) << "\n";
        out << "  2 " << "event number " << eh[2-1] << "\n";
        out << "  3 " << "particle id " << eh[3-1] << "\n";
        out << "  4 " << "total energy " << eh[4-1] << "GeV\n";
        out << "  5 " << "starting altitude " << eh[5-1] << "g/cm^2\n";
        out << "  6 " << "number of first target if fixed " << eh[6-1] << "\n";
        out << "  7 " << "z coordinate (height) of first interaction " << eh[7-1] << "cm\n";
        out << "  8 " << "px momentum in x " << eh[8-1] << "GeV\n";
        out << "  9 " << "px momentum in y " << eh[9-1] << "GeV\n";
        out << " 10 " << "px momentum in z " << eh[10-1] << "GeV\n";
        out << " 11 " << "zenith angle Theta " << eh[11-1] << "rad\n";
        out << " 12 " << "azimuth angle Phi " << eh[12-1] << "rad\n";
        out << "------------------------------------------------------------\n";
        return out.str();
    }
    
    float str2float(const string word) {

        if(word.size() != 4) {
            stringstream info;
            info << "Corsika::" << __func__ << "()\n";
            info << "Expected word size: 4 chars, but actual: ";
            info << word.size() << " chars.\n";
            info << "word: '" << word << "'.\n";
            throw Not4Chars(info.str());          
        }

        char word_array[] = {word.at(0), word.at(1), word.at(2), word.at(3)};
        float f;
        memcpy(&f, &word_array, sizeof(f));
        return f;
    }

    string float2str(const float word_in_float) {

        char word_array[4];
        memcpy(&word_array, &word_in_float, sizeof(word_array));
        string word;
        for(uint i=0; i<4; i++)
            word += word_array[i];

        return word;
    }
}