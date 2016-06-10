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
        out << " 13 " << "number of different random number sequences " << eh[13-1] << "\n";
        out << " 11+3i " << "\n";
        out << " 12+3i " << "\n";
        out << " 13+3i " << "\n";
        out << "------------------------------------------------------------\n";
        out << " 44 " << "run number " << eh[44-1] << "\n";
        out << " 45 " << "date of begin run (yymmdd) " << eh[45-1] << "\n";
        out << " 46 " << "version of program " << eh[46-1] << "\n";
        out << "------------------------------------------------------------\n";
        out << " 47 " << "number of observation levels " << eh[47-1] << "\n";
        for(uint t=1; t<= eh[47-1]; t++) {
        out << " " << 47+t << " " << "height of level " << t << eh[47+t-1] << "cm\n";
        }
        out << "------------------------------------------------------------\n";
        out << " 58 " << "slope of energy spectrum " << eh[58-1] << "\n";
        out << " 59 " << "lower limit of energy range " << eh[59-1] << "GeV\n";
        out << " 60 " << "upper limit of energy range " << eh[60-1] << "GeV\n";
        out << " 61 " << "cutoff for hadrons kinetic energy " << eh[61-1] << "GeV\n";
        out << " 62 " << "cutoff for muons kinetic energy " << eh[62-1] << "GeV\n";
        out << " 63 " << "cutoff for electrons kinetic energy " << eh[63-1] << "GeV\n";
        out << " 64 " << "cutoff for photons energy " << eh[64-1] << "GeV\n";
        out << " 65 " << "NFLAIN " << eh[65-1] << "\n";
        out << " 66 " << "NFLDIF " << eh[66-1] << "\n";
        out << " 67 " << "NFLPI0 " << eh[67-1] << "\n";
        out << " 68 " << "NFLPIF " << eh[68-1] << "\n";
        out << " 69 " << "NFLCHE " << eh[69-1] << "\n";
        out << " 70 " << "NFRAGM " << eh[70-1] << "\n";
        out << " 71 " << "x component of Earth’s magnetic field " << eh[71-1] << "uT\n";
        out << " 72 " << "z component of Earth’s magnetic field " << eh[72-1] << "uT\n";
        out << " 73 " << "flag for activating EGS4 " << eh[73-1] << "\n";
        out << " 74 " << "flag for activating NKG " << eh[74-1] << "\n";
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