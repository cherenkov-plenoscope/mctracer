#include "Function1D.h"
//------------------------------------------------------------------------------
Function1D::Function1D(std::string path2xml_input_file){
	set(path2xml_input_file);
}
//------------------------------------------------------------------------------
Function1D::Function1D(){}
//------------------------------------------------------------------------------
void Function1D::set(std::string path2xml_input_file){
	// the xml path is stored in the Function to be accessable to all the sub
	// in here functions throwing exceptions
	XmlName = path2xml_input_file;
	// read in the xml file storing the function data tuples
	read();
	// sort the func using the argument
	check_and_sort();
}
//------------------------------------------------------------------------------
double Function1D::at(const double argument)const{

	std::vector< std::pair<double,double> >::const_iterator upp;

	upp=std::upper_bound(func.begin(),func.end(), argument, comp_upp);

	switch(mode){
		case STRICT:
			return at_with_Boundary_Mode_STRICT(argument); break;
		case ZERO:
			return at_with_Boundary_Mode_ZERO(argument); break;
		case CLOSEST:
			return at_with_Boundary_Mode_CLOSEST(argument); break;
		default:			
			// this can only happen when the mode of the xml file was excepted 
			// but does not show up in this switch on mode.
			std::stringstream info;
			info << "In Function1D.at(argument) a boundary mode number: ";
			info << mode << " was chosen. But this mode is not dealt in the ";
			info << "at(double argument) function of Function1D.\n";
			throw TracerException( info.str() );	
		return 0.0;
	}
}
//------------------------------------------------------------------------------
std::vector< std::pair<double,double> >::const_iterator Function1D::get_upper_bound(
	double func_arg)const{
	return std::upper_bound(func.begin(),func.end(), func_arg, comp_upp);	
}
//------------------------------------------------------------------------------
double Function1D::at_with_Boundary_Mode_ZERO(double func_arg)const{

	std::vector< std::pair<double,double> >::const_iterator upper =
		get_upper_bound(func_arg);

	// In mode ZERO the value 0.0 is returned in case the argument 
	// passed in is out of the function definition range. Execution
	// is continued.
	if(upper == func.begin() || upper == func.end()){
		return 0.0;
	}

	return interpolate_linear(*(upper-1), *upper, func_arg);
}
//------------------------------------------------------------------------------
double Function1D::at_with_Boundary_Mode_STRICT(double func_arg)const{

	std::vector< std::pair<double,double> >::const_iterator upper =
		get_upper_bound(func_arg);

	// in mode STRICT it is not accepted when the argument is out of the
	// function definition range. All calculations are aborted.
	if(upper == func.begin() || upper == func.end()){
		
		std::stringstream info;
		info << "In Function1D.at(argument) an argument was given\n";
		info << "which is out of the boundaries of the function\n";
		info << "definition and the mode to handle this is set to:\n";
		info << "STRICT.\n";
		info << "function name: " << Name << "\n";
		info << "file parsed from: " << XmlName << "\n";
		info << "lower argument boundary: ";
		info << func.begin()->first << " [" << Unit_of_argument<< "]\n";
		info << "upper argument boundary: ";
		info << (func.end()-1)->first << " ["<<Unit_of_argument<< "]\n";
		info << "argument passed in: ";
		info << func_arg << " [" << Unit_of_argument << "]\n";
		throw TracerException( info.str() );	
	}

	return interpolate_linear(*(upper-1), *upper, func_arg);
}
//------------------------------------------------------------------------------
double Function1D::at_with_Boundary_Mode_CLOSEST(double func_arg)const{

	std::vector< std::pair<double,double> >::const_iterator upper =
		get_upper_bound(func_arg);

	// in mode CLOSEST an argument above the aupper limit is assigned
	// to the value of the upper argument limit and in case the
	// argument is below the lower argument limit it is assigned to
	// the value of the lower limit vice versa.

	// return the lower boundary value in case the argument is below
	// the lowest argument in the function range
	if(upper == func.begin())
		return func.begin()->second;
	// return the upper boundary value in case the argument is above
	// the uppest argument in the function range
	if(upper == func.end())
		return (func.end()-1)->second; 

	return interpolate_linear(*(upper-1), *upper, func_arg);
}
//------------------------------------------------------------------------------
std::string Function1D::name()const{
	return Name;
}
//------------------------------------------------------------------------------
std::string Function1D::get_print()const{
	std::stringstream out;

	out << " _Function_1D___________________________________________________\n";
	out << "| number | argument [" << Unit_of_argument << "]";
	out << "| value [" << Unit_of_value << "]\n";
	uint counter = 0;
	for( std::pair<double,double> d : func ){	
		counter++;
	out << "| " << counter << " | "<< d.first << " | " << d.second << "\n";

	}	
	out << "|_______________________________________________________________\n";
	return out.str();
}
//------------------------------------------------------------------------------
void Function1D::read(){
	
    pugi::xml_document doc;

    XmlResult = doc.load_file(XmlName.c_str());
	
    if (XmlResult){

    	assert_child_exists(doc, xml_func_node_id);
		pugi::xml_node function_node = doc.child(xml_func_node_id.c_str());
		//------------------------------------------------------------------
		// parse the name
		assert_attribute_exists(function_node, xml_func_name_id);
		Name = function_node.attribute(xml_func_name_id.c_str()).value();

		//------------------------------------------------------------------
		// parse arguments units
		assert_attribute_exists(function_node, xml_func_arg_unit_id);
		Unit_of_argument = function_node.
		attribute(xml_func_arg_unit_id.c_str()).value();

		//------------------------------------------------------------------
		// parse value units
		assert_attribute_exists(function_node, xml_func_val_unit_id);
		Unit_of_value = function_node.
		attribute(xml_func_val_unit_id.c_str()).value();

		//------------------------------------------------------------------
		// parse the boundary mode
		assert_attribute_exists(function_node, xml_func_boundary_mode_id);

		std::string xml_boundary_mode = function_node.
		attribute(xml_func_boundary_mode_id.c_str()).value();

		set_boundary_mode(xml_boundary_mode);

		//------------------------------------------------------------------
		// parse the actual data
		for(pugi::xml_node_iterator it = function_node.begin();
			it != function_node.end();
		 	++it
		){
			assert_node_name(*it, xml_func_pair_id);

	    	//----------------------------------------------------------
	    	// parse the argument
			assert_attribute_exists(*it, xml_func_arg_id);
	    	std::string arg = it->attribute(xml_func_arg_id.c_str()).value();

	    	//----------------------------------------------------------
	    	// parse the value
	    	assert_attribute_exists(*it, xml_func_val_id);
	    	std::string val = it->attribute(xml_func_val_id.c_str()).value();			    	

			//----------------------------------------------------------
			// parse the strings to floating numbers
		    std::pair< double , double > *a_new_data_pair;
		    a_new_data_pair = new std::pair< double , double >;

		    a_new_data_pair->first  = pedantic_strtod(arg);
		    a_new_data_pair->second = pedantic_strtod(val);

		    func.push_back(*a_new_data_pair);
		}
    }else{
    	// there was an error reading the xml file
    	std::stringstream info;
        info << "XML [" << XmlName << "] ";
        info << "parsed with errors, attr value: [";
        info << doc.child("node").attribute("attr").value() << "]\n";
        info << "Error description: " << XmlResult.description() << "\n";
		info << "Number of character in file: " << XmlResult.offset;
		throw XmlIoException(info.str(), this);	
	}
}
//------------------------------------------------------------------------------
void Function1D::set_boundary_mode(const std::string mode_text){

	if(StringUtilities::is_equal(mode_text,"STRICT")){
		mode = STRICT;
	}else if(StringUtilities::is_equal(mode_text,"ZERO")){
		mode = ZERO;
	}else if(StringUtilities::is_equal(mode_text,"CLOSEST")){
		mode = CLOSEST;
	}else{
		// the mode is unknown. this is not acceptable
		std::stringstream info;
    	info << "The boundary mode: "<< xml_func_boundary_mode_id;
    	info << "=" << mode_text;
    	info << " is unknown.";
    	throw XmlIoException(info.str(), this);		
	}
}
//------------------------------------------------------------------------------
void Function1D::check_and_sort(){
	// sort the func using the arguments
	std::sort(func.begin(), func.end());

	// there must not be an argument twice! This is enforced here by throwing an
	// exception in case it is. Since the list is now sorted, same arguments will
	// be next to each other. We go through the vector and compare each element
	// with its upper neighbor.
	for(uint i=0; i < func.size()-1; i++){

		if( func.at(i).first == func.at(i+1).first ){

			std::stringstream info;
        	info << "The argument " << func.at(i).first;
        	info << " must not appear twice!";
			throw XmlIoException(info.str(), this);	
		}
	}
}
//------------------------------------------------------------------------------
double Function1D::interpolate_linear(
	const std::pair<double,double> p0, 
	const std::pair<double,double> p1, 
	const double x)const{
	// 	point p0: (x0,y0)
	//	point p1: (x1,y1)
	//	the line in between point p0 and p1 is
	//	f(x) = a*x + b
	//
	//	a = (y0-y1)/(x0-x1)
	//	b = y0-a*x0
	
	double a = (p0.second - p1.second)/(p0.first - p1.first);
	return a*x + p0.second - a*p0.first;
}
//------------------------------------------------------------------------------
double Function1D::weighted_mean()const{

	double Range = range();

	if(Range == 0){
		std::stringstream info;
		info << "The weighted mean of the function: '" << Name << "', ";
		info << "defined in file: '" << XmlName <<"' does not exist! ";
		info << "This is because: mean = integral/range; ";
		info << "and the range in this case is ZERO!\n";
		throw TracerException( info.str() );			
	}else{
		return integral()/Range;		
	}
}
//------------------------------------------------------------------------------
double Function1D::integral()const{
	// integral

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
double Function1D::range()const{
	return (func.end()-1)->first - func.begin()->first;
}