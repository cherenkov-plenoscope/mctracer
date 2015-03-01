#include "FactTelescope.h"
//==================================================================
void FactTelescope::init_outer_mirror_surface() {
	mirror_colour = new ColourProperties(255,255,255);
	outer_mirror_reflection = new ReflectionProperties(0.9);
}
//==================================================================
void FactTelescope::init_inner_mirror_surface() {
	inner_mirror_colour = new ColourProperties(64,64,64);
}
//==================================================================
void FactTelescope::assert_alpha_geometry_parameter_is_valid(
	const double alpha
)const {

	if(alpha < 0.0 || alpha > 1.0){
		std::stringstream info;
		info << "FactTelescope::" << __func__ << "()\n";
		info << "Expected 0.0 <= alpha <= 1.0, but actual: alpha = ";
		info << alpha;
		throw TracerException(info.str());
	}	
}
//==================================================================
FactTelescope::FactTelescope(double alpha){
	
	init_outer_mirror_surface();
	init_inner_mirror_surface();
	assert_alpha_geometry_parameter_is_valid(alpha);
	bool prompt = false;

	// mirror properties
	Vector3D  	mirror_pos; 		//to be set individual
	Rotation3D 	mirror_rotation;	//to be set individual

	// declare FACT geometry, only mirror positions
	focal_length_telescope = 4.8892;
	/// distribution of focal length of mirrors (4.8892 plus minus 0.002)m 
	/// as in Ann Kristins Diploma Thesis
	MirrorRadius_m = 0.345;
	const int int_number_of_circles = 4;
	const int int_number_of_mirrors_in_circle[int_number_of_circles] = 
		{6,6,6,12};
	//========================
	// all radii
	//========================
	double circle_radius_of_mirror_positions[4]=
	{0.62, 1.07, 1.24, 1.64}; //as in Ann Kristins Diploma Thesis
	//{0.61, 1.07, 1.24, 1.67};
	//========================
	// 1st circle R1 = 0.62m
	//========================
	double sixths = 1.0/6.0;
	double  circle_angle_R1[6]=
	{	1.0*sixths*M_PI + M_PI/2.0, 
		3.0*sixths*M_PI + M_PI/2.0,
		5.0*sixths*M_PI + M_PI/2.0, 
		7.0*sixths*M_PI + M_PI/2.0,
		9.0*sixths*M_PI + M_PI/2.0,
		11.0*sixths*M_PI + M_PI/2.0 };

	//========================
	// 2nd circle R2 = 1.07m
	//========================
	double thirds = 1.0/3.0;
	double  circle_angle_R2[6]=
	{	0.0 + M_PI/2.0,
		1.0*thirds*M_PI + M_PI/2.0,
		2.0*thirds*M_PI + M_PI/2.0,
		3.0*thirds*M_PI + M_PI/2.0,
		4.0*thirds*M_PI + M_PI/2.0,
		5.0*thirds*M_PI + M_PI/2.0};
	//========================
	// 3rd circle R3 = 1.24m
	//========================
	//double sixths = 1.0/6.0; //the same as R1
	double  circle_angle_R3[6]=
	{	1.0*sixths*M_PI + M_PI/2.0 ,
		3.0*sixths*M_PI + M_PI/2.0 ,
		5.0*sixths*M_PI + M_PI/2.0 ,
		7.0*sixths*M_PI + M_PI/2.0 ,
		9.0*sixths*M_PI + M_PI/2.0 ,
		11.0*sixths*M_PI + M_PI/2.0 };
	//========================
	// 4th circle R4 = 1.64m
	//========================
	double sixteesixths = 1.0/66.0;
	double  circle_angle_R4[12]=
	{	  4.0*sixteesixths*M_PI + M_PI/2.0,
		 26.0*sixteesixths*M_PI + M_PI/2.0,
		 48.0*sixteesixths*M_PI + M_PI/2.0,
		 70.0*sixteesixths*M_PI + M_PI/2.0,
		 92.0*sixteesixths*M_PI + M_PI/2.0,
		114.0*sixteesixths*M_PI + M_PI/2.0,
		 18.0*sixteesixths*M_PI + M_PI/2.0,
		 40.0*sixteesixths*M_PI + M_PI/2.0,
		 62.0*sixteesixths*M_PI + M_PI/2.0,
		 84.0*sixteesixths*M_PI + M_PI/2.0,
		106.0*sixteesixths*M_PI + M_PI/2.0,
		128.0*sixteesixths*M_PI + M_PI/2.0
	};
	//==============================================================
	// HYBRID
	// pre calculate mean image distance of mirrors
	//==============================================================
	double mean_image_distance_deviation = 0.0;
	int mirror_number =0;
	double angle = 0.0;
	for(int radius=0;radius<int_number_of_circles;radius++){
		//cout<<"Radius: ";
		//cout<<circle_radius_of_mirror_positions[radius]<<endl;
		for(	int circ1=0;
				circ1<int_number_of_mirrors_in_circle[radius];
				circ1++){
				// get angle 
				switch(radius){
				case 0: angle=circle_angle_R1[circ1]; break;
				case 1: angle=circle_angle_R2[circ1]; break;
				case 2: angle=circle_angle_R3[circ1]; break;
				case 3: angle=circle_angle_R4[circ1]; break;
				}
				
				mirror_number++;
				
				//cout<<"alpha: "<<alpha<<" [1]"<<endl;
				double z_Davies_Cotton = ( 
				focal_length_telescope-
				sqrt( 
				pow(focal_length_telescope,2.0) -
				pow(circle_radius_of_mirror_positions[radius],2.0)
				)
				);
				//cout<<"z_Davies_Cotton: "<<z_Davies_Cotton<<" [m]"<<endl;
				
				double z_parabolic = 
				1.0/(4.0*focal_length_telescope)*
				pow(circle_radius_of_mirror_positions[radius],2.0);
				//cout<<"z_parabolic: "<< z_parabolic <<" [m]"<<endl;

				mean_image_distance_deviation += 
				(1.0-alpha)*(z_Davies_Cotton - z_parabolic);	
			}
		}
	mean_image_distance_deviation = 
	mean_image_distance_deviation/(double)mirror_number;
	
	/// fine correction 0.99
	mean_image_distance_deviation  = 
	mean_image_distance_deviation *0.99;
	telescope_table<<std::setprecision(9);
	telescope_table<<" ________________________________________________"<<std::endl;
	telescope_table<<"| mean_image_distance_deviation: g(alpha) "<<std::endl;
	telescope_table<<"| "<<mean_image_distance_deviation<<" [m]"<<std::endl;
	telescope_table<<"|________________________________________________"<<std::endl;		
	
	//==============================================================
	// allocate and initialize mirrors
	//==============================================================
	mirror_number =0;
	angle = 0.0;
	double final_mean_distance_of_mirrors_and_image_plane = 0.0;

	for(int radius=0; radius<int_number_of_circles; radius++) {
		for(int circ1=0; circ1<int_number_of_mirrors_in_circle[radius];	circ1++) {
			// get angle 
			switch(radius){
				case 0: angle=circle_angle_R1[circ1]; break;
				case 1: angle=circle_angle_R2[circ1]; break;
				case 2: angle=circle_angle_R3[circ1]; break;
				case 3: angle=circle_angle_R4[circ1]; break;
			}

			//==================================================
			// HYBRID 
			//==================================================
			telescope_table << "________Mirror_" << mirror_number+1;
			telescope_table << "_{" << radius+1 << "," << circ1+1 << "}______________\n";
			telescope_table << "| alpha: " << alpha << " [1]\n";
			
			double z_Davies_Cotton = ( 
				focal_length_telescope-
				sqrt( 
					pow(focal_length_telescope,2.0) -
					pow(circle_radius_of_mirror_positions[radius],2.0)
				)
			);

			telescope_table<<"| z_Davies_Cotton: ";
			telescope_table<<z_Davies_Cotton<<" [m]\n";
			
			double z_parabolic = 
			1.0/(4.0*focal_length_telescope)*
			pow(circle_radius_of_mirror_positions[radius],2.0);
			telescope_table<<"| z_parabolic:     ";
			telescope_table<< z_parabolic <<" [m]\n";
			
			// the naive mirror position in z without image 
			// distance adaption 
			double z_hybrid_tick = 
				alpha*z_Davies_Cotton + (1.0-alpha)*z_parabolic;
			
			telescope_table << "| z_hybrid_tick:   ";
			telescope_table << z_hybrid_tick << " [m]\n";
				
			double z_hybrid = 
				z_hybrid_tick + mean_image_distance_deviation;

			telescope_table << "| z_hybrid     :   ";
			telescope_table << z_hybrid << " [m]\n";
			
			mirror_pos.set(
				circle_radius_of_mirror_positions[radius]*sin(angle),
				circle_radius_of_mirror_positions[radius]*cos(angle),
				z_hybrid
			);

			telescope_table << "| final mirror position m_{" << radius+1 << "," << circ1+1 << "}:   ";
			telescope_table << mirror_pos.get_string() << "\n";
			//==================================================
			// calculate mirror orientation
			//==================================================

				//==============================================
				// calculate rotation axis
				//==============================================
			Vector3D focal_point(0.0, 0.0, focal_length_telescope);
			
			Vector3D focal_point_to_mirror_pos = focal_point - mirror_pos;
				
			Vector3D EZ; EZ.set_unit_vector_z();
			Vector3D vec_rotation_axis;
			
			vec_rotation_axis = 
				focal_point_to_mirror_pos.CrossProduct(EZ);

				//==============================================
				// calculate rotation angle to rotation axis
				//==============================================
			
			Vector3D direction_of_focal_point_to_mirror_pos =
				focal_point_to_mirror_pos/
				focal_point_to_mirror_pos.norm2();
			
			telescope_table << "| direction_of_focal_point_to_mirror_pos: ";
			telescope_table << direction_of_focal_point_to_mirror_pos.get_string() << "\n";
		
			double angle_between_EZ_and_focal_point_to_mirror_pos =
				acos(EZ*direction_of_focal_point_to_mirror_pos);
			
			double dbl_rot_angle = 1.0/2.0*
				angle_between_EZ_and_focal_point_to_mirror_pos;
			
			// acos(EZ*vec_mirror_surface_normal_in_center);
			telescope_table << "| rotation angle: ";
			telescope_table << dbl_rot_angle << " [rad], ";
			telescope_table << dbl_rot_angle/(M_PI*2.0)*360 << " [DEG]\n";
			
			final_mean_distance_of_mirrors_and_image_plane +=
			focal_point_to_mirror_pos.norm2();
			
			telescope_table << "| distance mirror to image plane: ";
			telescope_table << focal_point_to_mirror_pos.norm2() << " [m]\n";
			telescope_table << "|____________________________________\n";
			//==================================================
			// set rotation 
			//==================================================
			mirror_rotation.set(vec_rotation_axis, -dbl_rot_angle);
			
			//==================================================
			// test rotation
			//==================================================
			/*
			Vector3D test_vec; test_vec.set_unit_vector_z();
			test_vec.disp();
			
			Vector3D none_translation; none_translation.set(0.0,0.0,0.0);
			HomoTrafo3D homoT_test;
			homoT_test.set_transformation(mirror_rotation,none_translation);
			homoT_test.disp();
			
			homoT_test.transform_orientation(&test_vec);
			cout<<"surface normal: ";
			cout<<"test    normal: ";
			test_vec.disp();
			*/
			//==================================================
			// allocate memory for new mirror
			//==================================================
			//CartesianFrame* mirror = new OpticalMirrorSphericHexagonal;
			SphereCapWithHexagonalBound* mirror = new SphereCapWithHexagonalBound;
			list_of_mirrors.push_back(mirror);
				
			//==================================================
			// initialize mirror
			//==================================================
			mirror_number++;

			std::stringstream mirror_name;
			mirror_name << "fact_mirror_" << mirror_number;
			
			mirror->set_frame(
				mirror_name.str(),
				mirror_pos,
				mirror_rotation
			);
			
			mirror->set_outer_color(mirror_colour);
			mirror->set_inner_color(inner_mirror_colour);
			mirror->set_outer_reflection(outer_mirror_reflection);
				
			mirror->set_focal_length_and_outer_hex_radius(
				focal_length_telescope,
				MirrorRadius_m
			);
		}
	}

	final_mean_distance_of_mirrors_and_image_plane = 
		final_mean_distance_of_mirrors_and_image_plane/mirror_number;

	telescope_table << " ________________________________________________\n";
	telescope_table << "| final_mean_distance_of_mirrors_and_image_plane\n";
	telescope_table << "| " << final_mean_distance_of_mirrors_and_image_plane << "[m]\n";
	telescope_table << "|________________________________________________\n";
	
	if(prompt)
		std::cout << telescope_table.str();

	init();
}
//======================================================================
/*CartesianFrame* get_full_mirror(
	const std::string name,
	const Vector3D pos,
	const Rotation3D rot,
	const double focal_length_telescope,
	const double MirrorRadius_m
)const {

	std::string name_surface = name;
	name_surface.append("_surface");

	CartesianFrame* full_mirror = new CartesianFrame;
	full_mirror.set_frame(name, pos, rot);

	SphereCapWithHexagonalBound* mirror_surface = new SphereCapWithHexagonalBound;
	mirror_surface->set_frame(name_surface, mirror_pos, mirror_rotation);
	mirror->set_outer_surface(outer_mirror_surface);
	mirror->set_inner_surface(inner_mirror_surface);		
	mirror->set_focal_length_and_outer_hex_radius(
		focal_length_telescope,
		MirrorRadius_m
	);

	full_mirror.set_mother_and_child(mirror_surface);

	Rotation3D edge_rotation(0.0, M_PI/2.0, 0.0);

	for(uint i=0; i<6; i++) {
		Plane* mirror_edge = new Plane;
		Plane.set_frame();
	}

}*/
//======================================================================	
void FactTelescope::init(){	
	//set relationships
	for(unsigned int mirror_number=0;
			mirror_number<list_of_mirrors.size();
			mirror_number++)
	{
			set_mother_and_child(list_of_mirrors.at(mirror_number));
	}

	/*for(CartesianFrame* mirror : list_of_mirrors) 
		set_mother_and_child(mirror);*/
}
//======================================================================	
void FactTelescope::disp() {
	std::cout << get_fact_string();
}
//======================================================================	
std::string FactTelescope::get_fact_string() {
	std::stringstream out;
	out << "\n";
	out << "[ FACT Telescope ]\n";
	out << get_frame_print();
	out << "||__________FACT mirrors:_________________________\n";	
	for(unsigned int i=0; i<list_of_mirrors.size(); i++)
			out << list_of_mirrors.at(i)->get_frame_print();
	out << "||________________________________________________\n";
	return out.str();
}
//==================================================================
void FactTelescope::export_table_of_telescope(std::string telescope_table_file_name) {
	
	std::ofstream myfile (telescope_table_file_name.c_str());
	if(myfile.is_open()){
		myfile << telescope_table.str();
		myfile.close();
	}else{
		std::cout << "FACT-telescope->export_table_of_telescope():";
		std::cout << " Unable to open file: ";
		std::cout << telescope_table_file_name << "\n";
	}
}
//======================================================================	
FactTelescope::~FactTelescope(){
	//==============================================================
	// destructor free each mirrors memory
	//==============================================================
	list_of_mirrors.clear();
}
