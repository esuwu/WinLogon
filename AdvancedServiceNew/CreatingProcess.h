#pragma once

#include <windows.h>
#include <Ntsecapi.h>
#include <Sddl.h>
#include <tchar.h>
#include <stdio.h>
#include <fstream>
#include <string>
//#include <ntstatus.h>
#include <malloc.h>
#include <strsafe.h>

#pragma comment (lib, "Secur32.lib")
#pragma comment (lib, "strsafe.lib")

#define STATUS_SUCCESS ((NTSTATUS)0x00000000L)

DWORD GetCurrentSessionId();
bool LaunchProcess(const char* process_path);
