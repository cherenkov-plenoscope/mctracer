// Copyright 2014 Sebastian A. Mueller
#ifndef XML_DOCUMENT_H_
#define XML_DOCUMENT_H_

#include <string>
#include "pugi/pugixml.hpp"
#include "Node.h"

namespace Xml {

class Document {
    std::string path;
    std::shared_ptr<std::string> file;
    pugi::xml_document pugi_doc;
    pugi::xml_parse_result pugi_result;
 public:
    explicit Document(const std::string path);
    Node node()const;
 private:
    void read_in_documnet();
    void assert_xml_file_is_valid();
};

std::string pugi_status_msg(int encoding);

}  // namespace Xml

#endif  // XML_DOCUMENT_H_
