#include "stdafx.h"
#include "ProcessSuspender.h"


bool ProcessSuspender::initialize()
{
	HMODULE hNtDll = LoadLibrary(L"ntdll.dll");
	if (hNtDll == nullptr)
	{
		return false;
	}

	void* procAddr = GetProcAddress(hNtDll, "NtSuspendProcess");
	if (procAddr == nullptr)
	{
		return false;
	}

	this->_ntSuspendProcRoutine = static_cast<NtSuspendResumeProcess>(procAddr);

	procAddr = GetProcAddress(hNtDll, "NtResumeProcess");
	if (nullptr == procAddr)
	{
		return false;
	}

	this->_ntResumeProcRoutine = static_cast<NtSuspendResumeProcess>(procAddr);

	this->_suspended = this->_ntSuspendProcRoutine(this->_hProc) == S_OK;

	return this->_suspended;
}

ProcessSuspender::~ProcessSuspender()
{
	if (this->_suspended)
	{
		this->_ntResumeProcRoutine(this->_hProc);
	}
}
