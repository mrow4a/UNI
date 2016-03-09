#include <iostream>
#include <map>
#include "employee.h"
#include <typeinfo>

using namespace std;
typedef unsigned int ID;

//class must be forward declared because it contains a friend function template
template <class emp_id, class EMP> class Map;

//friend function also must be forwards declared
template <class emp_id, class EMP>
ostream& operator<< ( ostream& os, Map<emp_id,EMP> &_empmap);

template <class emp_id, class EMP> class Map{
	private:
		map<ID, Employee> employees;
	public:
		class no_such_attribute_error{};
		void Add(ID _emp_id,Employee _emp);
		Employee* Find(ID _emp_id);
		friend ostream& operator<< <>( ostream& os, Map<emp_id,EMP> &_empmap);
};

template<class emp_id, class EMP>
ostream& operator<<( ostream& os, Map<emp_id,EMP> &_empmap )
{
	Employee *_emp_ptr;
	ID _emp_id;
	for( map<ID, Employee>::iterator ii=(_empmap.employees).begin(); ii!=(_empmap.employees).end(); ++ii)
	{
		_emp_ptr = &((*ii).second);
		_emp_id = (*ii).first;
		os << _emp_id << " " << _emp_ptr->show() << endl;
	}
	//os << "Hello " << typeid(_empmap.employees).name();
	return os;
}

template<class emp_id, class EMP>
void Map<emp_id, EMP>::Add(ID _emp_id,Employee _emp)
{
	employees.insert(pair<ID,Employee>(_emp_id,_emp));
}

template<class emp_id, class EMP>
Employee* Map<emp_id, EMP>::Find(ID _emp_id_find)
{
	map<ID, Employee>::iterator ii = (employees.find(_emp_id_find));
	if (ii != employees.end()) return &((*ii).second);
	throw no_such_attribute_error();
}
