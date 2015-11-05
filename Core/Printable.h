#ifndef __PRINTABLE__
#define __PRINTABLE__

#include <iostream>

class Printable{

public:
	virtual std::string get_print() const = 0;

	friend std::ostream& operator<<(std::ostream& os, const Printable& to_be_displayed)
	{
    	os << to_be_displayed.get_print();
    	return os;
	};
};

#endif //__PRINTABLE__

