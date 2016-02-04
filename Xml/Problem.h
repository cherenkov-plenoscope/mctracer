//=================================
// include guard
#ifndef __XmlProblem_H_INCLUDED__
#define __XmlProblem_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "XmlIO/pugixml.hpp"
#include "Node.h"
#include <memory>
#include <vector>
#include <string>

//------------------------------------------------------------------------------
namespace Xml {

	class Problem {

		const std::shared_ptr<std::string> file;
		const int offset;
		const std::string path;

		std::vector<ptrdiff_t> new_lines;
		uint column;
		uint line;
	public:

		Problem(
			const std::shared_ptr<std::string> _file, 
			const int _offset,
			const std::string _path
		);
		Problem(Node node);
		std::string get_problem_section_from_original_file()const;
		uint get_line()const;
		uint get_column()const;
		std::string get_path()const;
	private:

		void init();
		void locate_new_lines_in_xml_file();
		void locate_problem_column_and_line_using_new_lines();
	};
}
#endif // __XmlProblem_H_INCLUDED__ 
