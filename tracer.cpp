//------------------------------------------------------------------------------
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
//
//  dependencies:
//          If on Ubuntu (14.04), you can install the dependencies using
//          syaptic, jist install these two:
//          
//          * libopencv-dev
//          * libgtest-dev
//
//          gtest is a header only library, but in order to use it
//          one needs to build it first. See this thread on askubuntu
//          for details:
//          http://askubuntu.com/questions/145887/why-no-library-files-installed-for-google-test
//
//  compile:
//          mkdir build
//          cd build
//          cmake ..
//          make
//
//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------
int main(){
	//--------------------------------------------------------------------------
	// start tracer
	//--------------------------------------------------------------------------
	int system_call_return_value = system("clear");
	
	stringstream out;
	out.str("");
	//               1         2         3         4         5         6
	//      123456789012345678901234567890123456789012345678901234567890
	out << " ___________________________________________________________ \n";
	out << "|                                                           |\n";
	out << "|                 welcome to MCtracer                       |\n";
	out << "|                                                           |\n";
	out << "|      high performance raytracing tool to design and       |\n";
	out << "|     investigate optical devices for particle physics      |\n";
	out << "|                                                           |\n";
	out << "|     Author: Sebastian Mueller              year 2013      |\n";
	out << "|___________________________________________________________|\n";
	cout << out.str();

	//--------------------------------------------------------------------------
	// set up settings
	//--------------------------------------------------------------------------
	GlobalSettings settings;
	try{

		settings.set_max_number_of_reflections(3);
		settings.set_csv_decimal_presicion(9);

	}catch(TracerException& error){

		error.ReportException();
		return 0;
	}	

	//--------------------------------------------------------------------------
	// list of propagations
	//--------------------------------------------------------------------------	
	/*
	ListOfPropagations mylist("my_little_list");
	
	Photon* Hans;
	Hans = new Photon( 0,0.1,0, 0,0,1, 475e-9);
	Hans->SetID(13);
	Hans->disp();
	
	Photon* Franz;
	Franz = new Photon( 0,0,0, 0,0,1, 475e-9);
	Franz->SetID(42);
	Franz->disp();
	
	//CameraRay* Bob;
	//Bob = new CameraRay;
	//Bob->disp();
	
	mylist.push_back(Hans);
	mylist.push_back(Franz);
	//mylist.push_back(Bob);
	
	cout << mylist;
	mylist.export_propagations_csv(settings);


	ListOfPropagations my2ndlist("my_2nd_little_list");
	my2ndlist.import_propagations_csv("my_little_list.csv",settings);
	// test csv parser
	
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
	
	try{

		file2world.load_file(user_input);

		CartesianFrame *Mworld = file2world.get_pointer_to_world();
		//std::cout << Mworld->get_frame_prompt_including_children();


		// test list of propagations
		//mylist.propagate(Mworld,&settings);

		//mylist.export_history_csv(settings);
		
		FreeOrbitCamera free;
		free.set_free_orbit(Mworld,&settings);
		free.start_free_orbit();

	}catch(TracerException& error){

		error.ReportException();
		return 0;
	}	

	return 0;
}
