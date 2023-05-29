#include <iostream>
#include <windows.h>
#include "employee.h"
using std::cout;
using std::cin;
using std::endl;

void fileWrite(DWORD& writeByte, bool& isSend, HANDLE hPipe, char cmd[10]){
    isSend = WriteFile(hPipe,cmd,10,&writeByte, NULL);
    if(!isSend){cout<<"Message can't be send"<<endl; return;}
}
void sending(bool& isSend, employee& e, DWORD& writeBytes, HANDLE hPipe){
    isSend=WriteFile(hPipe,&e,sizeof(e), &writeBytes,NULL);
    if(isSend) cout<<"New record sending"<<endl;
    else{
        cout<<"error"<<endl;
        return;
    }
}
int main(int argc, char*argv[]) {
    HANDLE EvReady = OpenEventA(EVENT_MODIFY_STATE,FALSE, argv[1]);
    HANDLE EvStart = OpenEventA(SYNCHRONIZE,FALSE,"START_ALL");
    SetEvent(EvReady);
    WaitForSingleObject(EvStart,INFINITE);
    HANDLE hPipe;
    while(true){
        hPipe=CreateFile( "\\\\.\\pipe\\pipe_name", GENERIC_WRITE|GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0, NULL);
        if(hPipe!=INVALID_HANDLE_VALUE) break;
    }
    while (true){
        char cmd[10];
        cout<<"Enter 1 to read, 2 to write ID of employee, or 3 to exit"<<endl;
        cin>>cmd;
        if(cmd[0]=='3') break;
        DWORD writeByte;
        bool isSend;
        fileWrite(writeByte,isSend,hPipe,cmd);
        employee emp;
        if(emp.id<0){
            cout<<"Employee not found"<<endl;
            continue;
        }
        else{
            print(emp);
            if('2'==cmd[0]){
                cout<<"Input ID, name and hours"<<endl;
                read(emp);
                sending(isSend,emp,writeByte,hPipe);
            }
        }
    }
    return 0;
}