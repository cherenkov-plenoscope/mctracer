//=================================
// include guard
#ifndef __CorsikaEventIo_H_INCLUDED__
#define __CorsikaEventIo_H_INCLUDED__

#include <vector>
#include <array>
#include <iostream>
#include <fstream>

#include "Corsika/MmcsCorsikaRunHeader.h"
#include "Corsika/MmcsCorsikaEventHeader.h"

using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::vector;
using std::array;
using std::istream;
using std::ifstream;

namespace EventIo {

    class WrongTypeException :public TracerException {
        using TracerException::TracerException;
    };

    class NoSyncFoundException :public TracerException {
        using TracerException::TracerException;
    };
    
    struct TelPos {
        float x,y,z,r;
        string get_print() const;
    };

    struct TelOffset {
        float toff;
        float xoff;
        float yoff;
        float weight;
        string get_print() const;
    };

    struct EventIoRunHeader {
        MmcsCorsikaRunHeader mmcs_runheader;
        string input_card;
        vector<TelPos> tel_pos;
    };

    struct EventIoEventHeader {
        MmcsCorsikaEventHeader mmcs_event_header;
        vector<TelOffset> telescope_offsets;
    };

    struct Event {
        EventIoEventHeader header;
        vector<array<float, 8>> photons;
    };

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
        Header(istream& f, bool top_level=true);
        string get_print();
    private:

        struct FirstFour {
            int32_t sync;
            int32_t type;
            int32_t id;
            int32_t length;
        };

        struct TypeInfo {
            int32_t type;
            int32_t version;
            bool user;
            bool extended;
            TypeInfo(int32_t _type);
        };

        struct LengthInfo {
            bool only_sub_objects;
            int32_t length;
            LengthInfo(int32_t _length);
        };

        bool check_if_sync(int32_t _sync);
        int64_t extend_length(int32_t extended, const LengthInfo length_info);
    };


    struct BunchHeader {
        int16_t array;
        int16_t tel;
        float photons;
        int32_t n_bunches;
        string get_print() const;
    };


    MmcsCorsikaRunHeader make_run_header_from_stream(istream& f);
    MmcsCorsikaEventHeader make_event_header_form_stream(istream& f);
    string make_input_card_from_stream(istream& f, const Header& head);
    vector<TelPos> make_telescope_positions(istream& f, const Header& head);
    vector<TelOffset> make_telescope_offsets_from_stream(istream& f, const Header& head);

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
    vector<array<float, 8>> make_photons_from_stream(istream& f, const Header& head);

    class EventIoFile {

        ifstream f;
        string path;
        bool run_end_found;
        vector<array<float, 8>> _current_photon_data;

        Header __get_header(int expect_type);
    
        void __read_run_reader();
        void __read_event_header();
        void __read_event_end();
        void __read_run_end();
        vector<array<float, 8>> _next();
    public:

        bool has_still_events_left()const;
        EventIoRunHeader run_header;
        vector<float> current_event_end;
        EventIoEventHeader current_event_header;
        vector<float> run_end;

        EventIoFile(const string path);
        //EventIoFile(string path, istream& stream);
        //void read_all_headers();

        Event next_event();
    };
} //namespace EventIo

#endif // __CorsikaEventIo_H_INCLUDED__ 