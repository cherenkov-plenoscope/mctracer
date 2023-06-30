// Copyright 2016 Sebastian A. Mueller, Dominik Neise
#ifndef EVENTIO_H_
#define EVENTIO_H_

#include <cstdint>
#include <string>
#include <vector>
#include <array>
#include <fstream>

namespace eventio {

class WrongTypeException :public std::runtime_error {
    using runtime_error::runtime_error;
};

class NoSyncFoundException :public std::runtime_error {
    using runtime_error::runtime_error;
};

struct TelPos {
    float x, y, z, r;
    std::string str() const;
};

struct TelOffset {
    float toff;
    float xoff;
    float yoff;
    float weight;
    std::string str() const;
};

struct EventIoRunHeader {
    std::array<float, 273> raw;
    std::string input_card;
    std::vector<TelPos> tel_pos;
};

struct EventIoEventHeader {
    std::array<float, 273> raw;
    std::vector<TelOffset> telescope_offsets;
};

struct Event {
    EventIoEventHeader header;
    std::vector<std::array<float, 8>> photons;
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
    explicit Header(std::istream& f, bool top_level = true);
    std::string str();

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
        explicit TypeInfo(int32_t _type);
    };

    struct LengthInfo {
        bool only_sub_objects;
        int32_t length;
        explicit LengthInfo(int32_t _length);
    };

    bool check_if_sync(int32_t _sync);
    int64_t extend_length(int32_t extended, const LengthInfo length_info);
};

struct BunchHeader {
    int16_t array;
    int16_t tel;
    float photons;
    int32_t n_bunches;
    std::string str() const;
};

std::array<float, 273> make_corsika_273float_sub_block_form_stream(
    std::istream& f);
std::string make_input_card_from_stream(std::istream& f, const Header& head);
std::vector<TelPos> make_telescope_positions(
    std::istream& f,
    const Header& head);
std::vector<TelOffset> make_telescope_offsets_from_stream(
    std::istream& f,
    const Header& head);

//  returns N photon bunches as 2D float Matrix: with shape: (N x 8)
//
//  the 8 floats per 'row' stand for:
//  * x in cm
//  * y in cm
//  * cx
//  * cy
//  * time in nanoseconds since first interaction.
//  * zem
//  * photons
//  * wavelength is in nanometer negative if scattered ?! (whatever this means)
//
std::vector<std::array<float, 8>> make_photons_from_stream(
    std::istream& f,
    const Header& head);

class Run {
    std::ifstream f;
    std::string path;
    bool run_end_found;
    std::vector<std::array<float, 8>> _current_photon_data;

    Header __get_header(int expect_type);
    void __read_run_header();
    void __read_event_header();
    void __read_event_end();
    void __read_run_end();
    std::vector<std::array<float, 8>> _next();
    std::array<float, 273> current_event_end;
    EventIoEventHeader current_event_header;

 public:
    bool has_still_events_left()const;
    EventIoRunHeader header;
    std::array<float, 273> end;
    explicit Run(const std::string path);
    Event next_event();
};

void write_photon_bunches_to_path(
    const std::vector<std::array<float, 8>> &photons,
    const std::string &path
);

}  // namespace eventio

#endif  // EVENTIO_H_
