#include "PhotonsReader.h"
#include "PhotonsReaderWrapper/EventIoWrapper.h"
#include "PhotonsReaderWrapper/MmcsCorsikaIoWrapper.h"
#include "PhotonsReaderWrapper/AsciiIoWrapper.h"
#include "PhotonsReaderWrapper/InternalPhotonSourceWrapper.h"
using std::cout;
//------------------------------------------------------------------------------
PhotonsReader::PhotonsReader(const string path) {
    
    std::stringstream out;

    try{

        out << "___EventIo___\n";
        photons_from_file = new EventIoWrapper(path);
    }catch(std::exception &error) {
        out << error.what();
        try{

            out << "___MmcsCorsikaIo___\n";
            photons_from_file = new MmcsCorsikaIoWrapper(path);
        }catch(std::exception &error) {
            out << error.what();
            try{
                
                out << "___AsciiIo___\n";
                photons_from_file = new AsciiIoWrapper(path);
            }catch(std::exception &error) {
                out << error.what();
                try{
                
                    out << "___InternalLightSource___\n";
                    photons_from_file = new InternalPhotonSourceWrapper(path);
                }catch(std::exception &error) {

                    out << error.what();
                    out << 
                    "Can not read photons from file, it is neither "
                    "Corsika EventIo, Mmcs Corsika, nor Ascci text\n";

                    throw TracerException(out.str());   
                }   
            }
        }   
    }
}
//------------------------------------------------------------------------------
PhotonsReader::~PhotonsReader() {
    delete photons_from_file;
}
//------------------------------------------------------------------------------
bool PhotonsReader::has_still_photons_left()const {
    return photons_from_file->has_still_photons_left();
}
//------------------------------------------------------------------------------
vector<Photon> PhotonsReader::next(Random::Generator* prng) {
    return photons_from_file->next(prng);
}