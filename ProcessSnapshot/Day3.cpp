#include <windows.h>
#include <tlhelp32.h>
#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <iostream>
#include <vector>
#include <tchar.h>
#include <conio.h>
#include <iterator>
#include <algorithm>

using namespace std;

int contor = 0;

struct ProcessInfo {
    DWORD ProcessId;
    TCHAR PathName[10000] = "";
    DWORD ParentId;
};

vector<ProcessInfo> PastSnapshot,CurentSnapshot;

HANDLE hEvent;
HANDLE hThread;

static void VectorSort(vector<ProcessInfo>& Vector)
{
    std::sort(Vector.begin(), Vector.end(), [](const ProcessInfo& a, const ProcessInfo& b) {
        return a.ProcessId < b.ProcessId;
        });

}

static void Snapshot(vector<ProcessInfo>& Processes)
{
    PROCESSENTRY32 PInfo;
    HANDLE ProcessHandle;
    HANDLE SnapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (SnapshotHandle == INVALID_HANDLE_VALUE)
    {
        cout << "SNAPSHOT ERROR : " << GetLastError()<<endl;
        return;
    }
    PInfo.dwSize = sizeof(PROCESSENTRY32);
    auto ProcessStatus = Process32First(SnapshotHandle, &PInfo);
    while (ProcessStatus)
    {
        ProcessInfo MyProcess;
        TCHAR PathName[100];
        DWORD PathNameSize = sizeof(PathName) / sizeof(TCHAR);
        printf("\n\n==================================================== = \n");
        printf("INFORMATION ABOUT THE PROCESS : %s\n", PInfo.szExeFile);
        printf("\n------------------------------------------------------ - ");
        printf("\nPROCESS ID : %d", PInfo.th32ProcessID);
        printf("\nPARENT'S ID : %d", PInfo.th32ParentProcessID);
        MyProcess.ParentId = PInfo.th32ParentProcessID;
        MyProcess.ProcessId = PInfo.th32ProcessID;
        ProcessHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, PInfo.th32ProcessID);
        if (ProcessHandle != NULL)
        {
            _FILETIME Time, Time2, Time3, Time4;
            auto TimeStatus = GetProcessTimes(ProcessHandle, &Time, &Time2, &Time3, &Time4);
            if (TimeStatus != 0)
            {
                SYSTEMTIME SystemTime;
                if (FileTimeToSystemTime(&Time, &SystemTime))
                {
                    printf("\nCREATION TIME : %02d/%02d/%d %02d:%02d:%02d", SystemTime.wDay, SystemTime.wMonth, SystemTime.wYear,
                        SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond);
                }
            }

            auto QueryStatus = QueryFullProcessImageName(ProcessHandle, 0, PathName, &PathNameSize);
            CloseHandle(ProcessHandle);
            if (QueryStatus != 0)
            {
                printf("\nPATH : %s", PathName);
                _tcscpy_s(MyProcess.PathName, PathName);
            }
            else
            {
                _tcscpy_s(MyProcess.PathName, _T(""));
            }
            
            
        }
        cout << endl;
        Processes.push_back(MyProcess);
        ProcessStatus = Process32Next(SnapshotHandle, &PInfo);
    }
    CloseHandle(SnapshotHandle);
    VectorSort(Processes);
}

void ProceseCompare(vector<ProcessInfo> Processes, vector<ProcessInfo> Procese2)
{
    vector<int> New,Old,Changed;
    int i = 0, j = 0;
    while (i < Processes.size() and j < Procese2.size())
    {
        if (Processes.at(i).ProcessId > Procese2.at(j).ProcessId)
        {
            New.push_back(Procese2.at(j).ProcessId);
            j++;
        }
        else if (Processes.at(i).ProcessId == Procese2.at(j).ProcessId)
        {
            if (strcmp(Processes.at(i).PathName, Procese2.at(j).PathName) != 0)
            {
                Changed.push_back(Processes.at(i).ProcessId);
            }
            //Procesele au aceeasi valoare,comparam pathul. daca au acelasi path, procesul inca exista
            //if path differit =>  DifProcess;
            //else => Ramasi;
            i++; 
            j++;
        }
        else
        {
            
            Old.push_back(Processes.at(i).ProcessId);
            i++;//Verificam in celelalte procese. Procesul este nou
        }
    }
    while (i < Processes.size())
    {
        Old.push_back(Processes.at(i).ProcessId); // Remaining in Procese have ended
        i++;
    }
    while (j < Procese2.size())
    {
        New.push_back(Procese2.at(j).ProcessId); // Remaining in Procese2 are new
        j++;
    }
    cout << endl << endl << "====================================================" << endl;
    cout << "NEW PROCESSES : " << New.size() << "; OLD PROCESSES : " << Old.size() << "; CHANGED PROCESSES : " << Changed.size() <<";" << endl;
    cout << endl << endl;
    if (Old.size() != 0)
    {
        cout << "The processes that have been terminated : ";
        for (auto x : Old)
        {
            cout << x << " ";
        }
        cout << endl << endl;
    }
    if (New.size() != 0)
    {
        cout << "The processes that have started are : ";
        for (auto x : New)
        {
            cout << x << " ";
        }
        cout << endl << endl;
    }
    if (Changed.size() != 0)
    {
        cout << "The processes that have inherited an ID are : ";
        for (auto x : Changed)
        {
            cout << x << " ";
        }
        cout << endl << endl;
    }
    cout << endl << endl << "====================================================" << endl;
}

DWORD ThreadFunction(LPVOID Param)
{
    UNREFERENCED_PARAMETER(Param);

    DWORD hWait;
    hWait = WaitForSingleObject(
        hEvent,
        INFINITE
    );
    switch (hWait)
    {
    case WAIT_OBJECT_0:
        //Iese din while
    {
        contor = -1;
        cout << endl << "Thank you for using the app. Bye!" << endl;
        return 1;
    }
    default:
        cout << "Wait Error : " << GetLastError() << endl;
        return 0;
    }
    cout << "SwitchExit" << endl;
    return 2;
}

void CreateEventAndThread()
{
    DWORD ThreadId;
    hEvent = CreateEvent(
        NULL,
        FALSE,
        FALSE,
        TEXT("EventCreate"));
    if (hEvent == NULL)
    {
        cout << "CreateEvent Error : " << GetLastError() << endl;
        return;
    }
    hThread = CreateThread(
        NULL,
        0,
        ThreadFunction,
        NULL,
        0,
        &ThreadId
    );
    if (hThread == NULL)
    {
        cout << "CreateThread Error : " << GetLastError() << endl;
        return;
    }

};

void App()
{
    CreateEventAndThread();
    while (contor != -1)
    {       
        if (_kbhit())
        {
            SetEvent(hEvent);
        }
        else
        {
            Snapshot(CurentSnapshot);
            if (contor != 0)
            {
                ProceseCompare(CurentSnapshot, PastSnapshot);
            }
            PastSnapshot.clear();
            copy(CurentSnapshot.begin(), CurentSnapshot.end(), back_inserter(PastSnapshot));
            CurentSnapshot.clear();
            contor++;
            Sleep(3000);
        }
            
    }
    CloseHandle(hEvent);
    CloseHandle(hThread);
}

int main()
{
    App();
}