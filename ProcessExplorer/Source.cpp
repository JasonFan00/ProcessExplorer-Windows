
//  Write up:  This is my first project in C++, .NET, and the windows API.  I motivate myself to learn new languages through projects.  This program works like task manager, it will scan all the running processes on your computer and display them neatly with some information, such as #threads and modules, etc.  There's nothing that my program does that windows tasks manager and process explorer don't already do, but making this was a fun learning experience.  I did most of my research on MSDN's documentation of the windows API and .NET programming.  Most of my takeaways from this project will just be the usage of C++ and .NET in general, but I also did learn a lot about the internals of operating systems, such as how tasks are scheduled, the stack, how a process's memory is split into up/structured by the OS, and lots of new terminology!

#include "MyForm.h"

using namespace System;
using namespace System::Windows::Forms;

#include <iostream>
#include <windows.h>
#include <Tlhelp32.h> //  For system snapshot functions
#include <Psapi.h>    //  Process enumeration functions, process status API
#include <vector>
#include <string>
#include <stdio.h> 
#include <Winternl.h>
#include <tuple>
#include <sstream>


namespace proc
{
	static LPVOID memAddress;
	static LPCVOID overWriteVal;
	static HANDLE hProcessOpen;
	static DWORD processPriority;  //  A priority level of execution that determines which thread is worked on next by the CPU
	DWORD pIDs[1024];
	static DWORD processesRead;
	static std::vector<char*> exeNames;
};

//  My ghetto way of handling errors.  I'll learn a better way in the future.
namespace errorMsges
{
	#define ARCHITECTUREerr 299  // defines not bound by namespaces
	#define modFailure -2
	#define hError 144;
	std::string failureMsg = "ERROR";
	std::string aDenied = "Access Denied";
	std::string nFailure = "nError";
	std::string te32Failure = "Cannot Retrieve Threads";
	std::string archiFailure = "Compatibility Error";
	std::string pathHFailure = "pHandle failed c:"; //  typically b/c pID has been identified but process no longer exists by time of handle, or may be processes owned by different users
	std::string modFailureMsg = "Module Search failed";
	std::string getMemFailure = "Unable to retrieve mem usage";
}

class winProc
{
	
public:
	MODULEENTRY32 modEntry;
	MODULEINFO modInfo;
	THREADENTRY32 threadEntry;
	PROCESSENTRY32 procEntry; 
	PROCESS_MEMORY_COUNTERS memCounter;
	std::string path;
	std::string exeName;
	std::string getName(std::string path);
	int totalMods;
	int totalThreads;
	LONG pTBasePriority;
	DWORD pID;
};

std::string winProc::getName(std::string path)
{
	std::string exeName;

	//  Get the substring, put it into exeName string.  There are better ways to do it with some winapi functions but I thought this would be a nice little exercise.
	if (path.find("\\") != std::string::npos)
	{
		std::size_t posBeg = path.find_last_of("\\") + 1;
		std::size_t space = path.find_last_of(".exe") - posBeg + 1;
		std::string exeName = path.substr(posBeg, space); 
		return exeName;
	}
	else if (path.compare(errorMsges::aDenied) == 0)
	{
		return errorMsges::aDenied;
	}
	return path;
}

DWORD enum_procs(void);

std::string display_paths(DWORD pID); 
void log_mods(MODULEENTRY32 entry, MODULEINFO info);
void PEinit(DWORD processesRead, System::Windows::Forms::ListView^ lObj);
std::tuple<MODULEENTRY32, MODULEINFO, int> mod_search(DWORD pID, const wchar_t* exeName);
std::tuple<PROCESS_MEMORY_COUNTERS, bool> getMemUsage(DWORD pID);

int getThreadCount(DWORD pID);
std::string wcharTostring(const wchar_t* str);
std::string addrPtrToStr(BYTE* ptr);

[STAThread]
void Main(array<String^>^ args)
{
	//--
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	PEf::MyForm form;
	Application::Run(%form);
	//--
}



void PEinit(DWORD processesRead, System::Windows::Forms::ListView^ lObj)
{
	for (int i = 0; i < sizeof(proc::pIDs); i++)
	{
		if (proc::pIDs[i] != 0)
		{
			winProc procHolder;
			procHolder.path = display_paths(proc::pIDs[i]);

			procHolder.exeName = procHolder.getName(procHolder.path);
			std::wstring nameWString = std::wstring(procHolder.exeName.begin(), procHolder.exeName.end());
			const wchar_t* wExeName = nameWString.c_str();
			std::tie(procHolder.modEntry, procHolder.modInfo, procHolder.totalMods) = mod_search(proc::pIDs[i], (const wchar_t*)wExeName);
			bool getMemValid;
			std::tie(procHolder.memCounter, getMemValid) = getMemUsage(proc::pIDs[i]);
			procHolder.totalThreads = getThreadCount(proc::pIDs[i]);
			

			String^ procNameSS = gcnew String((procHolder.exeName).c_str());  //  Convert to System strings...close now, seems to turn it all into whitespace.  Issue with encoding maybe.  Re-compiled and ran fine?
			String^ procPathSS = gcnew String((procHolder.path).c_str());
			String^ pIDSS = gcnew String((std::to_string(proc::pIDs[i]).c_str()));
			String^ numMods;
			if (procHolder.totalMods != modFailure)
			{
				numMods = gcnew String((std::to_string(procHolder.totalMods)).c_str());
			}
			else
			{
				numMods = gcnew String((errorMsges::modFailureMsg).c_str());
			}
			String^ baseAddrS = gcnew String(("0x" + addrPtrToStr(procHolder.modEntry.modBaseAddr)).c_str());
			String^ numThreads;
			if (procHolder.procEntry.cntThreads != 0)
			{
				numThreads = gcnew String((std::to_string(procHolder.totalThreads).c_str()));
			}
			else
			{
				numThreads = gcnew String((errorMsges::te32Failure).c_str());
			}
			String^ memUsageSS;
			if (getMemValid)
			{
				memUsageSS = gcnew String((std::to_string(procHolder.memCounter.PagefileUsage).c_str()));
			}
			else
			{
				memUsageSS = gcnew String(((errorMsges::getMemFailure).c_str()));
			}
		
			ListViewItem^ listViewItem = gcnew Windows::Forms::ListViewItem(procNameSS); //  See object browser for more clarity on valid arg types.  ListViewItem class in namespace Windows, sysName passed to its constructor.  Got tripped up here.
			listViewItem->SubItems->Add(pIDSS); // -> is same as dereferencing a class pointer then using . operator.  In this case can use . operator (I believe) anyways instead of ->.  Just going with the generated code.
			listViewItem->SubItems->Add(numMods);
			listViewItem->SubItems->Add(baseAddrS);
			listViewItem->SubItems->Add(numThreads);
			listViewItem->SubItems->Add(memUsageSS);
			listViewItem->SubItems->Add(procPathSS);

			lObj->Items->Add(listViewItem); // need to figure out how to add the created items to proclistview now...need to be able to access that object.  Is part of namespace then class, new issues now.  Fixed a error misleading me.  Get obj in clicked event, pass it here.  Also made a static reference in .h, can use that alternatively.
		}
	}
	lObj->Text = L"Scanned";
	lObj->Refresh();
}

DWORD enum_procs(void)
{
	DWORD bytesRead;
	int get_processes = EnumProcesses(proc::pIDs, sizeof(proc::pIDs), &bytesRead);
	if (!get_processes)
	{
		std::cout << "pID retriever failed, error code: " << GetLastError() << std::endl;
		return 1;
	}
	if (bytesRead == sizeof(proc::pIDs))
	{
		std::cout << "Number of bytes returned possibly exceeds storage array" << std::endl;
	}
	return bytesRead / sizeof(DWORD);
}

void log_mods(MODULEENTRY32 entry, MODULEINFO info)
{
	std::cout << "Success getting info from module size: " << entry.modBaseSize << std::endl;
	std::cout << "Module has load address: " << info.lpBaseOfDll << " Size (bytes): " << info.SizeOfImage << " Entrypoint: " << info.EntryPoint << std::endl;
}

std::string display_paths(DWORD pID)
{
	std::string pathCopy, exeName;
	//  std::size_t posBeg;
	HANDLE currentH;
	char tempPath[MAX_PATH];
	if (pID != 0)
	{
		//  Remember to close handles!
		currentH = OpenProcess(PROCESS_QUERY_INFORMATION, false, pID);
		if (currentH == NULL)
		{				
			if (GetLastError() == 5)
			{
				std::cout << "Access Denied to pID: " << pID << std::endl;
				return &errorMsges::aDenied[0u];
			}
			else
			{
				return (errorMsges::pathHFailure + std::to_string(GetLastError()));  //  typically error 87, most likely trying to access 64-bit processes
			}
		}
		else
		{
			if (GetProcessImageFileNameA(currentH, tempPath, sizeof(tempPath)) == 0)
			{
				std::cout << "Error retrieving exe file path, code: " << GetLastError() << std::endl;
				return (&errorMsges::failureMsg[0u]);
			}
			CloseHandle(currentH);
			std::string pathStr(tempPath);  //  keeping as char seems to cut off part of the path when assigning to .path in call, tried debugging, doing this now
			return pathStr;
		}
	}
	return (&errorMsges::failureMsg[0u]); // convert it to definition type
}


int getThreadCount(DWORD pID)
{
	THREADENTRY32 threadStruc;
	threadStruc.dwSize = sizeof(THREADENTRY32);
	HANDLE currentSnapH = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, NULL);
	int tCount = 0;
	if (currentSnapH != INVALID_HANDLE_VALUE && Thread32First(currentSnapH, &threadStruc))
	{
		do
		{
			if (threadStruc.th32OwnerProcessID == pID)
			{
				tCount++;
			}
		} while (Thread32Next(currentSnapH, &threadStruc));
	}
	return tCount;
	//  Note:  Implement proper error handling
}

std::tuple<PROCESS_MEMORY_COUNTERS, bool> getMemUsage(DWORD pID)
{
	PROCESS_MEMORY_COUNTERS memCounter;
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, false, pID);
	bool isValid = false;
	if (hProcess != NULL)
	{
		if (GetProcessMemoryInfo(hProcess, &memCounter, sizeof(memCounter)) > 0)  //  fill struct
		{
			isValid = true;  // success
			return std::make_tuple(memCounter, isValid);
		}
	}
	else
	{
		DWORD err = std::getchar(); // debug
		return std::make_tuple(memCounter, isValid);  // isValid false, implement better error handling
	}
}

std::tuple<MODULEENTRY32, MODULEINFO, int> mod_search(DWORD pID, const wchar_t* exeName)
{
	MODULEENTRY32 entryStruc; 
	MODULEINFO infoStruc;
	entryStruc.dwSize = sizeof(MODULEENTRY32);
	entryStruc.modBaseAddr = NULL;

	//  Get handles and snapshots
	HANDLE currentH = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, pID); 
	HANDLE currentSnapH64 = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID); 
	HANDLE currentSnapH32 = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE32, pID);

	int totalMods = 0;
	if ((currentSnapH64 != INVALID_HANDLE_VALUE && currentH != NULL && Module32First(currentSnapH64, &entryStruc)) || (currentSnapH32 != INVALID_HANDLE_VALUE && currentH != NULL && Module32First(currentSnapH32, &entryStruc)))
	{
		//  Get module handle from entry struct.
		do
		{
			HMODULE modH = entryStruc.hModule;  // update handle for specific module, Read-only val, does not need to be cleaned up (CloseHandle doesn't accept HMODULE anyways)
			if (modH && GetModuleInformation(currentH, modH, &infoStruc, sizeof(MODULEINFO)))
			{
				totalMods++;
			}
		} while (Module32Next(currentSnapH64, &entryStruc)); //  Go to next module in snapshot
	}
	else if((Module32First(currentSnapH64, &entryStruc) == false) || (Module32First(currentSnapH32, &entryStruc) == false))
	{ //  Most likely due to trying to access 64 bit process, error 299 snapshot partial copy
		CloseHandle(currentH);
		if (currentSnapH64 != INVALID_HANDLE_VALUE)
		{
			CloseHandle(currentSnapH64);
		}
		if (currentSnapH32 != INVALID_HANDLE_VALUE)
		{
			CloseHandle(currentSnapH32);
		}
		DWORD err = GetLastError(); // debug purposes
		return std::make_tuple(entryStruc, infoStruc, modFailure);
	}
	//  In case of handles failing
	DWORD err = GetLastError(); // debug purposes
	return std::make_tuple(entryStruc, infoStruc, totalMods);
}


//  For print debugging purposes, convert unicode to the typical.  UTF-16 I think.
std::string wcharTostring(const wchar_t* str)
{
	std::string mystring;
	while (*str)
		mystring += (char)*str++;
	return  mystring;
}

std::string addrPtrToStr(BYTE* ptr)
{
	std::ostringstream strmO; // create stream class for its str buffer
	strmO << (void*)ptr; //  stream address location pointed at casted to void* (bc BYTE* is a win data type)
	std::string addrS(strmO.str()); //  get contents streamed into strmO, save to string
	return addrS;
}

//  Button click event handler
System::Void PEf::MyForm::ScanButton_Click(System::Object ^ sender, System::EventArgs ^ e)
{
	this->ScanButton->Enabled = false;
	this->ScanButton->Text = L"Scanning...";
	this->ScanButton->Refresh();
	DWORD processesRead = enum_procs();
	if (processesRead > 1)
	{
		PEinit(processesRead, this->ProcListView);
	}
	
	return System::Void();
}
