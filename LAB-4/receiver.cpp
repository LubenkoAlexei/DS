#include <Windows.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <fstream>
CRITICAL_SECTION criticalSection;
int main(){
	std::cout<<"Input filename"<<std::endl;
	char filename[100];
	std::cin>>filename;
	std::cout<<"Input number of sender processes"<<std::endl;
	int countOfSender;
	std::cin>>countOfSender;
	InitializeCriticalSection(&criticalSection);
	STARTUPINFOA*sendersSI = new STARTUPINFO[countOfSender];
	PROCESS_INFORMATION* sendersPI = new PROCESS_INFORMATION[countOfSender];
    HANDLE* SenderEventsToSync = new HANDLE[countOfSender];
    HANDLE* senders = new HANDLE[countOfSender];
	std::fstream fin(filename,std::ios_base::trunc | std::ios_base::out | std::ios_base::binary | std::ios_base::in);
	const char* appName="E:\\OperatingSystems\\LAB-4\\Debug\\sender.exe";
	SECURITY_ATTRIBUTES securityAtb={sizeof(SECURITY_ATTRIBUTES),0, TRUE};
	HANDLE eventToStart=CreateEventA(&securityAtb, FALSE,FALSE,"SenderStartEvent");
	for(int i = 0;i < countOfSender; i++){
		char arg[200];
		char*buff=new char[10];
		strcat(arg,filename);
		ZeroMemory(&sendersSI[i],sizeof(STARTUPINFO));
		sendersSI[i].cb=sizeof(STARTUPINFO);
		ZeroMemory(&sendersPI[i], sizeof(PROCESS_INFORMATION));
		char argg[10];
		strcat(argg,"Event ");
		strcat(argg,itoa(i,buff,10));
		SenderEventsToSync[i]=CreateEventA(&securityAtb,FALSE, FALSE,argg);
		strcat(arg," Event ");
		strcat(arg, itoa(i,buff,10));
		
		if(!CreateProcessA(appName,arg,NULL,NULL,TRUE,CREATE_NEW_CONSOLE,NULL,NULL,&sendersSI[i],&sendersPI[i])){
			std::cout<<GetLastError();
			ExitProcess(0);
		}
		senders[i]=sendersPI[i].hProcess;
	}
	WaitForMultipleObjects(countOfSender, SenderEventsToSync, TRUE, INFINITE);
    SetEvent(eventToStart);

	while(WaitForMultipleObjects(countOfSender,senders,TRUE,0)==WAIT_TIMEOUT){
		std::cout<<"Input 9 for trying to read file";
		std::string text;
		std::cin>>text;
		if(text.compare("9")!=0){
			break;
		}
		std::string message;
		EnterCriticalSection(&criticalSection);
		fin.sync();
		if(!(std::getline(fin,message))){
			std::cout<<"No messages"<<std::endl;
			fin.clear();
		}
		else std::cout<<message<<std::endl;
		while(std::getline(fin,message)){
			std::cout<<message<<std::endl;
		}
		fin.clear();
		LeaveCriticalSection(&criticalSection);
	}
	for(int i = 0; i<countOfSender;i++){
		CloseHandle(sendersPI[i].hThread);
		CloseHandle(sendersPI[i].hProcess);
		CloseHandle(SenderEventsToSync[i]);
	}
	CloseHandle(eventToStart);
	fin.close();
	return 0;
}