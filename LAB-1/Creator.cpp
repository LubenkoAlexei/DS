#include <iostream>
#include <fstream>
#include "employee.h"

using std::cin;
using std::cout;
using std::endl;
using std::fstream;

int main (int argc, char* argv[]){
	int n = atoi(argv[1]);
	fstream fout(argv[2], ios::out|ios::binary);
	employee* e = new employee[n];
	for(int i = 0; i < n; i++){
		cout << "Enter employee number: ";
		cin >> e[i].num;
		cout << "Enter employee name(10 symbols max): ";
		cin >> e[i].name;
		cout << "Enter working hours: ";
		cin >> e[i].hours;
	}
	fout.write((const char*)e, sizeof(employee)* n);
	fout.close();
	delete[] e;
	return 0;
}
