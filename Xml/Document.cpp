// Copyright 2014 Sebastian A. Mueller
#include "Xml/Document.h"
#include "Exceptions.h"
#include "Problem.h"
#include <fstream>
#include <sstream>
using std::string;

namespace Xml {

Document::Document(const string path) {
    this->path = path;
    read_in_documnet();
    assert_xml_file_is_valid();
}

Node Document::node()const {
    return Node(pugi_doc, file, path);
}

void Document::read_in_documnet() {
    std::shared_ptr<string> xmlfile (new string);
    file = xmlfile;
    std::ifstream t(path.c_str());

    if (!t) {
        std::stringstream info;
        info << __FILE__ << ", " << __LINE__ << "\n";
        info << "\n";
        info << "Xml Documnet: Can not open '" << path << "'\n";
        throw IoError(info.str());
    }

    t.seekg(0, std::ios::end);
    file->reserve(t.tellg());
    t.seekg(0, std::ios::beg);

    file->assign(
        (std::istreambuf_iterator<char>(t)),
        std::istreambuf_iterator<char>());

    pugi_result = pugi_doc.load(file->c_str());
}

void Document::assert_xml_file_is_valid() {
    if (pugi_result.status == pugi::status_ok) {
        return;
    } else if (
        pugi_result.status == pugi::status_unrecognized_tag ||
        pugi_result.status == pugi::status_bad_pi ||
        pugi_result.status == pugi::status_bad_comment ||
        pugi_result.status == pugi::status_bad_start_element ||
        pugi_result.status == pugi::status_bad_attribute ||
        pugi_result.status == pugi::status_bad_end_element ||
        pugi_result.status == pugi::status_end_element_mismatch
    ) {
        Problem xml_problem(file, pugi_result.offset, path);
        std::stringstream info;
        info << __FILE__ << ", " << __LINE__ << "\n";
        info << "\n";
        info << "Xml Syntax Error in '" <<
        path << "', line " << xml_problem.get_line()+1;
        info << ", column " << xml_problem.get_column() << "\n";
        info << "\n";
        info << xml_problem.get_problem_section_from_original_file();
        info << "\n";
        info << "Pugi Xml reader Error message:\n";
        info << pugi_status_msg(pugi_result.status) << "\n";
        throw SyntaxError(info.str());
    } else {
        std::stringstream info;
        info << __FILE__ << ", " << __LINE__ << "\n";
        info << "\n";
        info << "Xml Internal parser Error.\n";
        info << "Pugi Xml reader Error message:\n";
        info << pugi_status_msg(pugi_result.status) << "\n";
        throw InternalError(info.str());
    }
}

string Document::pugi_status_msg(int encoding)const {
    // The pugi xml error encodings with detailed informaion
    // http://pugixml.googlecode.com/svn/tags/latest/docs/manual/loading.html
    switch (encoding) {
        case pugi::status_ok:
            return"status_ok means that no error was encountered during "
            "parsing; the source stream represents the valid XML document "
            "which was fully parsed and converted to a tree.";
        break;
        case pugi::status_file_not_found:
            return"status_file_not_found is only returned by load_file "
            "function and means that file could not be opened.";
        break;
        case pugi::status_io_error:
            return"status_io_error is returned by load_file function and "
            "by load functions with std::istream/std::wstream arguments; "
            "it means that some I/O error has occurred during reading the "
            "file/stream.";
        break;
        case pugi::status_out_of_memory:
            return"status_out_of_memory means that there was not enough "
            "memory during some allocation; any allocation failure during "
            "parsing results in this error.";
        break;
        case pugi::status_internal_error:
            return"status_internal_error means that something went horribly "
            "wrong; currently this error does not occur.";
        break;
        case pugi::status_unrecognized_tag:
            return"status_unrecognized_tag means that parsing stopped due "
            "to a tag with either an empty name or a name which starts with "
            "incorrect character, such as #.";
        break;
        case pugi::status_bad_pi:
            return"status_bad_pi means that parsing stopped due to incorrect "
            "document declaration/processing instruction.";
        break;
        case pugi::status_bad_comment:
            return"status_bad_comment, status_bad_cdata, status_bad_doctype "
            "and status_bad_pcdata mean that parsing stopped due to the "
            "invalid construct of the respective type.";
        break;
        case pugi::status_bad_start_element:
            return"status_bad_start_element means that parsing stopped "
            "because starting tag either had no closing > symbol or "
            "contained some incorrect symbol.";
        break;
        case pugi::status_bad_attribute:
            return"status_bad_attribute means that parsing stopped because "
            "there was an incorrect attribute, such as an attribute without "
            "value or with value that is not quoted (note that <node attr=1> "
                "is incorrect in XML)";
        break;
        case pugi::status_bad_end_element:
            return"status_bad_end_element means that parsing stopped because "
            "ending tag had incorrect syntax (i.e. extra non-whitespace "
                "symbols between tag name and >).";
        break;
        case pugi::status_end_element_mismatch:
            return"status_end_element_mismatch means that parsing stopped "
            "because the closing tag did not match the opening one "
            "(i.e. <node></nedo>) or because some tag was not closed at all.";
        break;
        // this seems not to be supported by the pugi xml used here
        // case pugi::status_no_document_element:
        // return"status_no_document_element means that no element nodes were
        // discovered during parsing; this usually indicates an empty or
        // invalid document.";
        break;
        default:
            return"Unknown pugi::xml encoding";
    }
}

}  // namespace Xml
