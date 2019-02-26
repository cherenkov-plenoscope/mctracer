// Copyright 2014 Sebastian A. Mueller
#include "plenoscope/calibration/LixelStatistics.h"
#include <sstream>
#include <fstream>
using std::vector;
using std::string;

namespace plenoscope {
namespace calibration {

LixelStatistic::LixelStatistic():
    efficiency(0.0), efficiency_std(0.0),
    cx_mean(0.0), cx_std(0.0),
    cy_mean(0.0), cy_std(0.0),
    x_mean(0.0), x_std(0.0),
    y_mean(0.0), y_std(0.0),
    time_delay_mean(0.0), time_delay_std(0.0)
{}

void write(const vector<LixelStatistic> &lixel_statistics, const string &path) {
    std::ofstream file;
    file.open(path, std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        std::stringstream info;
        info << __FILE__ << " " << __LINE__ << "\n";
        info << "LixelStatistics: Unable to write file: '" << path << "'\n";
        throw std::runtime_error(info.str());
    }

    for (const LixelStatistic &lixel : lixel_statistics)
        file.write((char*)&lixel, sizeof(LixelStatistic));
    file.close();
}

vector<LixelStatistic> read(const string &path) {
    std::ifstream file;
    file.open(path, std::ios::binary);
    if (!file.is_open()) {
        std::stringstream info;
        info << __FILE__ << " " << __LINE__ << "\n";
        info << "LixelStatistics: Unable to read file: '" << path << "'\n";
        throw std::runtime_error(info.str());
    }

    vector<LixelStatistic> lixel_statistics;
    while (true) {
        LixelStatistic lixel;
        file.read((char*)&lixel, sizeof(LixelStatistic));
        if (!file.eof())
            lixel_statistics.push_back(lixel);
        else
            break;
    }
    file.close();
    return lixel_statistics;
}

}  // namespace calibration
}  // namespace plenoscope
