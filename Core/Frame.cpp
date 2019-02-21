// Copyright 2014 Sebastian A. Mueller
#include "Core/Frame.h"
#include "Frames.h"
#include <set>
#include <exception>
#include <sstream>
#include <iostream>
using std::string;
using std::stringstream;
using std::vector;

namespace relleums {

const char Frame::PATH_DELIMITER = '/';
const unsigned int Frame::MAX_NUMBER_CHILDREN = 16;
const double Frame::MIN_STRUCTURE_SIZE = 1e-6;

Frame Frame::VOID_FRAME;

Frame::Frame():
    bounding_sphere_radius(0.0),
    mother(&VOID_FRAME),
    root_frame(this) {}

Frame::~Frame() {
    for (Frame* child : children)
        delete child;
}

HomTra3 Frame::calculate_frame2world()const {
    if (has_mother())
        return mother->T_frame2world*T_frame2mother;
    else
        return T_frame2mother;
}

void Frame::set_name_pos_rot(
    const string name,
    const Vec3 pos,
    const Rot3 rot
) {
    set_name(name);
    pos_in_mother = pos;
    rot_in_mother = rot;

    // The only transformation set yet is the frame2mother. The others are set
    // when the post initialization is performed.
    T_frame2mother.set_transformation(rot_in_mother, pos_in_mother);
}

void Frame::set_name(const string name) {
    assert_name_is_valid(name);
    this->name = name;
    this->name.shrink_to_fit();
}

void Frame::assert_name_is_valid(const string name_to_check)const {
    if (name_to_check.empty()) {
        stringstream info;
        info << "Expected name of frame not to be empty, ";
        info << "but actually it is empty.";
        throw std::invalid_argument(info.str());
    }

    unsigned int char_pos = 0;
    for (auto single_character : name_to_check) {
        if (isspace(single_character)) {
            stringstream info;
            info << "Expected name of frame '" << name_to_check << "'' ";
            info << "to have no whitespaces, but actual the char at pos ";
            info << char_pos << " is a whitespace: '";
            info << name_to_check << "'.";
            throw std::invalid_argument(info.str());
        }
        char_pos++;
    }

    if (StringTools::string_contains_char(name_to_check, PATH_DELIMITER)) {
        std::stringstream info;
        info << "Expected name of frame '" << name_to_check << "' ";
        info << "to not contain any char of '" << PATH_DELIMITER << "', ";
        info << "but actual it does.";
        throw std::invalid_argument(info.str());
    }
}

string Frame::str()const {
    stringstream out;
    out << "frame: " << name << "\n";
    out << "| pos in mother: " << pos_in_mother.str() << "\n";
    out << "| rot in mother: " << rot_in_mother.str() << "\n";
    out << "| pos in world:  ";
    out << (has_mother()? get_position_in_world().str(): "root") << "\n";
    out << "| bounding radius: " << bounding_sphere_radius << "m\n";
    return out.str();
}

string Frame::get_tree_print()const {
    stringstream out;
    out << name << ", pos " << pos_in_mother.str() << ", r ";
    out << bounding_sphere_radius << "m\n";

    for (Frame* child : children)
        out << StringTools::place_first_infront_of_each_new_line_of_second(
            "| ",
            child->get_tree_print());

    return out.str();
}

void Frame::erase(const Frame* child_rm) {
    bool found = false;
    for (unsigned int i=0; i < children.size(); i++) {
        if (children.at(i) == child_rm) {
            delete children.at(i);
            children.erase(children.begin()+i);
            found = true;
        }
    }
    if (!found) {
        stringstream info;
        info << "Expected frame '" << name << "'' ";
        info << "to have child '" << child_rm->name;
        info << "', but actual there is no such child.";
        throw std::out_of_range(info.str());
    }
}

void Frame::init_tree_based_on_mother_child_relations() {
    cluster_children();
    init_root();
    init_frame2world();
    update_bounding_sphere();
}

void Frame::init_frame2world() {
    // Run from top to bottom through the tree.
    T_frame2world = calculate_frame2world();

    for (Frame* child : children)
        child->init_frame2world();
}

void Frame::update_bounding_sphere() {
    // Run from bottom to the top through the tree.
    for (Frame *child : children)
        child->update_bounding_sphere();
    if (has_children())
        bounding_sphere_radius =
            Frames::bounding_sphere_radius(children, Vec3::ORIGIN);
}

void Frame::init_root() {
    // Run from top to bottom through the tree.
    if (has_mother())
        root_frame = mother->get_root();
    else
        root_frame = this;

    for (Frame* child : children)
        child->init_root();
}

const Frame* Frame::get_root()const {
    return root_frame;
}

string Frame::get_path_in_tree_of_frames()const {
    // The delimiter sign is '/' as for directorys on
    // unix systems.
    // eg. City/Street14/house18/roof/chimney/chimney_wall_2
    if (has_mother())
        return mother->get_path_in_tree_of_frames() + PATH_DELIMITER + name;
    else
        return "";
}

bool Frame::has_mother()const {
    return mother != &VOID_FRAME;
}

bool Frame::has_children()const {
    return children.size() > 0;
}

#include "Core/Ray.h"
#include "Core/Intersection.h"

void Frame::calculate_intersection_with(
    const Ray* ray,
    vector<Intersection> *intersections
)const {
    (void)*ray;
    (void)*intersections;
}

void Frame::cluster_children() {
    if (children.size() > MAX_NUMBER_CHILDREN) {
        vector<Frame*> oct_tree[8];

        // assign children temporarly to octtree
        for (Frame* child : children) {
            child->mother = &VOID_FRAME;
            oct_tree[child->pos_in_mother.get_octant()].push_back(child);
        }
        children.clear();

        for (unsigned int sector=0; sector < 8; sector++) {
            if (Frames::positions_in_mother_are_too_close_together(
                    oct_tree[sector])
            ) {
                warn_about_close_frames();
                for (Frame* child : oct_tree[sector]) {
                    if (
                        child->get_bounding_sphere_radius() <
                        MIN_STRUCTURE_SIZE
                    )
                        warn_small_child(child);

                    child->mother = this;
                    children.push_back(child);
                }
            } else {
                if (oct_tree[sector].size() == 1) {
                    oct_tree[sector].at(0)->mother = this;
                    this->children.push_back(oct_tree[sector].at(0));
                } else if (oct_tree[sector].size() > 1) {
                    Vec3 octant_center = Frames::bounding_sphere_center(
                        oct_tree[sector]);

                    Frame* octant = append<Frame>();
                    octant->set_name_pos_rot(
                        "oct_"+std::to_string(sector),
                        octant_center,
                        Rot3::UNITY);

                    for (Frame* sector_child : oct_tree[sector]) {
                        if (
                            !sector_child->has_children() &&
                            sector_child->get_bounding_sphere_radius() <
                            MIN_STRUCTURE_SIZE
                        )
                            warn_small_child(sector_child);

                        sector_child->pos_in_mother =
                            sector_child->pos_in_mother - octant_center;

                        sector_child->T_frame2mother.set_transformation(
                            sector_child->rot_in_mother,
                            sector_child->pos_in_mother);

                        sector_child->mother = octant;
                        octant->children.push_back(sector_child);
                    }
                }
            }
        }
    }

    for (Frame* child : children)
        child->cluster_children();
}

void Frame::warn_about_close_frames()const {
    stringstream out;
    out << "___Warning___\n";
    out << __FILE__ << " " << __func__ << "(frame) " << __LINE__ << "\n";
    out << "The children of the frame '" << name;
    out << "' are stucked very close together.\n";
    out << "No tree structure can be used to accelerate ";
    out << "the photon propagation in such a scenery.\n";
    out << "Procesing continues, but poor performance is expected.\n";
    std::cerr << out.str();
}

void Frame::warn_small_child(const Frame* frame)const {
    stringstream out;
    out << "___Warning___\n";
    out << __FILE__ << " " << __func__ << "(frame) " << __LINE__ << "\n";
    out << "Frame: " << frame->name << " is neglected. ";
    out << "Contour radius is below " << MIN_STRUCTURE_SIZE << "m, i.e. ";
    out << frame->get_bounding_sphere_radius() << "m.\n";
    std::cerr << out.str();
}

string Frame::get_name()const {
    return name;
}

Vec3 Frame::get_position_in_mother()const {
    return pos_in_mother;
}

Rot3 Frame::get_rotation_in_mother()const {
    return rot_in_mother;
}

Vec3 Frame::get_position_in_world()const {
    return T_frame2world.get_translation();
}

double Frame::get_bounding_sphere_radius()const {
    return bounding_sphere_radius;
}

const HomTra3* Frame::frame2mother()const {
    return &T_frame2mother;
}

const HomTra3* Frame::frame2world()const {
    return &T_frame2world;
}

const vector<Frame*>* Frame::get_children()const {
    return &children;
}

void Frame::update_rotation(const Rot3 rot) {
    rot_in_mother = rot;
    T_frame2mother.set_transformation(rot_in_mother, pos_in_mother);
    init_frame2world();
}

void Frame::assert_no_children_duplicate_names()const {
    // this also checks for duplicate frames
    std::set<string> unique_set;

    for (Frame* child : children) {
        auto ret = unique_set.insert(child->get_name());
        if (ret.second == false) {
            stringstream info;
            info << __FILE__ << ", " << __LINE__ << "\n";
            info << "The frame '" << get_name();
            info << "' has a duplicate child name '";
            info << child->get_name();
            info << "', maybe even the child is a duplicate itself.";
            throw std::invalid_argument(info.str());
        }
    }

    for (Frame* child : children)
        child->assert_no_children_duplicate_names();
}

}  // namespace relleums
