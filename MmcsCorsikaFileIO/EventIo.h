#include <vector>
#include <iostream>
#include <fstream>


#include "MmcsCorsikaFileIO/MmcsCorsikaRunHeader.h"
#include "MmcsCorsikaFileIO/MmcsCorsikaEventHeader.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;

namespace EventIo{

    class Header{

    public:
        bool is_sync;
        int32_t type;
        int32_t version;
        bool user;
        bool extended;
        bool only_sub_objects;
        uint64_t length;
        int32_t id;
        Header(std::istream& f, bool top_level=true);
        std::string get_print();
        
    private:
        struct FirstFour {
            int32_t sync;
            int32_t type;
            int32_t id;
            int32_t length;
        };

        struct TypeInfo{
            int32_t type;
            int32_t version;
            bool user;
            bool extended;
            TypeInfo(int32_t _type);
        };

        struct LengthInfo{
            bool only_sub_objects;
            int32_t length;
            LengthInfo(int32_t _length);
        };

        bool check_if_sync(int32_t _sync);
        int64_t extend_length(int32_t extended, const LengthInfo length_info);
    };

    struct TelPos{
        float x,y,z,r;
        std::string get_print() const;
    };

    struct TelOffset{
        float toff;
        float xoff;
        float yoff;
        float weight;
        std::string get_print() const;
    };

    struct BunchHeader{
        int16_t array;
        int16_t tel;
        float photons;
        int32_t n_bunches;
        std::string get_print() const;
    };


    MmcsCorsikaRunHeader make_run_header_from_stream(std::istream& f); 

    MmcsCorsikaEventHeader make_event_header_form_stream(
        std::istream& f, 
        const Header& head);

    std::string make_input_card_from_stream(
        std::istream& f, 
        const Header& head);

    std::vector<TelPos> make_telescope_positions(
        std::istream& f, 
        const Header& head);

    std::vector<TelOffset> make_telescope_offsets_from_stream(
        std::istream& f, 
        const Header& head);

    /* returns photon bunches as 2D float Matrix: with shape: (N x 8)
     * 
     *  the 8 floats per 'row' stand for:
     *  * x in cm
     *  * y in cm
     *  * cx
     *  * cy
     *  * time in nanoseconds since first interaction.
     *  * zem
     *  * photons
     *  * wavelength is in nanometer negative if scattered ?! (whatever this means)
     */
    vector<vector<float> > make_photons_from_stream(
        std::istream& f, 
        const Header& head);
            

} //namespace EventIo