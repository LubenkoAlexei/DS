#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <stdlib.h>
#include "employee.h"
const int SIZE_OF_BUFF = 10;
const int NDIGIT = 10;
const int SIZE_OF_ARGS = 100;
const int SIZE_OF_FILENAME = 30;
using std::cin;
using std::cout;
using std::endl;
using std::ios;
using std::fstream;
void write(employee* e, int numsEmpl){
	for(int i=0; i<numsEmpl; i++){
		cout << e[i].num << " " << e[i].name << " " << e[i].hours << endl;
	}
}

void creatorStart(char* filename, int numsEmployees){
	char*buff=new char[SIZE_OF_BUFF];
	char args[SIZE_OF_ARGS]="Creator.exe";
	strcat(args, " ");
	strcat(args, itoa(numsEmployees,buff,SIZE_OF_BUFF)); 
	strcat(args, " ");
	strcat(args, filename);
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb=sizeof(STARTUPINFO);
	CreateProcess(NULL, args, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
}
void reporterStart(char binFile[], char txtFile[], double salary){
	char args[SIZE_OF_ARGS]="Reporter.exe ";
	char*buff=new char[SIZE_OF_BUFF];
	strcat(args, binFile);
	strcat(args," ");
	strcat(args, txtFile);
	strcat(args," ");
	strcat(args, gcvt(salary, NDIGIT, buff));
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb=sizeof(STARTUPINFO);
	CreateProcess(NULL, args, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
}

int main(int argc, char*argv[]){
	char binFile[SIZE_OF_FILENAME];
	char txtFile[SIZE_OF_FILENAME];
	int numsEmpl;
	int salary;
	cout<<"Enter binFile name: ";
	cin>>binFile;
	cout << "Enter number of employees: ";
	cin >> numsEmpl;
	cout << endl;
	creatorStart(binFile, numsEmpl);
	employee* e=new employee[numsEmpl];
	fstream fin(binFile,ios::in|ios::binary|ios::ate);
	fin.seekg(0, ios::end);
	int n=fin.tellg()/sizeof(employee);
	fin.seekg(0, ios::beg);
	fin.read((char*)e, sizeof(employee)* n);
	write(e, numsEmpl);
	fin.close();
	cout<<"Creator stops working"<<endl<<endl;
	cout<<"Enter report filename: ";
	cin>>txtFile;
	cout<<"Enter salary: ";
	cin>>salary;
	cout<<endl;
	reporterStart(binFile, txtFile, salary);
	fstream txtfin(txtFile);
	string str;
	while(getline(txtfin, str)){
		cout<< str <<endl;
	}
	delete[] e;
	system("pause");
	return 0;
}
