#include <iostream>
#include <fstream>
#include <sstream>

class dout
	//! "double out" class.  use this object to output the same value to two stream
{
private:
	std::ostream &os1, &os2;
public:
	explicit dout(std::ostream &_os1, std::ostream &_os2) : os1(_os1), os2(_os2) {};
	template <typename T>
	dout& operator<< (const T &rhs) { os1 << rhs;  os2 << rhs; return *this; };
	dout& operator<< (std::ostream& (*__pf)(std::ostream&)) { __pf(os1); __pf(os2); return *this; };
	/*!<  Interface for manipulators, such as \c std::endl and \c std::setw
	For more information, see ostream header */
};

