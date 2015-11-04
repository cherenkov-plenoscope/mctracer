#include <sstream>
#include "PhotonMcTruth.h"
//------------------------------------------------------------------------------
const PhotonMcTruth PhotonMcTruth::void_truth = PhotonMcTruth();
//------------------------------------------------------------------------------
PhotonMcTruth::PhotonMcTruth() {
	production_height_over_sea_level = 0. ;
}
//------------------------------------------------------------------------------
std::string PhotonMcTruth::get_print() const{
	std::stringstream out; 
	out << "production height over sea level: " ;
	out << production_height_over_sea_level ;
	out << "\n";
	return out.str();
}
//------------------------------------------------------------------------------
double PhotonMcTruth::get_production_height_over_sea_level() const{
	return production_height_over_sea_level;
}