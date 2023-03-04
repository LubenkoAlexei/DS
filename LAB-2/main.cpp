#include <iostream>
#include <windows.h>
#include <vector>

using std::cin;
using std::cout;
using std::endl;

const int TIME_FOR_SLEEP_FIRST = 7;
const int TIME_FOR_SLEEP_SECOND = 12;

struct forThreads{
    std::vector<int>arr;
    double average;
    int min;
    int max;
    int indexMin;
    int indexMax;

};
void print(vector<int>& arr){
    for(e: arr)
        cout<< e <<" ";
}
DWORD WINAPI MinMax(LPVOID param){
    forThreads*minMax=static_cast<forThreads*>(param);
    std::vector<int>mas=minMax->arr;
    int min=mas[0];
    int imin=0;
    int imax=0;
    int max=mas[0];
    for(int i=1; i<mas.size(); i++){
        if(mas[i]>max){
            Sleep(TIME_FOR_SLEEP_FIRST);
            max=mas[i];
            imax=i;

        }
        if(mas[i]<min){
            Sleep(TIME_FOR_SLEEP_FIRST);
            min=mas[i];
            imin=i;
        }
    }
    minMax->max=max;
    minMax->min=min;
    minMax->indexMax=imax;
    minMax->indexMin=imin;
    cout<<"Minimum is: "<<minMax->min<<endl;
    cout<<"Maximum is: "<<minMax->max<<endl;
    cout<<"Thread is finished"<<endl;
    return 0;
}

DWORD WINAPI average(LPVOID param){
    forThreads* avArr=static_cast<forThreads*>(param);
    std::vector<int>mas=avArr->arr;
    double sum = 0.;
    for(int i = 0; i < mas.size(); i++){
        sum+=mas[i];
        Sleep(TIME_FOR_SLEEP_SECOND);
    }
    avArr->average=static_cast<double>(sum/mas.size());
    cout<< "average is: "<< avArr->average<< endl;
    cout<<"Thread is finished"<<endl;
    return 0;
}

int main() {
    cout<<"Input size of array: ";
    int size;
    cin>>size;
    std::vector<int>array;
    int elem;
    cout<<"Input elements of array: ";
    for(int i=0;i<size;i++){
        cin >> elem;
        array.push_back(elem);
    }
    forThreads* forThread=new forThreads();
    forThread->arr=array;
    HANDLE ThreadAverage= CreateThread(NULL,0,average,forThread,NULL,NULL);
    HANDLE ThreadMinMax= CreateThread(NULL,0,MinMax,forThread,NULL,NULL);
    if(ThreadAverage== NULL || ThreadMinMax == NULL){
        return GetLastError();
    }
    WaitForSingleObject(ThreadAverage, INFINITE);
    WaitForSingleObject(ThreadMinMax, INFINITE);
    CloseHandle(ThreadAverage);
    CloseHandle(ThreadMinMax);
    array[forThread->indexMax]=array[forThread->indexMin]=static_cast<int>(forThread->average);
    cout<<"New array: ";
    print(array);
    delete[]forThread;
    return 0;
}
