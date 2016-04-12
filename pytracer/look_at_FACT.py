# coding: utf-8
from mctracer import *
settings = TracerSettings()

world = Frame()
world.set_name_pos_rot("world", Vec3.null, Rot3.null)

ground = Disc()
ground.set_name_pos_rot("ground", Vec3(0.0, 0.0, -2.0), Rot3.null)
ground.set_radius(1e3)

ground_color = Color(0,55,0)
ground.set_outer_color(ground_color)
ground.set_inner_color(ground_color)

FACT = TelescopeFrame("FACT", Vec3.null, Rot3.null)

		
reflector_generator = SegmetedReflectorGenerator()
reflector_generator.set_focal_length(4.889)
reflector_generator.set_facet_spacing(0.62)
reflector_generator.set_max_outer_diameter(3.5)
reflector_generator.set_min_inner_diameter(0.5)
reflector_generator.set_hybrid_geometry(0.5)

reflector = reflector_generator.get_reflector()


Sensor = Disc()
Sensor.set_name_pos_rot("Sensor", Vec3(0.0, 0.0, 4.889), Rot3.null)
 		
sensor_color = Color(255,0,0)
Sensor.set_radius(0.32)
Sensor.set_outer_color(sensor_color)
Sensor.set_inner_color(sensor_color)


sensor_housing_lower_cap = Disc()
sensor_housing_lower_cap.set_name_pos_rot(
	"sensor_housing_lower_cap", 
	Vec3(0.0, 0.0, 4.890), 
	Rot3.null)
sensor_housing_lower_cap.set_radius(0.32)

sensor_housing_color = Color(200,255,200)
sensor_housing_lower_cap.set_outer_color(sensor_housing_color)
sensor_housing_lower_cap.set_inner_color(sensor_housing_color)

FACT.set_mother_and_child(reflector)
FACT.set_mother_and_child(Sensor)
FACT.set_mother_and_child(sensor_housing_lower_cap)	

world.set_mother_and_child(FACT)
world.set_mother_and_child(ground)

world.init_tree_based_on_mother_child_relations()

free = FreeOrbitCamera(world, settings)