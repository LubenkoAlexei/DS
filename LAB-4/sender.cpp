#include <iostream>
#include <fstream>
#include <Windows.h>
#include <string>
#include <cstdlib>
const CHAR* nameEvent="StartSenderEvent";
CRITICAL_SECTION criticalSection;

int main(int args, char* argv[]){
	InitializeCriticalSection(&criticalSection);
	if(args==2){
		HANDLE Event = OpenEventA(EVENT_MODIFY_STATE, FALSE, argv[0]);
        HANDLE startEvent = OpenEventA(SYNCHRONIZE, FALSE, nameEvent);
		std::ofstream fout(argv[1], std::ios_base::binary | std::ios_base::out | std::ios_base::app);
		std::string message;
		SignalObjectAndWait(Event, startEvent, INFINITE, FALSE);
		while(true){
			std::cout<<"Input message for send or 0 to stop"<<std::endl;
			std::cin>>message;
			if(message=="0") break;
			EnterCriticalSection(&criticalSection);
			fout<<message<<std::endl;
			SetEvent(Event);
			LeaveCriticalSection(&criticalSection);
		}
		CloseHandle(startEvent);
		fout.close();
		CloseHandle(Event);

	}
	return 0;
}