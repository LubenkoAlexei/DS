#include <iostream>
#include <conio.h>
#include <algorithm>
#include <process.h>
#include <windows.h>
#include "employee.h"
#include "strings.h"
using std::cout;
using std::cin;
using std::endl;
const int sizeOfPipeName = 30;
const int sizeOfBuff = 10;
const int wrongId = -1;
const int sizeOfFilename = 50;
const int clientCount = 3;
int empCount;
employee* emps;
HANDLE* hReadyEvents;
CRITICAL_SECTION empsCS;
bool *empIsModifying;
const char pipeName[sizeOfPipeName] = "\\\\.\\pipe\\pipe_name";

HANDLE createNamedPipe(const char pipeName[]){
    return CreateNamedPipe(pipeName, PIPE_ACCESS_DUPLEX,
                           PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
                           PIPE_UNLIMITED_INSTANCES,0, 0,INFINITE, NULL);
}

void startProcesses(int count){
    char buff[sizeOfBuff];
    for(int i = 0; i < count; i++) {
        char cmdargs[80] = "..\\..\\Client\\cmake-build-debug\\Client.exe ";
        char eventName[50] = "READY_EVENT_";
        itoa(i + 1, buff, sizeOfBuff);
        strcat(eventName, buff);
        strcat(cmdargs, eventName);
        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(STARTUPINFO));
        si.cb = sizeof(STARTUPINFO);
        hReadyEvents[i] = CreateEvent(NULL, TRUE, FALSE, eventName);
        if (!CreateProcess(NULL, cmdargs, NULL, NULL, FALSE, CREATE_NEW_CONSOLE,
                           NULL, NULL, &si, &pi)){
            GetLastError();
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
    }
}

void writeFile (HANDLE hPipe, employee* empToSend, DWORD& bytesWritten){
    bool isSent = WriteFile(hPipe, empToSend, sizeof(employee), &bytesWritten, NULL);
    if(isSent) cout << sendAns << endl;
    else cout << sendError << endl;
}
void readFile(HANDLE& hPipe, char*message, bool& isRead){
    DWORD readBytes;
    isRead = ReadFile(hPipe, message, sizeOfBuff, &readBytes, NULL);
    if(!isRead) {
        if (ERROR_BROKEN_PIPE == GetLastError()) {
            cout << CLdisc << endl;
            return;
        } else {
            cout << readError << endl;
            return;
        }
    }
}
void check(bool& isRead, employee* empToSend){
    if(isRead){
        cout << changeRec << endl;
        empIsModifying[empToSend - emps] = false;
        EnterCriticalSection(&empsCS);
        sortEmps(emps, empCount);
        LeaveCriticalSection(&empsCS);
    }
    else cout << readError << endl;
}

void requestions(int command, employee* empToSend, int id){
    int ind = empToSend - emps;
    switch (command) {
        case '2':
            cout << modifyReq << id << endl;
            empIsModifying[ind] = true;
            break;
        case '1':
            cout << readReq<< id<<endl;
            break;
        default:
            cout << reqError;
    }
}

DWORD WINAPI messaging(LPVOID p){
    HANDLE hPipe = (HANDLE) p;
    employee* errorEmp = new employee(wrongId);
    while(true) {
        bool isRead = false;
        char message[sizeOfBuff];
        readFile(hPipe, message, isRead);
        if(strlen(message) > 0) {
            char command = message[0];
            message[0] = ' ';
            int id = atoi(message);
            DWORD bytesWritten;
            EnterCriticalSection(&empsCS);
            employee* empToSend = findEmp(id, emps, empCount);
            LeaveCriticalSection(&empsCS);
            if(NULL == empToSend) empToSend = errorEmp;
            else requestions(command,empToSend,id);
            writeFile(hPipe, empToSend,bytesWritten);
            if('2' == command && empToSend != errorEmp){
                readFile(hPipe,message, isRead);
                check(isRead, empToSend);
                break;
            }
        }
    }
    FlushFileBuffers(hPipe);
    DisconnectNamedPipe(hPipe);
    CloseHandle(hPipe);
    delete errorEmp;
    return 0;
}

int main() {
    char filename[sizeOfFilename];
    cout << inputString;
    cin >> filename >> empCount;
    emps = new employee[empCount];
    readData(emps, empCount);
    writeData(filename,emps, empCount);
    sortEmps(emps, empCount);
    InitializeCriticalSection(&empsCS);
    HANDLE hstartALL = CreateEvent(NULL, TRUE, FALSE, "START_ALL");
    empIsModifying = new bool[empCount];
    for(int i = 0; i < empCount; ++i)
        empIsModifying[i] = false;
    hReadyEvents = new HANDLE[clientCount];
    startProcesses(clientCount);
    WaitForMultipleObjects(clientCount, hReadyEvents, TRUE, INFINITE);
    cout << Start << endl;
    SetEvent(hstartALL);
    HANDLE hPipe;
    HANDLE* hThreads = new HANDLE[clientCount];
    for(int i = 0; i < clientCount; i++){
        hPipe = createNamedPipe(pipeName);
        if (INVALID_HANDLE_VALUE == hPipe){
            cout << pipeError << endl;
            break;
        }
        hThreads[i] = CreateThread(NULL, 0, messaging, (LPVOID)hPipe,0,NULL);
    }
    cout << CLconnect << endl;
    WaitForMultipleObjects(clientCount, hThreads, TRUE, INFINITE);
    printEmployees(emps, empCount);
    cout << Exit << endl;
    getch();
    DeleteCriticalSection(&empsCS);
    delete[] hThreads;
    delete[] hReadyEvents;
    delete[] empIsModifying;
    delete[] emps;
    return 0;
}