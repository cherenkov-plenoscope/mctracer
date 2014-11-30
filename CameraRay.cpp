#include "CameraRay.h"
CameraRay::CameraRay(const Vector3D support, const Vector3D direction){
	SetRay(support, direction);
}
//======================================================================
std::string CameraRay::get_string()const{
	std::stringstream out; 
	out << Ray::get_string() << ", colour: " << colour;
	return out.str();
}
//======================================================================
void CameraRay::disp()const{
	std::cout << "CameraRay -> " << get_string() << "\n";
}
//======================================================================
CsvRow CameraRay::getCameraRayCsvRow(GlobalSettings& settings)const{

	CsvRow row;

	stringstream out;
	out.precision(settings.get_decimal_precision_for_csv_output());

	out << colour.blue();
	row.push_back( out.str() );
	out.str("");

	out << colour.green();
	row.push_back( out.str() );
	out.str("");

	out << colour.red();
	row.push_back( out.str() );
	out.str("");

	return row;
}
//======================================================================
CsvRow CameraRay::getCsvRow(GlobalSettings& settings)const{
	
	CsvRow combinedRow;

	if(settings.ShowCsvIdentifier())
	combinedRow.push_back("Cam");

	CsvRow RowOfRay 	= getRayCsvRow(settings);
	CsvRow RowOfPhoton 	= getCameraRayCsvRow(settings);
	
	combinedRow.append(RowOfRay);
	combinedRow.append(RowOfPhoton);

	return combinedRow;
}
//======================================================================
// friends of osstream
//======================================================================
std::ostream& operator<<(std::ostream& os, 
const CameraRay& camera_ray_to_be_displayed){
    os << camera_ray_to_be_displayed.get_string();
    return os;
}
