// Copyright 2014 Sebastian A. Mueller
#ifndef XML_NODE_H_
#define XML_NODE_H_

#include <string>
#include <memory>
#include "pugi/pugixml.hpp"
#include "Tools/Tuple3.h"
#include "Core/Vec3.h"
#include "Core/Rot3.h"
#include "Core/Color.h"

namespace Xml {

class Node {
    friend class Problem;
    pugi::xml_node node;
    const std::shared_ptr<std::string> file;
    const std::string path;

 public:
    void operator=(const Node eq);
    Node(
        const pugi::xml_node _node,
        const std::shared_ptr<std::string> _file,
        const std::string _path);
    std::string name()const;
    std::string attribute(const std::string attribute)const;
    Node child(const std::string child)const;
    Node first_child()const;
    Node next_child();
    operator bool()const {
        return node ? true: false;
    }

    bool has_child(const std::string child)const;
    bool has_attribute(const std::string attribute)const;
    std::string xml_path()const;

    int to_int(const std::string attribute_name)const;
    double to_double(const std::string attribute_name)const;
    bool to_bool(const std::string attribute_name)const;
    Tuple3 to_tuple3(const std::string attribute_name)const;
    Vec3 to_vec3(const std::string attribute_name)const;
    Rot3 to_rot3(const std::string attribute_name)const;
    Color to_color(const std::string attribute_name)const;

    class AttributeIsNoInt : public std::invalid_argument {
        using invalid_argument::invalid_argument;
    };

    class AttributeIsNoDouble : public std::invalid_argument {
        using invalid_argument::invalid_argument;
    };

    class AttributeIsNoBool : public std::invalid_argument {
        using invalid_argument::invalid_argument;
    };

    class AttributeIsNoTuple3 : public std::invalid_argument {
        using invalid_argument::invalid_argument;
    };

    class AttributeIsNoVec3 : public std::invalid_argument {
        using invalid_argument::invalid_argument;
    };

    class AttributeIsNoRot3 : public std::invalid_argument {
        using invalid_argument::invalid_argument;
    };

    class AttributeIsNoColor : public std::invalid_argument {
        using invalid_argument::invalid_argument;
    };

 private:
    void assert_child_exists(const std::string child)const;
    void assert_attribute_exists(const std::string attribute)const;
};

}  // namespace Xml

#endif  // XML_NODE_H_
