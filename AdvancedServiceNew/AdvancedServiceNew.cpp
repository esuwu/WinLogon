// AdvancedServiceNew.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>

#include <iostream>
#include <Windows.h>
#include <Winuser.h>
#include <winbase.h>
#include <tchar.h>
#include <sensevts.h>
#include <WtsApi32.h>
#include <SensApi.h>
#include <Sens.h>
#include <EventSys.h>
#include <NTSecAPI.h>
#pragma comment(lib, "Wtsapi32.lib")

SERVICE_STATUS_HANDLE g_ServiceStatusHandle;
HANDLE g_StopEvent;
DWORD g_CurrentState = 0;
bool g_SystemShutdown = false;

void WINAPI ServiceMain(DWORD argc, LPTSTR* argv);
void ReportStatus(DWORD state);
void ReportProgressStatus(DWORD state, DWORD checkPoint, DWORD waitHint);
void ReportErrorStatus(DWORD errorCode);
DWORD WINAPI HandlerEx(DWORD control, DWORD eventType, void* eventData, void* context);

int WriteToLog(const char* str)
{
    FILE* log;

    log = fopen("C:\\Users\\Alexandr\\source\\repos\\AdvancedServiceNew\\Debug\\security_log.txt", "a+");
    if (log == NULL)
        return -1;
    fprintf(log, "%s\n", str);
    fclose(log);
    return 0;
}

int main(int argc, char** argv)
{
    WriteToLog("Main start");
    SERVICE_TABLE_ENTRY serviceTable[] = {
        { TEXT("AdvancedServiceNew"), &ServiceMain },
        { NULL, NULL }
    };

    if (StartServiceCtrlDispatcher(serviceTable))
        return 0;
    else if (GetLastError() == ERROR_FAILED_SERVICE_CONTROLLER_CONNECT)
        return -1; 
    else
        return -2; 
}

void WINAPI ServiceMain(DWORD argc, LPTSTR* argv)
{

    g_ServiceStatusHandle = RegisterServiceCtrlHandlerEx(TEXT("AdvancedServiceNew"), &HandlerEx, NULL);


    ReportStatus(SERVICE_START_PENDING);
    g_StopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    ReportStatus(SERVICE_RUNNING);


    while (WaitForSingleObject(g_StopEvent, 3000) != WAIT_OBJECT_0)
    {
        Beep(1000, 100);
    }

    ReportStatus(SERVICE_STOP_PENDING);

    CloseHandle(g_StopEvent);
    ReportStatus(SERVICE_STOPPED);
}



void ReportStatus(DWORD state)
{
    g_CurrentState = state;
    SERVICE_STATUS serviceStatus = {
        SERVICE_WIN32_OWN_PROCESS,
        g_CurrentState,
        state == SERVICE_START_PENDING ? 0 : SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN,
        NO_ERROR,
        0,
        0,
        0,
    };
    serviceStatus.dwControlsAccepted |= SERVICE_ACCEPT_SESSIONCHANGE;
    SetServiceStatus(g_ServiceStatusHandle, &serviceStatus);
}

void ReportProgressStatus(DWORD state, DWORD checkPoint, DWORD waitHint)
{
    g_CurrentState = state;
    SERVICE_STATUS serviceStatus = {
        SERVICE_WIN32_OWN_PROCESS,
        g_CurrentState,
        state == SERVICE_START_PENDING ? 0 : SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN,
        NO_ERROR,
        0,
        checkPoint,
        waitHint,
    };
    SetServiceStatus(g_ServiceStatusHandle, &serviceStatus);
}

void ReportErrorStatus(DWORD errorCode)
{
    g_CurrentState = SERVICE_STOPPED;
    SERVICE_STATUS serviceStatus = {
        SERVICE_WIN32_OWN_PROCESS,
        g_CurrentState,
        0,
        ERROR_SERVICE_SPECIFIC_ERROR,
        errorCode,
        0,
        0,
    };
    SetServiceStatus(g_ServiceStatusHandle, &serviceStatus);
}

DWORD WINAPI HandlerEx(DWORD control, DWORD eventType, void* eventData, void* context)
{
    switch (control)
    {
        // Entrie system is shutting down.
    case SERVICE_CONTROL_SHUTDOWN:
        g_SystemShutdown = true;

    case SERVICE_CONTROL_STOP:
        ReportStatus(SERVICE_STOP_PENDING);
        SetEvent(g_StopEvent);
        break;
    case (SERVICE_CONTROL_SESSIONCHANGE):
        switch (eventType)
        {
        case(WTS_SESSION_LOGON):
            // Handle logon
            WriteToLog("LOGON WAS");
            break;

        case(WTS_SESSION_LOCK):
            // Handle lock
            WriteToLog("SESSION WAS LOCK");
            break;
        }
        break;
 
    default:
        WriteToLog("DEFAULT");
        ReportStatus(g_CurrentState);
        break;
    }
    return NO_ERROR;
}


// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"
