// Copyright 2014 Sebastian A. Mueller
#include "merlict_portal_plenoscope/calibration/LixelStatistics.h"
#include <sstream>
#include <fstream>


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

void write(
    const std::vector<LixelStatistic> &lixel_statistics,
    const std::string &path
) {
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

std::vector<LixelStatistic> read(const std::string &path) {
    std::ifstream file;
    file.open(path, std::ios::binary);
    if (!file.is_open()) {
        std::stringstream info;
        info << __FILE__ << " " << __LINE__ << "\n";
        info << "LixelStatistics: Unable to read file: '" << path << "'\n";
        throw std::runtime_error(info.str());
    }

    std::vector<LixelStatistic> lixel_statistics;
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

std::vector<float> read_efficiencies(const std::string &path) {
    std::ifstream file;
    file.open(path, std::ios::binary);
    if (!file.is_open()) {
        std::stringstream info;
        info << __FILE__ << " " << __LINE__ << "\n";
        info << "LixelStatistics: Unable to read file: '" << path << "'\n";
        throw std::runtime_error(info.str());
    }

    std::vector<float> efficiencies;

    while (true) {
        LixelStatistic lixel;
        file.read((char*)&lixel, sizeof(LixelStatistic));
        if (!file.eof())
            efficiencies.push_back(lixel.efficiency);
        else
            break;
    }
    file.close();

    return efficiencies;
}

}  // namespace calibration
}  // namespace plenoscope
