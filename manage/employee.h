/* Employee INTERFACE */
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

class Employee
{
	public:
	unsigned int age;
	string name;
	string position;
		Employee();
		Employee(string name, std::string position, unsigned int age);
		~Employee();
		string show();
};

template < class T >
string cvrt_to_string( T t )
{
    stringstream ss;
    ss << t;
    return ss.str();
}
