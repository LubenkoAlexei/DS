#include <string>
#include <iostream>
#include <fstream>
struct employee{
    std::string name;
    int id;
    int hours;
    employee(int id_):id(id_),name(""),hours(0){};
    employee():id(0),name(""),hours(0){};
};
void print(employee& empl){
    std::cout<<empl.id<<"\t"<<empl.name<<"\t"<<empl.hours<<std::endl;
}
void read(employee&e){
    std::cin>>e.id>>e.name>>e.hours;
}
void readData(employee*& emps, int empCount){
    std::cout << "Enter ID, name and hours of each employee:" <<std::endl;
    for(int i = 0; i < empCount; ++i){
        read(emps[i]);
    }
}
void printEmployees(employee*& emps, int empCount){
    for(int i = 0; i < empCount; i++)
        print(emps[i]);
}

int compare(const void *a, const void *b){
    employee* e1 = (employee*)a;
    employee* e2 = (employee*)b;
    return e1->id-e2->id;
}

void sortEmps(employee*&emps, int empCount){
    qsort(emps, empCount, sizeof(employee), compare);
}
employee* findEmp(int id, employee*&emps, int empCount){
    employee key(id);
    return (employee*)bsearch((const char*)(&key), (const char*)(emps), empCount, sizeof(employee), compare);
}
void writeData(char* filename, employee*& emps, int empCount){
    std::fstream fin(filename, std::ios::binary | std::ios::out);
    fin.write(reinterpret_cast<char*>(emps), sizeof(employee) * empCount);
    fin.close();
}