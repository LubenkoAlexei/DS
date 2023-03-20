#include <iostream>
#include <windows.h>
#include<vector>
#include<thread>
#include <mutex>
#include <condition_variable>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

int arrSize;
vector<bool> statementArray;
int countOfThreads;
int cot;
vector<int>arr;
std::mutex mt;
std::condition_variable cond;
int numberToEnd=-1;

bool consistM(const vector<int>& vec, int a){
    for (int i: vec) if (i == a) return true;
    return false;
}
bool ifAllStop(){
    for (int i = 0; i < cot; i++)if (statementArray[i]) return false;
    return true;
}
void funcForThread(int x){
    srand(x);
    vector<int> m;
    while(true){
        std::unique_lock<std::mutex> ul(mt);
        if(numberToEnd==x){
            statementArray[x]=false;
            for(int i : m){
                arr[i]=0;
            }
            countOfThreads--;
            cond.notify_all();
            ul.unlock();
            return;
        }
        int ch=rand()%arrSize;
        if(arr[ch]==0){
            Sleep(5);
            arr[ch]=x+1;
            if(!consistM(m,ch)){
                m.push_back(ch);
            }
            Sleep(5);
        }
        else{
            statementArray[x]=false;
            cout<<"thread #"<<x<<endl;
            for(int i:m){
                cout<<i<<" ";
            }
            cout<<endl<<arrSize<<endl;
            cond.notify_all();
            cond.wait(ul,[=](){return statementArray[x];});
        }
        ul.unlock();
    }
}
int main() {
    cout<<"Input size of array: ";
    cin>>arrSize;
    arr.resize(arrSize);
    fill(arr.begin(),arr.end(),0);
    cout<<"Input nums of maker:";
    cin>>countOfThreads;cot=countOfThreads;
    vector<std::thread>Threads;
    for(int i=0;i<countOfThreads;i++){
        Threads.emplace_back(&funcForThread,i);
        statementArray.push_back(true);
    }
    for(int i=0;i<countOfThreads;i++){
        Threads[i].detach();
    }
    vector<int>endedThreads;
    while(countOfThreads>0){
        std::unique_lock<std::mutex> ul(mt);
        cond.wait(ul,[=](){return ifAllStop();});
        cout<<"array: ";
        for(int i : arr){
            cout<< i <<" ";
        }
        cout<<endl<<"Number of thread for stop: "<<endl;
        int n; cin>>n;
        if(n>=Threads.size()) n=0;
        endedThreads.push_back(n);
        statementArray[n]=true;
        numberToEnd=n;
        cond.notify_all();
        cond.wait(ul,[=](){return ifAllStop();});
        for(int i=0;i<countOfThreads;i++){
            if(!consistM(endedThreads,i)){
                statementArray[i]=true;
            }
        }
        cond.notify_all();
        ul.unlock();
    }
    return 0;
}
