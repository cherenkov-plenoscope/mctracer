//======================================================================
// Author: 	Sebastian Mueller 
// 		Department of Physics 
//		University of Dortmund in Germany
//		Chair e5b supervised by Prof. Dr. Dr. Wolfgang Rhode
//		year 2013
// Title:	
//			Tracer is a raytracing rendering programm made by the author
//			in his freetime.
//			
//			A rendering algorithm transforms 3D objects and light 
//			phenomena into a 2D plane, aka an image.
//
//			Tracer was inspired by all the beautifull light 
//			phenomenas in nature which are visible by the human eye 
//			while riding ones bikecycle.
//		
//			This version of tracer is the first cpp implementation.
//			When you are new to raytracing consider to use "pov ray" to
//			fit your wishes.
//			Tracer is much less powerfull by means of varity of the 
//			supported features. 
//			It is only used because the author likes it and is known to
//			it. But anyway the authuor invites you to make tracer a 
//			powerfull tool in scientific research.
//			 
// 			This version was made to investigate optical devices like
//			Telescopes.
//			Espacially the Cherenkov Light FACT-Telescope on La Palma,
//			Spain
//			
//	features:
//			Tracer supports a tree structure to store its 3D objects.
//			A pre-tracer is used to generate a list of hit candidates
//			using a maximum norm calculation of each pixel.
//			This is the most important feature which gains a speed up
//			in the number of objects n in a scene by
//			O(n^1) to O(log(n)).
//  compile:
//			cmake .
//			make
//
//======================================================================

#include <iostream> 
#include <string>
#include <sstream>

#include "WorldFactory.h"
#include "Vector3D.h"

//====================
#include "Ray.h"
#include "Photon.h"
#include "CameraRay.h"
//====================
#include "ListOfPropagations.h"

#include "FreeOrbitCamera.h"

#include "CsvHandler.h"
// namespaces
using namespace std;

//======================================================================
// main
//======================================================================
int main(){
	//==================================================================
	// start tracer
	//==================================================================
	int system_call_return_value = system("clear");
	system_call_return_value = system("clear");
	system_call_return_value = system("clear");
	
	stringstream out;
	out.str("");
	out<<" ____________________________________________________ "<<endl;
	out<<"|                                                    |"<<endl;
	out<<"|               welcome to MCtracer                  |"<<endl;
	out<<"|                                                    |"<<endl;
	out<<"|   high performance raytracing tool to design and   |"<<endl;
	out<<"|  investigate optical devices for particle physics  |"<<endl;
	out<<"|                                                    |"<<endl;
	out<<"|  Author: Sebastian Mueller              year 2013  |"<<endl;
	out<<"|____________________________________________________|"<<endl;
	cout<<out.str();

	//==================================================================
	// set up global settings
	//==================================================================
	GlobalSettings settings;
	try{

		settings.set_max_number_of_reflections(3);
		settings.set_csv_decimal_presicion(6);

	}catch(TracerException& error){

		error.ReportException();
		return false;
	}	
	//==================================================================
	// list of propagations
	//==================================================================	
	ListOfPropagations mylist("my little list");
	
	Ray* Hans;
	Hans = new Ray;
	Hans->set_base(13,42,3.1415);
	Hans->set_dir(666.3,1.1e3,6.0e-19);
	Hans->disp();
	
	Photon* Franz;
	Franz = new Photon(*Hans,475e-9);
	Franz->disp();
	
	CameraRay* Bob;
	Bob = new CameraRay;
	Bob->disp();
	
	mylist.push_back(Hans);
	mylist.push_back(Franz);
	mylist.push_back(Bob);
	
	cout << mylist;
	mylist.export_csv("mylist.csv",settings);
	
	// test csv parser
	/*
    ifstream	infile("mylist.csv");
    ofstream	outfile;
    outfile.open("my_Gnarf.csv");

    CsvRow		row;
    while(infile >> row)
    {
    	cout << "4th Element(" << row[3] << ")\n";
 		outfile << row;
    }
    outfile.close();
	*/

	//==================================================================
	// open / read file
	//==================================================================
	WorldFactory file2world;
	
	string user_input;
	cout << "Enter a file to load: ";
	cin  >> user_input;
	
	bool loading_file_was_successful = file2world.load_file(user_input);
	
	if(loading_file_was_successful){

		CartesianFrame *Mworld = file2world.get_pointer_to_world();
		
		//mylist.propagate(Mworld,&settings);
		//cout << Mworld->get_frame_prompt_including_children();
		
		FreeOrbitCamera free;
		free.set_free_orbit(Mworld,&settings);
		free.start_free_orbit();
	}
	
	return 0;
}
