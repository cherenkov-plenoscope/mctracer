#include "SceneryFactory.h"
#include "FrameFab.h"
#include "Scenery/Primitive/Primitive.h"
#include "Scenery/StereoLitography/StereoLitography.h"
#include "Scenery/SegmentedReflector/SegmentedReflector.h"
#include "Xml/Factory/Function.h"
using StringTools::is_equal;
using std::stringstream;

//------------------------------------------------------------------------------
namespace Xml {
SceneryFactory::SceneryFactory(const string path): xml_path(path), xml_doc(path) {

    const Node root_node = xml_doc.node().first_child();
    
    if(root_node.has_attribute("author"))
        author = root_node.attribute("author");
    if(root_node.has_attribute("comment"))
        comment = root_node.attribute("comment");

    telescopes = new TelescopeArrayControl();
    raw_sensors = new vector<PhotonSensor::Sensor*>;

    scenery = &Scenery::void_scenery;
}
//------------------------------------------------------------------------------
void SceneryFactory::append_to_frame_in_scenery(Frame* frame, Scenery* scenery) {
    this->scenery = scenery;
    const Node root_node = xml_doc.node().first_child();
    make_geometry(frame, root_node);
}
//------------------------------------------------------------------------------
void SceneryFactory::make_geometry(Frame* mother, const Node node) {

    add_to_sensors_if_sensitive(mother, node);
    add_to_array_if_telescope(mother, node);

    for(Node child=node.first_child(); child; child=child.next_child()) {
        if(is_equal(child.name(), "function"))
            add_function(child);
        else if(is_equal(child.name(), "color"))
            add_color(child);
        else if(is_equal(child.name(), "frame"))
            make_geometry(add_Frame(mother, child), child);
        else if(is_equal(child.name(), "disc"))
            make_geometry(add_Disc(mother, child), child);
        else if(is_equal(child.name(), "sphere"))
            make_geometry(add_Sphere(mother, child), child);
        else if(is_equal(child.name(), "plane"))
            make_geometry(add_Plane(mother, child), child);
        else if(is_equal(child.name(), "hex_plane"))
            make_geometry(add_HexPlane(mother, child), child);
        else if(is_equal(child.name(), "cylinder"))
            make_geometry(add_Cylinder(mother, child), child);
        else if(is_equal(child.name(), "annulus"))
            make_geometry(add_Annulus(mother, child), child);
        else if(is_equal(child.name(), "segmented_reflector"))
            make_geometry(add_SegmentedReflector(mother, child), child);
        else if(is_equal(child.name(), "sphere_cap_hexagonal"))
            make_geometry(add_SphereCapWithHexagonalBound(mother, child), child); 
        else if(is_equal(child.name(), "sphere_cap_rectangular"))
            make_geometry(add_SphereCapWithRectangularBound(mother, child), child); 
        else if(is_equal(child.name(), "triangle"))
            make_geometry(add_Triangle(mother, child), child);
        else if(is_equal(child.name(), "light_field_sensor"))
            make_geometry(add_light_field_sensor(mother, child), child);
        else if(is_equal(child.name(), "stl"))
            make_geometry(add_STL(mother, child), child);
    }
}
//------------------------------------------------------------------------------
void SceneryFactory::add_to_array_if_telescope(Frame* frame, const Node node) {
    
    if(node.has_child("set_telescope"))
        telescopes->add_to_telescope_array(frame);        
}
//------------------------------------------------------------------------------
void SceneryFactory::add_to_sensors_if_sensitive(Frame* frame, const Node node) {

    if(node.has_child("set_sensitive")) {
        const uint id = node.child("set_sensitive").attribute2int("id");
        PhotonSensor::Sensor* sens = new PhotonSensor::Sensor(id, frame);
        raw_sensors->push_back(sens);
    }
}
//------------------------------------------------------------------------------
Frame* SceneryFactory::add_Frame(Frame* mother, const Node node) {

    FrameFab fab(node);
    Frame* frame = mother->append<Frame>();
    frame->set_name_pos_rot(fab.name, fab.pos, fab.rot);
    return frame;
}
//------------------------------------------------------------------------------
Frame* SceneryFactory::add_Disc(Frame* mother, const Node node) {

    FrameFab framefab(node);
    Disc* disc = mother->append<Disc>();
    disc->set_name_pos_rot(framefab.name, framefab.pos, framefab.rot);

    disc->set_inner_color(surface_color(node));
    disc->set_outer_color(surface_color(node));
    disc->set_outer_reflection(surface_refl(node));
    disc->set_inner_reflection(surface_refl(node)); 
    disc->set_radius(node.child("set_disc").attribute2double("radius"));
    return disc;
}
//------------------------------------------------------------------------------
Frame* SceneryFactory::add_Sphere(Frame* mother, const Node node) {

    FrameFab framefab(node);
    Sphere* sphere = mother->append<Sphere>();
    sphere->set_name_pos_rot(framefab.name, framefab.pos, framefab.rot);

    sphere->set_inner_color(surface_color(node));
    sphere->set_outer_color(surface_color(node));
    sphere->set_outer_reflection(surface_refl(node));
    sphere->set_inner_reflection(surface_refl(node));   
    sphere->set_radius(
        node.child("set_sphere").attribute2double("radius"));
    return sphere;
}
//------------------------------------------------------------------------------
Frame* SceneryFactory::add_Plane(Frame* mother, const Node node) {

    FrameFab framefab(node);
    Plane* plane = mother->append<Plane>();
    plane->set_name_pos_rot(framefab.name, framefab.pos, framefab.rot);

    plane->set_inner_color(surface_color(node));
    plane->set_outer_color(surface_color(node));
    plane->set_outer_reflection(surface_refl(node));
    plane->set_inner_reflection(surface_refl(node));
    plane->set_x_y_width(
        node.child("set_plane").attribute2double("x_width"), 
        node.child("set_plane").attribute2double("y_width")
    );
    return plane;
}
//------------------------------------------------------------------------------
Frame* SceneryFactory::add_HexPlane(Frame* mother, const Node node) {

    FrameFab framefab(node);
    HexPlane* plane = mother->append<HexPlane>();
    plane->set_name_pos_rot(framefab.name, framefab.pos, framefab.rot);

    plane->set_inner_color(surface_color(node));
    plane->set_outer_color(surface_color(node));
    plane->set_outer_reflection(surface_refl(node));
    plane->set_inner_reflection(surface_refl(node));
    plane->set_outer_hex_radius(
        node.child("set_hex_plane").attribute2double("outer_hex_radius"));
    return plane;
}
//------------------------------------------------------------------------------
Frame* SceneryFactory::add_Cylinder(Frame* mother, const Node node) {

    FrameFab framefab(node);
    Cylinder* cyl = mother->append<Cylinder>();
    cyl->set_name_pos_rot(framefab.name, framefab.pos, framefab.rot);

    cyl->set_inner_color(surface_color(node));
    cyl->set_outer_color(surface_color(node));
    cyl->set_outer_reflection(surface_refl(node));
    cyl->set_inner_reflection(surface_refl(node));
    cyl->set_cylinder(
        node.child("set_cylinder").attribute2double("radius"),
        node.child("set_cylinder").attribute2Vec3("start_pos"),
        node.child("set_cylinder").attribute2Vec3("end_pos")
    );
    return cyl;
}
//------------------------------------------------------------------------------
Frame* SceneryFactory::add_Annulus(Frame* mother, const Node node) {

    FrameFab framefab(node);
    Annulus* ann = mother->append<Annulus>();
    ann->set_name_pos_rot(framefab.name, framefab.pos, framefab.rot);

    ann->set_inner_color(surface_color(node));
    ann->set_outer_color(surface_color(node));
    ann->set_outer_reflection(surface_refl(node));
    ann->set_inner_reflection(surface_refl(node));
    ann->set_outer_inner_radius(
        node.child("set_annulus").attribute2double("outer_radius"),
        node.child("set_annulus").attribute2double("inner_radius")
    );
    return ann;
}
//------------------------------------------------------------------------------
Frame* SceneryFactory::add_BiConvexLensHex(Frame* mother, const Node node) {

    FrameFab framefab(node);
    BiConvexLensHexBound* lens = mother->append<BiConvexLensHexBound>();
    lens->set_name_pos_rot(framefab.name, framefab.pos, framefab.rot);

    lens->set_inner_color(surface_color(node));
    lens->set_outer_color(surface_color(node));
    lens->set_outer_reflection(surface_refl(node));
    //lens->set_inner_reflection(surface_refl(node));
    lens->set_inner_refraction(
        scenery->functions.get(node.child("set_medium").attribute("refraction_vs_wavelength"))
    );
    lens->set_curvature_radius_and_outer_hex_radius(
        node.child("set_bi_convex_lens_hex").attribute2double("curvature_radius"),
        node.child("set_bi_convex_lens_hex").attribute2double("outer_radius")
    );
    return lens;
}
//------------------------------------------------------------------------------
Frame* SceneryFactory::add_SphereCapWithHexagonalBound(Frame* mother, const Node node) {

    FrameFab framefab(node);
    SphereCapWithHexagonalBound* cap = mother->append<SphereCapWithHexagonalBound>();
    cap->set_name_pos_rot(framefab.name, framefab.pos, framefab.rot);

    cap->set_inner_color(surface_color(node));
    cap->set_outer_color(surface_color(node));
    cap->set_outer_reflection(surface_refl(node));
    cap->set_inner_reflection(surface_refl(node));
    cap->set_curvature_radius_and_outer_hex_radius(
        2.0*node.child("set_sphere_cap_hexagonal").attribute2double("focal_length"),
        node.child("set_sphere_cap_hexagonal").attribute2double("outer_radius")
    );
    return cap;
}
//------------------------------------------------------------------------------
Frame* SceneryFactory::add_SphereCapWithRectangularBound(Frame* mother, const Node node) {

    FrameFab framefab(node);
    SphereCapWithRectangularBound* cap = mother->append<SphereCapWithRectangularBound>();
    cap->set_name_pos_rot(framefab.name, framefab.pos, framefab.rot);

    cap->set_inner_color(surface_color(node));
    cap->set_outer_color(surface_color(node));
    cap->set_outer_reflection(surface_refl(node));
    cap->set_inner_reflection(surface_refl(node));
    cap->set_curvature_radius_and_x_y_width(
        node.child("set_sphere_cap_rectangular").attribute2double("curvature_radius"),
        node.child("set_sphere_cap_rectangular").attribute2double("x_width"),
        node.child("set_sphere_cap_rectangular").attribute2double("y_width")
    );
    return cap;
}
//------------------------------------------------------------------------------
Frame* SceneryFactory::add_Triangle(Frame* mother, const Node node) {

    FrameFab framefab(node);
    Triangle* tri = mother->append<Triangle>();
    tri->set_name_pos_rot(framefab.name, framefab.pos, framefab.rot);

    tri->set_inner_color(surface_color(node));
    tri->set_outer_color(surface_color(node));
    tri->set_outer_reflection(surface_refl(node));
    tri->set_inner_reflection(surface_refl(node));
    tri->set_corners_in_xy_plane(
        node.child("set_triangle").attribute2double("Ax"),
        node.child("set_triangle").attribute2double("Ay"),
        node.child("set_triangle").attribute2double("Bx"),
        node.child("set_triangle").attribute2double("By"),
        node.child("set_triangle").attribute2double("Cx"),
        node.child("set_triangle").attribute2double("Cy")
    );
    return tri;
}
//------------------------------------------------------------------------------
Frame* SceneryFactory::add_STL(Frame* mother, const Node node) {

    const string file = PathTools::join(
        xml_path.dirname, 
        node.child("set_stl").attribute("file")
    );
    const double scale = node.child("set_stl").attribute2double("scale");

    FrameFab framefab(node);
    SurfaceEntity* object = mother->append<SurfaceEntity>();
    object->set_name_pos_rot(framefab.name, framefab.pos, framefab.rot);
    object->set_inner_color(surface_color(node));
    object->set_outer_color(surface_color(node));
    object->set_outer_reflection(surface_refl(node));
    object->set_inner_reflection(surface_refl(node));
    StereoLitography::add_stl_to_and_inherit_surface_from_surfac_entity(
        file, object, scale
    );
    return object;
}
//------------------------------------------------------------------------------
Frame* SceneryFactory::add_SegmentedReflector(Frame* mother, const Node node) {

    const Node refl = node.child("set_segmented_reflector");
    SegmentedReflector::Config cfg;
    cfg.focal_length = refl.attribute2double("focal_length");
    cfg.DaviesCotton_over_parabolic_mixing_factor = refl.attribute2double("DaviesCotton_over_parabolic_mixing_factor");
    cfg.max_outer_aperture_radius = refl.attribute2double("max_outer_aperture_radius");
    cfg.min_inner_aperture_radius = refl.attribute2double("min_inner_aperture_radius");
    cfg.facet_inner_hex_radius = refl.attribute2double("facet_inner_hex_radius");
    cfg.gap_between_facets = refl.attribute2double("gap_between_facets");
    cfg.reflectivity = surface_refl(node);

    SegmentedReflector::Factory refl_fab(cfg);

    FrameFab fab(node);
    Frame* reflector = mother->append<Frame>();
    reflector->set_name_pos_rot(fab.name, fab.pos, fab.rot);
    refl_fab.add_reflector_mirror_facets_to_frame(reflector);
    return reflector;
}
//------------------------------------------------------------------------------
Frame* SceneryFactory::add_light_field_sensor(Frame* mother, const Node node) {

    FrameFab ffab(node);
    Frame* light_field_sensor = mother->append<Frame>();
    light_field_sensor->set_name_pos_rot(ffab.name, ffab.pos, ffab.rot);

    const Node lfs = node.child("set_light_field_sensor");

    Plenoscope::LightFieldSensor::Config config;
    config.sensor_plane2imaging_system = *light_field_sensor->frame2mother();
    config.expected_imaging_system_focal_length = lfs.attribute2double("expected_imaging_system_focal_length");
    config.expected_imaging_system_max_aperture_radius = lfs.attribute2double("expected_imaging_system_aperture_radius");
    config.max_FoV_diameter = Deg2Rad(lfs.attribute2double("max_FoV_diameter_deg"));
    config.pixel_FoV_hex_flat2flat = Deg2Rad(lfs.attribute2double("hex_pixel_FoV_flat2flat_deg")); 
    config.number_of_paxel_on_pixel_diagonal = lfs.attribute2int("number_of_paxel_on_pixel_diagonal");
    config.housing_overhead = lfs.attribute2double("housing_overhead");
    config.lens_refraction = scenery->functions.get(lfs.attribute("lens_refraction_vs_wavelength"));
    //config.lens_absorbtion = &perfect_transparency;
    config.bin_reflection = scenery->functions.get(lfs.attribute("bin_reflection_vs_wavelength"));


    Plenoscope::PlenoscopeInScenery pis(config);
    Plenoscope::LightFieldSensor::Factory lfs_factory(&pis.light_field_sensor_geometry);
    lfs_factory.add_light_field_sensor_to_frame_in_scenery(
        light_field_sensor,
        scenery);    
    pis.frame = light_field_sensor;    
    pis.light_field_channels = lfs_factory.get_sub_pixels();
    plenoscopes.push_back(pis);

    return light_field_sensor;    
}
//------------------------------------------------------------------------------
const Function::Func1D* SceneryFactory::surface_refl(const Node node)const {
    return scenery->functions.get(node.child("set_surface").attribute("reflection_vs_wavelength"));
}
//------------------------------------------------------------------------------
void SceneryFactory::add_color(const Node node) {
    scenery->colors.add(
        node.attribute("name"),
        node.attribute2Color("rgb"));
}
//------------------------------------------------------------------------------
void SceneryFactory::add_function(const Node node) {
  
    for(Xml::Node child=node.first_child(); child; child=child.next_child()) {

        if(is_equal(child.name(),"linear_interpolation")) {
            Function::LinInterpol* f = scenery->functions.add<Function::LinInterpol>(
                node.attribute("name")
            );
            init_function_with_node(f, child);
        }else if(is_equal(child.name(),"constant")) {
            Function::Constant* f = scenery->functions.add<Function::Constant>(
                node.attribute("name")
            );
            init_function_with_node(f, child);
        }else if(is_equal(child.name(),"polynom3")) {
            Function::Polynom3* f = scenery->functions.add<Function::Polynom3>(
                node.attribute("name")
            );
            init_function_with_node(f, child);
        }else if(is_equal(child.name(),"concatenation")) {
            Function::Concat* f = scenery->functions.add<Function::Concat>(
                node.attribute("name")
            );
            init_function_with_node(f, child, &scenery->functions);
        }else{
            stringstream info;
            info << "SceneryFactory " << __FILE__ << ", " << __LINE__;
            info << "Unknown xml child in '" << child.name() << "' in node ";
            info << "'" << node.name() << "'";
            throw TracerException(info.str());
        }
    }
}
//------------------------------------------------------------------------------
const Color* SceneryFactory::surface_color(const Node node)const {
    return scenery->colors.get(node.child("set_surface").attribute("color"));
}
//------------------------------------------------------------------------------
PhotonSensors::Sensors SceneryFactory::sensors()const {
    return PhotonSensors::Sensors(*raw_sensors);
}
//------------------------------------------------------------------------------
}//Xml