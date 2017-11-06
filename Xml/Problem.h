//=================================
// include guard
#ifndef __XmlProblem_H_INCLUDED__
#define __XmlProblem_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "pugi/pugixml.hpp"
#include "Node.h"
#include <memory>
#include <vector>
#include <string>
using std::string;
//------------------------------------------------------------------------------
namespace Xml {

	class Problem {

		const std::shared_ptr<string> file;
		const int offset;
		const string path;

		std::vector<ptrdiff_t> new_lines;
		unsigned int column;
		unsigned int line;
	public:

		Problem(
			const std::shared_ptr<string> _file, 
			const int _offset,
			const string _path
		);
		Problem(Node node);
		string get_problem_section_from_original_file()const;
		unsigned int get_line()const;
		unsigned int get_column()const;
		string get_path()const;
	private:

		void init();
		void locate_new_lines_in_xml_file();
		void locate_problem_column_and_line_using_new_lines();
	};
}
#endif // __XmlProblem_H_INCLUDED__ 
