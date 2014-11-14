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

	//std::vector< std::pair<double,double> >::const_iterator low,upp;
	std::vector< std::pair<double,double> >::const_iterator upp;
	//low=std::lower_bound(func.begin(),func.end(), argument, comp_low);
	upp=std::upper_bound(func.begin(),func.end(), argument, comp_upp);
/*
	if(upp == func.begin()){
		std::cout << "below lower boundary" <<"\n";
	}

	if(upp == func.end()){
		std::cout << "above upper boundary" <<"\n";
	}
*/
	//std::cout << "upper_bound: " << upp->first << "\n";
	//std::cout << "lower_bound: " << low->first << "\n";
	double a;
	switch(mode){
		//----------------------------------------------------------------------
		case STRICT:
		// in mode STRICT it is not accepted when the argument is out of the
		// function definitio range. All calculations are aborted.
			if(upp == func.begin() || upp == func.end()){
				
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
				info << argument << " [" << Unit_of_argument << "]\n";
				throw XmlIoException(info.str(), this);	
			}

			// linear interpolation
			a = (upp->second-(upp-1)->second)/(upp->first-(upp-1)->first);
			return a*argument + upp->second - a*upp->first;
		break;
		//----------------------------------------------------------------------
		case ZERO:
		// In mode ZERO the value 0.0 is returned in case the argument passed 
		// in is out of the function definition range. Execution is continued.
			if(upp == func.begin() || upp == func.end()){
				return 0.0;
			}

			// linear interpolation
			a = (upp->second-(upp-1)->second)/(upp->first-(upp-1)->first);
			return a*argument + upp->second - a*upp->first;		
		break;
		//----------------------------------------------------------------------
		case CLOSEST:
		// in mode CLOSEST an argument above the aupper limit is assigned to
		// the value of the upper argument limit and in case the argument is below
		// the lower argument limit it is assigned to the value of the lower
		// limit vice versa.
			// return the lower boundary value in case the argument is below
			// the lowest argument in the function range
			if(upp == func.begin())
				return func.begin()->second;
			// return the upper boundary value in case the argument is above
			// the uppest argument in the function range
			if(upp == func.end())
				return (func.end()-1)->second; 

			// return a linear interpolation else
			a = (upp->second-(upp-1)->second)/(upp->first-(upp-1)->first);
			return a*argument + upp->second - a*upp->first;
		break;
		//----------------------------------------------------------------------
		// this can only happen when the mode of the xml file was excepted but 
		// does not show up in this list.
		default:
			std::stringstream info;
			info << "In Function1D.at(argument) a boundary mode number: ";
			info << mode << " was chosen. But this mode is not dealt in the ";
			info << "at(double argument) function of Function1D.\n";
			throw XmlIoException(info.str(), this);	
		return 0.0;
	}
}
//------------------------------------------------------------------------------
std::string Function1D::name()const{
	return Name;
}
//------------------------------------------------------------------------------
double Function1D::pedantic_str2float(std::string text)const{

	if(text.compare("")==0){
		std::stringstream info;
    	info << "String to be parsed to floating number is empty";
    	throw XmlIoException(info.str(), this);	
	}
	
	char * e;
	double FloatingNumber = std::strtod(text.c_str(), &e);
	if (*e != 0) {
		std::stringstream info;
    	info << "String to be parsed to floating number is not valid: ";
    	info << text;
		throw XmlIoException(info.str(), this);	
	}
	return FloatingNumber;
}
//------------------------------------------------------------------------------
std::string Function1D::prompt()const{
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
    	// the xml file is fine


    	XmlNode = doc.child(xml_func_node_id.c_str());

		//check the top level child which should be >function1D<
		if( doc.child(xml_func_node_id.c_str()) == nullptr){

			std::stringstream info;
        	info << "I can not find the <" << xml_func_node_id << ">";
			info << "statement in the xml file.";
			throw XmlIoException(info.str(), this);		
		}else{

			pugi::xml_node function_node = 
			doc.child(xml_func_node_id.c_str());
			//------------------------------------------------------------------
			// parse the name
			if(	function_node.
				attribute(xml_func_name_id.c_str())
				== nullptr
			){
				// can not find the name identifier
				std::stringstream info;
	        	info << "I can not find the ";
	        	info << xml_func_name_id;
				info << " in the " << xml_func_node_id << "statement.";
				throw XmlIoException(info.str(), this);						
			}
			// name identifier was found and is assigned now
			Name = function_node.
			attribute(xml_func_name_id.c_str()).value();
			
			//------------------------------------------------------------------
			// parse arguments units
			if(	function_node.
				attribute(xml_func_arg_unit_id.c_str())
				== nullptr
			){
				// can not find argument unit
				std::stringstream info;
	        	info << "I can not find the ";
	        	info << xml_func_arg_unit_id;
				info << " in the " << xml_func_node_id << "statement.";
				throw XmlIoException(info.str(), this);			
			}
			// argument unit was found
			Unit_of_argument = function_node.
			attribute(xml_func_arg_unit_id.c_str()).
			value();
			//------------------------------------------------------------------
			// parse value units
			if(	function_node.
				attribute(xml_func_val_unit_id.c_str())
				== nullptr
			){
				// can not find argument unit
				std::stringstream info;
	        	info << "I can not find the ";
	        	info << xml_func_val_unit_id;
				info << " in the " << xml_func_node_id << "statement.";
				throw XmlIoException(info.str(), this);		
			}
			// argument unit was found
			Unit_of_value = function_node.
			attribute(xml_func_val_unit_id.c_str()).
			value();
			//------------------------------------------------------------------
			// parse the optional boundary mode. 
			// when not defined the default is used
			if(	function_node.
				attribute(xml_func_boundary_mode_id.c_str())
				!= nullptr
			){
				// there is a custom boundary mode set in the xml file
				// the default mode is now overwritten
				std::string xml_boundary_mode = function_node.
				attribute(xml_func_boundary_mode_id.c_str()).value();

				if(pedantic_str_comp(xml_boundary_mode,"STRICT")){
					mode = STRICT;
				}else if(pedantic_str_comp(xml_boundary_mode,"ZERO")){
					mode = ZERO;
				}else if(pedantic_str_comp(xml_boundary_mode,"CLOSEST")){
					mode = CLOSEST;
				}else{
					// the mode is unknown. this is not acceptable
					std::stringstream info;
		        	info << "The boundary mode: "<< xml_func_boundary_mode_id;
		        	info << "=" << xml_boundary_mode;
		        	info << " is unknown.";
		        	throw XmlIoException(info.str(), this);		
				}

			}
			//------------------------------------------------------------------
			// parse the actual data
			for(pugi::xml_node_iterator it = function_node.begin();
				it != function_node.end();
			 	++it
			){
				XmlNode = *it;

			    if(	xml_func_pair_id.compare(it->name())!=0){
					// this is not a data pair as I expect it. 
					std::stringstream info;
		        	info << "I do not know a node of type ";
		        	info << it->name();
					throw XmlIoException(info.str(), this);					    	
			    }else{
			    	//----------------------------------------------------------
			    	// parse the argument
			    	if( it->attribute(xml_func_arg_id.c_str())==nullptr ){
						// every data pair needs an argument and a value!
						std::stringstream info;
			        	info << "I can not find the " << xml_func_arg_id;
			        	info << " attribute.";
						throw XmlIoException(info.str(), this);					    			
			    	}

			    	std::string arg = 
			    	it->attribute(xml_func_arg_id.c_str()).value();

			    	//----------------------------------------------------------
			    	// parse the value
			    	if( it->attribute(xml_func_val_id.c_str())==nullptr ){
						// every data pair needs an argument and a value!
						std::stringstream info;
			        	info << "I can not find the " << xml_func_val_id;
			        	info << " attribute.";
						throw XmlIoException(info.str(), this);					    			
			    	}

			    	std::string val = 
			    	it->attribute(xml_func_val_id.c_str()).value();			    	

					//----------------------------------------------------------
					// parse the strings to floating numbers
				    std::pair< double , double > *a_new_data_pair;
				    a_new_data_pair = new std::pair< double , double >;

				    a_new_data_pair->first  = 
				    pedantic_str2float(arg);

				    a_new_data_pair->second = 
				    pedantic_str2float(val);

				    func.push_back(*a_new_data_pair);
			    }
			}
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
void Function1D::check_and_sort(){
	// sort the func using the arguments
	std::sort(func.begin(), func.end());

	// there must not be an argument twice! This is enforced here by throwing an
	// exception in case it is. Since the list is now sorted, same arguments will
	// be next to each other. We go through the vector and compare each element
	// with its upper neighbor.
	for(uint i=0; i < func.size()-1; i++){
		if( func.at(i).first == func.at(i+1).first ){
			// can not find the name identifier
			std::stringstream info;
        	info << "The argument " << func.at(i).first;
        	info << " must not appear twice!";
			throw XmlIoException(info.str(), this);	
		}
	}
}
//------------------------------------------------------------------------------
// I wanted to use this function to calculate the linear interpolations in the 
// .at(double argument) function but passing in the references here causes some 
// trouble I do not understand jet.
/*
double Function1D::interpolate_linear(
	const double &x0,const double &y0,
	const double &x1,const double &y1,
	const double &x
){
	// 	point 0: (x0,y0)
	//	point 1: (x1,y1)
	//	the line in between point 0 and 1 is
	//	f(x) = a*x + b
	//
	//	a = (y0-y1)/(x0-x1)
	//	b = y0-a*x0
	
	double a = (y0-y1)/(x0-x1);
	return a*x + y0 -a*x0;
}*/
//------------------------------------------------------------------------------
bool Function1D::pedantic_str_comp(
	const std::string A,const std::string B)const{
	return (A.compare(B)==0 && A.length() == B.length() );
}
//------------------------------------------------------------------------------
double Function1D::mean_value()const{
	// integral
	double integral_over_value = 0.0;
	for(std::vector< std::pair<double,double> >::const_iterator it = 
		func.begin();
		it < (func.end()-1); 
		it++
	){	

		integral_over_value = integral_over_value +
		0.5*( (it+1)->first - it->first )*(it->second + (it+1)->second );
		//std::cout << "step: "<<it->first<<" to "<<(it+1)->first<<"\n";
	}
	//std::cout << "arg begin " << func.begin()->first <<"\n";
	//std::cout << "arg end   " << (func.end()-1)->first <<"\n";
	return integral_over_value/( (func.end()-1)->first - func.begin()->first );
}