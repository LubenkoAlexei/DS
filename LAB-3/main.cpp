#include <iostream>
#include <windows.h>
#include <vector>

using std::cout;
using std::cin;
using std::endl;
using std::vector;

CRITICAL_SECTION criticalSection;
struct thread{
    int*array;
    int arraySize;
    int indexOfThread;
    HANDLE  stopWork;
    HANDLE startWork;
    HANDLE  *ev;
    thread(int*a, int s, int ind){
        array=a;
        arraySize=s;
        indexOfThread=ind;

    }

};
void print(int*arr, int size){
    cout<<"Array: ";
    for(int i =0;i<size;i++){
        cout<<arr[i]<<" ";
    }
    cout<<endl;
}
DWORD WINAPI foo(LPVOID params){
    thread info= *static_cast<thread*>(params);
    bool endTh=false;
    int numOfmarked=0;
    srand(info.indexOfThread);
    WaitForSingleObject(info.startWork,INFINITE);
    while(!endTh){
        int index=rand()%info.arraySize;
        EnterCriticalSection(&criticalSection);
        if(info.array[index]==0){
            Sleep(5);
            info.array[index]=info.indexOfThread;
            LeaveCriticalSection(&criticalSection);
            numOfmarked++;
            Sleep(5);
        }
        else{
            cout<<endl <<"Thread # "<<info.indexOfThread
                <<", number of marked elems: "<<numOfmarked
                <<", can't mark elems with index "<<index;
            LeaveCriticalSection(&criticalSection);
            SetEvent(info.stopWork);
            int k= WaitForMultipleObjects(2,info.ev,FALSE,INFINITE)-WAIT_OBJECT_0;
            if(k==0) {endTh=true;}
        }
    }
    for(int i=0;i<info.arraySize;i++){
        if(info.array[i]==info.indexOfThread){
            info.array[i]=0;
        }
    }
    return 0;
}
int main(){
    InitializeCriticalSection(&criticalSection);
    cout<<"Enter size of array: ";
    int size; cin>>size;
    int*arr=new int[size];
    for(int i =0;i<size;i++){
        arr[i]=0;
    }
    cout<<"Enter number threads marker: ";
    int numOfThreads; cin>>numOfThreads;
    HANDLE* threads=new HANDLE[numOfThreads];
    thread* information=new thread[numOfThreads];
    bool* termTh=new bool[numOfThreads];
    HANDLE start= CreateEvent(NULL,TRUE,FALSE,NULL);
    HANDLE*stoppedThreads=new HANDLE[numOfThreads];
    for(int i =0;i<numOfThreads;i++){
        information[i].array=arr;
        information[i].arraySize=size;
        information[i].indexOfThread=i+1;
        information[i].startWork=start;
        stoppedThreads[i]=information[i].stopWork= CreateEvent(NULL,TRUE, FALSE, NULL);
        information[i].ev=new HANDLE[2];
        information[i].ev[0] = CreateEvent(NULL,FALSE,FALSE, NULL);
        information[i].ev[1] = CreateEvent(NULL,FALSE,FALSE, NULL);
        threads[i]= CreateThread(NULL,0,foo,(LPVOID)(&information[i]),NULL,NULL);
        termTh[i]=false;
    }
    SetEvent(start);
    int endThs=0;
    while(endThs!=numOfThreads){
        WaitForMultipleObjects(numOfThreads,stoppedThreads,TRUE, INFINITE);
        cout<<endl;
        print(arr,size);
        bool isThTerm=false;
        while(!isThTerm){
            int indThToTerm;
            cout<<"Enter index of thread to be terminated: "; cin>>indThToTerm;
            indThToTerm--;
            if(indThToTerm >= numOfThreads || indThToTerm<0){
                cout<<"No such thread!"<<endl;
                continue;
            }
            if(termTh[indThToTerm]){
                cout<<"Thread is already terminated"<<endl;
            }
            else{
                SetEvent(information[indThToTerm].ev[0]);
                WaitForSingleObject(threads[indThToTerm],INFINITE);
                print(arr,size);
                termTh[indThToTerm]=true;
                isThTerm=true;
                endThs++;
            }
        }
        for(int i =0;i<numOfThreads;i++){
            if(!termTh[i]){
                ResetEvent(information[i].stopWork);
                SetEvent(information[i].ev[1]);
            }
        }
    }
    cout<<"All threads are terminated"<<endl;
    DeleteCriticalSection(&criticalSection);
    return 0;
}


