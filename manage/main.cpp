#include <iostream>												//Defines class Employee
#include "map.h"														//Defines template Map<Key,Value>

/* EMPLOYEE CLIENT */

using namespace std;

int main(void)
{
	typedef unsigned int ID; 											//Identification number of Employee
	Map<ID,Employee> database;											//Database of employees

	database.Add(761028073,Employee("Jan Kowalski","salesman",28)); 	//Add first employee: name: Jan Kowalski, position: salseman, age: 28,
	database.Add(510212881,Employee("Adam Nowak","storekeeper",54)); 	//Add second employee: name: Adam Nowak, position: storekeeper, age: 54
	database.Add(730505129,Employee("Anna Zaradna","secretary",32)); 	//Add third employee: name: Anna Zaradna, position: secretary, age: 32

	//database.Show();
	cout << database << endl;											//Print database

	Map<ID,Employee> newDatabase = database;							//Make a copy of database

	Employee* pE;
	pE = newDatabase.Find(510212881);									//Find employee using its ID
	pE->position = "salesman";											//Modify the position of employee
	pE = newDatabase.Find(761028073);									//Find employee using its ID
	pE->age = 29;														//Modify the age of employee

	database = newDatabase;												//Update original database

	cout << database << endl;											//Print original database
	return 0;
}
