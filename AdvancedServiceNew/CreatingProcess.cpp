#include "CreatingProcess.h"
#include <iostream>
#include <wtsapi32.h>
#include <userenv.h>

#pragma comment(lib, "Wtsapi32.lib")
#pragma comment(lib, "Userenv.lib")
using namespace std;

DWORD GetCurrentSessionId()
{
	WTS_SESSION_INFO* pSessionInfo;
	DWORD n_sessions = 0;
	BOOL ok = WTSEnumerateSessions(WTS_CURRENT_SERVER, 0, 1, &pSessionInfo, &n_sessions);
	if (!ok)
		return 0;

	DWORD SessionId = 0;

	for (DWORD i = 0; i < n_sessions; ++i)
	{
		if (pSessionInfo[i].State == WTSActive)
		{
			SessionId = pSessionInfo[i].SessionId;
			break;
		}
	}

	WTSFreeMemory(pSessionInfo);
	return SessionId;
}

bool LaunchProcess(const char* process_path)
{
	DWORD SessionId = GetCurrentSessionId();
	if (SessionId == 0)    // no-one logged in
		return false;

	HANDLE hToken;
	BOOL ok = WTSQueryUserToken(SessionId, &hToken);
	if (!ok)
		return false;

	void* environment = NULL;
	ok = CreateEnvironmentBlock(&environment, hToken, TRUE);

	if (!ok)
	{
		CloseHandle(hToken);
		return false;
	}

	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi = { };
	si.lpDesktop = "winsta0\\default";

	// Do NOT want to inherit handles here
	DWORD dwCreationFlags = NORMAL_PRIORITY_CLASS | CREATE_UNICODE_ENVIRONMENT;
	ok = CreateProcessAsUser(hToken, process_path, NULL, NULL, NULL, FALSE,
		dwCreationFlags, environment, NULL, &si, &pi);

	DestroyEnvironmentBlock(environment);
	CloseHandle(hToken);

	if (!ok)
		return false;

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	return true;
}





