// Copyright 2014 Sebastian A. Mueller
#ifndef PLENOSCOPE_CALIBRATION_WRITER_H_
#define PLENOSCOPE_CALIBRATION_WRITER_H_

#include <vector>
#include <string>
#include <fstream>
#include "merlict_portal_plenoscope/calibration/Calibrator.h"

namespace plenoscope {
namespace calibration {

class Writer {
    const std::string path;
    std::ofstream file;

 public:
    explicit Writer(const std::string path);
    void append(const std::vector<CalibrationPhotonResult> &table);

 private:
    void clear_file();
    void write_row(const CalibrationPhotonResult &row);
    void assert_file_is_open();
};

}  // namespace calibration
}  // namespace plenoscope

#endif  // PLENOSCOPE_CALIBRATION_WRITER_H_
