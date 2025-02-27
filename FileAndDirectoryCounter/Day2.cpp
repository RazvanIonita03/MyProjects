#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>
using namespace std;

int contor = 1, DContor =  1, FContor = 1;
vector<string> Parameters;
string PrimordialString = "C:\\Users\\Razvan-Intership\\source\\repos";



int FindNext(string CurrentParameter,HANDLE Handle)
{
    WIN32_FIND_DATA FileData;
    int FindNXTValue = FindNextFileA(Handle, &FileData);
    
    if (FindNXTValue == 0)
    {
        return 0;
    }
    else
    {
        if (FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if (FileData.cFileName[0] != '.')
            {
                cout << "Director " << contor << ": " << FileData.cFileName << endl;
                contor++;
                DContor++;
                Parameters.push_back(CurrentParameter + FileData.cFileName + "\\");
                return 1;
            }
        }
        else 
        {
            cout << "fisier " << contor << ": " << FileData.cFileName << endl;
            contor++;
            FContor++;
        }
    }
    return 1;
    
    
}
void DirectorySearch(string Directory,string CurrentParameter)
{
    WIN32_FIND_DATA FindFileData;
    HANDLE PrincipalHandle = FindFirstFileA((Directory + "*").c_str(), &FindFileData);
    if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    {
        if (FindFileData.cFileName[0] != '.')
        {
            cout << "Director " << contor << " este : " << FindFileData.cFileName << endl;
            contor++;
            DContor++;
            Parameters.push_back(CurrentParameter + FindFileData.cFileName + "\\");
        }
    }
    int StatusNext = FindNext(CurrentParameter, PrincipalHandle);
    while (StatusNext != 0)
    {
        StatusNext = FindNext(CurrentParameter, PrincipalHandle);
    }
    
}

void Show_DF_Numbers()
{
    cout << endl << "In pathul specificat s-au gasit " << DContor << " directoare si " << FContor << " fisiere." << endl;
}

void Program()
{
    Parameters.push_back("\\");
    string String = PrimordialString;
    while (Parameters.size() >= 1)
    {
        bool ok = 0;
        String = PrimordialString + Parameters.at(Parameters.size() - 1);
        string CurrentParameter = Parameters.at(Parameters.size() - 1);
        cout << endl << "Cautam in : " << String << endl << endl;
        Parameters.pop_back();
        DirectorySearch(String,CurrentParameter);
    }
    Show_DF_Numbers();
}

int main()
{
    Program();
}