// Copyright 2014 Sebastian A. Mueller
#ifndef PLENOSCOPE_CALIBRATION_WRITER_H_
#define PLENOSCOPE_CALIBRATION_WRITER_H_

#include <vector>
#include <string>
#include <fstream>
#include "plenoscope/Calibration/Calibrator.h"

namespace plenoscope {
namespace Calibration {

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

}  // namespace Calibration
}  // namespace plenoscope

#endif  // PLENOSCOPE_CALIBRATION_WRITER_H_
