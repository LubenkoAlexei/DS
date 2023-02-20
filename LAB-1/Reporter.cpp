#include <iostream>
#include <fstream>
#include "employee.h"

using std::cin;
using std::cout;
using std::endl;
using std::ios;
using std::fstream;

int comparator(const void*a, const void*b){
	employee* e1=(employee*)a;
	employee* e2=(employee*)b;
	return e1->num-e2->num;
}

int main(int argc, char* argv[]){
	fstream fin(argv[1], ios::in|ios::binary|ios::ate);
	ofstream fout(argv[2]);
	fin.seekg(0, ios::end);
	int n=fin.tellg()/sizeof(employee);
	fin.seekg(0, ios::beg);
	cout<<n;
	double salary=atoi(argv[3]);
	fout<<argv[1]<<endl;
	fout<<"Number, Name, Hours, Salary"<<endl;
	employee* e=new employee[n];
	fin.read((char*)e, sizeof(employee)*n);
	qsort(e, n, sizeof(employee), comparator);
	for(int i = 0; i<n;i++){
		fout<<e[i].num<<" "<<e[i].name<<" "<<e[i].hours<<" "<< static_cast<double>(e[i].hours*salary)<<endl;
	}
	fout.close();
	fin.close();
	delete[]e;
	return 0;
}
