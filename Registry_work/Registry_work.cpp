#include <iostream>
#include <Windows.h>
#include <windef.h>

using namespace std;

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383


void Read_Key(LPCTSTR Name, TCHAR* NameCheie, DWORD type, HKEY Pointer)
{
	HKEY ResultKey;
	TCHAR Value[255];
	DWORD Value_Length = 255;
	long xd = RegOpenKeyExA(Pointer, NameCheie, NULL, KEY_READ,&ResultKey);
	RegQueryValueExA(ResultKey, Name, NULL, &type, (LPBYTE)&Value, &Value_Length);
	RegCloseKey(ResultKey);
    if(Value != 0)
	    cout << Name << " : " << Value << endl;
}

void Get_Name(LPCTSTR Subkey, LPCTSTR Name, DWORD type)
{
    HKEY Pointer;
    long xd = RegOpenKeyExA(HKEY_LOCAL_MACHINE, Subkey, 0, KEY_READ, &Pointer);

    TCHAR    NumeCheie[MAX_KEY_LENGTH];   // buffer for subkey name
    DWORD    SizeNume = 0;                   // size of name string 
    TCHAR    NumeClasa[MAX_PATH] = TEXT("");  // buffer for class name 
    DWORD    SizeClasa = MAX_PATH;  // size of class string 
    DWORD    NrDeChei = 0;               // number of subkeys 
    DWORD    MaxCheieSize = 0;              // longest subkey size 
    DWORD    MaxClassSize = 0;              // longest class string 
    DWORD    Values = 0;              // number of values for key 
    DWORD    MaxValueName = 0;          // longest value name 
    DWORD    MaxValueData = 0;       // longest value data 
    DWORD    SecDescriptor = 0; // size of security descriptor 
    FILETIME LastWriteTime;      // last write time 

    DWORD i = 0, j = 0, retCode = 0;

    TCHAR  Valoare[MAX_VALUE_NAME];
    DWORD MarimeValoare = MAX_VALUE_NAME;

    // Get the class name and the value count. 
    retCode = RegQueryInfoKey(
        Pointer,                    // key handle 
        NumeClasa,
        &SizeClasa,
        NULL,
        &NrDeChei,               // numarul de chei 
        &MaxCheieSize,
        &MaxClassSize,
        &Values,                //numarul de valori
        &MaxValueName,
        &MaxValueData,
        &SecDescriptor,
        &LastWriteTime
    );
    if (NrDeChei)
    {
        cout << "Numarul de chei este : " << NrDeChei << endl << endl;

        for (int i = 0; i < NrDeChei; i++)
        {
            SizeNume = MAX_KEY_LENGTH;
            retCode = RegEnumKeyExA(
                Pointer,
                i,
                NumeCheie,
                &SizeNume,
                NULL,
                NULL,
                NULL,
                &LastWriteTime
            );
            if (retCode == ERROR_SUCCESS)
            {
                cout << "(" << i + 1 << ")" << " " << NumeCheie << endl;
            }
            Read_Key(Name, NumeCheie, type,Pointer);
            cout << endl << endl;
        }
    }


}

int main()
{
	Get_Name(TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall"),TEXT("DisplayName"), REG_SZ);
}