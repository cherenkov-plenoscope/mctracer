#include "Func1D.h"
//------------------------------------------------------------------------------
Func1D::Func1D(std::vector<std::vector<double>> two_column_xy) {

	func.resize(two_column_xy.size());

	for(uint i=0; i<two_column_xy.size(); i++) {

		if(two_column_xy.at(i).size() != 2) {
			std::stringstream info;
			info << exception_header();
			info << "Expected vec of vec matrix of shape N x 2, but actual:\n";
			info << "Row " << i << " has " << two_column_xy.at(i).size();
			info << " cols.\n";	
			throw TracerException( info.str() );			
		}

		double x = two_column_xy[i][0];
		double y = two_column_xy[i][1];
		std::pair<double,double> xy(x,y);
		func.at(i) = xy;
	}

	assert_func_is_not_empty();
	sort_function_arguments();
	assert_no_duplicate_argument_on_sorted_arguments();
}
//------------------------------------------------------------------------------
double Func1D::interploate_value_at_argument(const double argument)const{
	return at_with_Boundary_Mode_STRICT(argument);
}
//------------------------------------------------------------------------------
std::vector< std::pair<double,double> >::const_iterator Func1D::get_upper_bound(
	double func_arg
)const{
	return std::upper_bound(func.begin(),func.end(), func_arg, comp_upp);	
}
//------------------------------------------------------------------------------
double Func1D::at_with_Boundary_Mode_STRICT(const double func_arg)const{

	std::vector< std::pair<double,double> >::const_iterator upper =
		get_upper_bound(func_arg);

	// in mode STRICT it is not accepted when the argument is out of the
	// function definition range. All calculations are aborted.
	if(upper == func.begin() || ( (upper == func.end()) && func_arg > (func.end()-1)->first) ) {
		
		std::stringstream info;
		info << exception_header();
		info << "Expected argument to be in range of arguments ";
		info << "[ " << func.begin()->first << ", ";
		info << (func.end()-1)->first << " ] ";
		info << ", but actual argument = " << func_arg << "\n";
		throw TracerException( info.str() );	
	}

	return interpolate_linear(*(upper-1), *upper, func_arg);
}
//------------------------------------------------------------------------------
std::string Func1D::get_print()const{
	std::stringstream out;

	out << "_Func1D_\n";
	uint counter = 0;
	for( std::pair<double,double> d : func ) {	
		counter++;
		out << counter << " \t "<< d.first << " \t " << d.second << "\n";

	}	
	out << "\n";
	return out.str();
}
//------------------------------------------------------------------------------
void Func1D::sort_function_arguments(){
	// sort the func using the arguments
	std::sort(func.begin(), func.end());
}
//------------------------------------------------------------------------------
void Func1D::assert_no_duplicate_argument_on_sorted_arguments()const {
	// there must not be an argument twice! This is enforced here by throwing an
	// exception in case it is. Since the list is now sorted, same arguments will
	// be next to each other. We go through the vector and compare each element
	// with its upper neighbor.
	for(uint i=0; i < func.size()-1; i++) {
		if( func.at(i).first == func.at(i+1).first ) {
			std::stringstream info;
			info << exception_header();
        	info << "Expected argument " << func.at(i).first;
        	info << " to be unique, but actual its not, see position: " << i;
        	info << "\n";
			throw TracerException(info.str());	
		}
	}	
}
//------------------------------------------------------------------------------
/*double Func1D::at(const double x)const {

	std::vector<double>::const_iterator upper = upper_bound(x);

	if( upper==_func.begin() || ( ( upper==_func.end() ) && x > (_func.end()-1)->at(0) ) ){
		
		std::stringstream info;
		info << exception_header();
		info << "Expected argument to be in range of arguments ";
		info << "[ " << _func.begin()->at(0) << ", ";
		info << (_func.end()-1)->at(0) << " ] ";
		info << ", but actual argument = " << x << "\n";
		throw TracerException( info.str() );	
	}

	//const double slope = (*(upper-1))[2];
	//const double lower_x = (*(upper-1))[0];
	//const double lower_y = (*(upper-1))[2];
	return 0.0;
	
	//return slope*x + lower_y- slope*lower_x;
}
//------------------------------------------------------------------------------
bool Func1D::comp( const double A, const std::vector<double> row ){
	return ( row[0] > A);
}
//------------------------------------------------------------------------------
std::vector<std::vector<double>>::const_iterator Func1D::upper_bound(double x)const {
	return std::upper_bound(_func.begin(),_func.end(), x, comp);	
}*/
//------------------------------------------------------------------------------
double Func1D::interpolate_linear(
	const std::pair<double,double> p0, 
	const std::pair<double,double> p1, 
	const double x
)const{
	// 	point p0: (x0, y0)
	//	point p1: (x1, y1)
	//	the line in between point p0 and p1 is
	//	f(x) = slope*x + y_intercept
	//
	//	slope = (y0 - y1) / (x0 - x1)
	//	y_intercept = y0 - slope*x0
	
	double slope = (p0.second - p1.second) / (p0.first - p1.first);
	return slope*x + p0.second - slope*p0.first;
}
//------------------------------------------------------------------------------
bool Func1D::comp_upp( const double A, const std::pair<double,double> B ){
	return ( B.first > A);
}
//------------------------------------------------------------------------------
std::string Func1D::exception_header()const {
	return "Func1D:\n";
}
//------------------------------------------------------------------------------
double Func1D::get_weighted_mean_of_value()const{

	double Range = get_range_of_argument();

	if(Range == 0.0){
		std::stringstream info;
		info << exception_header();
		info << "Can not integrate on function in range 0.0";
		throw TracerException( info.str() );			
	}else{
		return get_integral_over_value()/Range;		
	}
}
//------------------------------------------------------------------------------
void Func1D::assert_func_is_not_empty()const {
	if(func.size() == 0 ) {
		std::stringstream info;
		info << exception_header();
		info << "Function is empty.\n";
		throw TracerException( info.str() );			
	}	
}
//------------------------------------------------------------------------------
double Func1D::get_integral_over_value()const {

	double integral_over_value = 0.0;

	for(std::vector< std::pair<double,double> >::const_iterator it = 
		func.begin();
		it < (func.end()-1); 
		it++
	){	
		integral_over_value = 
			integral_over_value +
			0.5*( (it+1)->first - it->first )*(it->second + (it+1)->second );
	}

	return integral_over_value;
}
//------------------------------------------------------------------------------
double Func1D::get_range_of_argument()const{
	return (func.end()-1)->first - func.begin()->first;
}