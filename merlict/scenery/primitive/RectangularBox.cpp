// Copyright 2014 Sebastian A. Mueller
#include "merlict/scenery/primitive/RectangularBox.h"
#include <math.h>

namespace relleums {

void RectangularBox::set_xyz_width(
    const double x_width,
    const double y_width,
    const double z_width
) {
    Plane* walls[6];
    for (unsigned int c = 0; c < 6; c++)
        walls[c] = append<Plane>();

    // along z axis
    Vec3 z_off = Vec3(0.0, 0.0, 0.5*z_width);

    walls[0]->set_name_pos_rot(
        get_name()+"_wall_0", z_off, Rot3(0.0, 0.0, 0.0));
    walls[0]->set_x_y_width(x_width, y_width);
    walls[0]->take_boundary_layer_properties_from(this);

    walls[1]->set_name_pos_rot(
        get_name()+"_wall_1", z_off*-1.0, Rot3(M_PI, 0.0, 0.0));
    walls[1]->set_x_y_width(x_width, y_width);
    walls[1]->take_boundary_layer_properties_from(this);

    // along x axis
    Vec3 x_off = Vec3(0.5*x_width, 0.0, 0.0);

    walls[2]->set_name_pos_rot(
        get_name()+"_wall_2", x_off, Rot3(0.0, -M_PI/2.0, 0.0));
    walls[2]->set_x_y_width(z_width, y_width);
    walls[2]->take_boundary_layer_properties_from(this);

    walls[3]->set_name_pos_rot(
        get_name()+"_wall_3", x_off*-1.0, Rot3(0.0, M_PI/2.0, 0.0));
    walls[3]->set_x_y_width(z_width, y_width);
    walls[3]->take_boundary_layer_properties_from(this);

    // along y axis
    Vec3 y_off = Vec3(0.0, 0.5*y_width, 0.0);

    walls[4]->set_name_pos_rot(
        get_name()+"_wall_4", y_off, Rot3(M_PI/2.0, 0.0, 0.0));
    walls[4]->set_x_y_width(x_width, z_width);
    walls[4]->take_boundary_layer_properties_from(this);

    walls[5]->set_name_pos_rot(
        get_name()+"_wall_5", y_off*-1.0, Rot3(-M_PI/2.0, 0.0, 0.0));
    walls[5]->set_x_y_width(x_width, z_width);
    walls[5]->take_boundary_layer_properties_from(this);
}

}  // namespace relleums
