#include "employee.h"

/* Employee IMPLEMENTATION */

Employee::Employee(std::string name, std::string position,unsigned int age){
	this->age=age;
	this->name=name;
	this->position=position;
}

Employee::Employee(){
	this->age=0;
	this->name="";
	this->position="";
}

Employee::~Employee() {}

string Employee::show(){
	return ("Name: " + name + " ,Position: " + position + " ,Age: " + cvrt_to_string(age));
}


