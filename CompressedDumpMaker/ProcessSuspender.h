#pragma once
typedef LONG(NTAPI *NtSuspendResumeProcess)(IN HANDLE ProcessHandle);

class ProcessSuspender
{
public:
	ProcessSuspender(HANDLE hTarget):
		_hProc(hTarget),
		_suspended(false),

		_ntSuspendProcRoutine(nullptr),
		_ntResumeProcRoutine(nullptr)
	{}

	ProcessSuspender(ProcessSuspender&) = delete;
	ProcessSuspender& operator=(ProcessSuspender&) = delete;

	bool initialize();

	~ProcessSuspender();

private:
	HANDLE _hProc;
	bool _suspended;

	NtSuspendResumeProcess _ntSuspendProcRoutine;
	NtSuspendResumeProcess _ntResumeProcRoutine;
};

