#include <iostream>
#include <Windows.h>
#include <string>
using namespace std;
#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383
void Access_Key(int Ordinul,TCHAR* Cheie,HKEY Pointer)
{
    HKEY Pointer2;
    long xd = RegOpenKeyExW(Pointer,Cheie, NULL, KEY_READ, &Pointer2);
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

    TCHAR  Valoare[MAX_VALUE_NAME] = { '\0' };
    DWORD MarimeValoare = MAX_VALUE_NAME;

    // Get the class name and the value count. 
    retCode = RegQueryInfoKeyW(
        Pointer2,                    // key handle 
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
    cout << endl << endl << MaxValueName << endl << endl;
    if (Values)
    {
        cout << "Numarul de valori este : " << Values << endl;
        for (int i = 0; i < Values; i++)
        {
            Valoare[0] = '\0';
            WCHAR lpData = new char[MAX_VALUE_NAME];
            DWORD lpDataLength = MAX_VALUE_NAME;
            MarimeValoare = MAX_VALUE_NAME;
            retCode = RegEnumValue(
                Pointer2,
                i,
                &Valoare[0],
                &MarimeValoare,
                NULL,
                NULL,
                (unsigned char*)lpData, 
                &lpDataLength);
            /*LSTATUS RegEnumValueA(
                [in]                HKEY    hKey,
                [in]                DWORD   dwIndex,
                [out]               LPSTR   lpValueName,
                [in, out]           LPDWORD lpcchValueName,
                LPDWORD lpReserved,
                [out, optional]     LPDWORD lpType,
                [out, optional]     LPBYTE  lpData,
                [in, out, optional] LPDWORD lpcbData
            );*/
            if (retCode == ERROR_SUCCESS)
            {   
                cout << "(" << i + 1 << ")" << " " << Valoare[0] << "   " << lpData[0] << endl;
            }
            else
            {
                cout << "(" << i + 1 << ")" << " eroare la afisarea sau citirea valorii" << endl;
            }
        }
       
    }
    else
    {
        cout << "Cheia (" << Ordinul << ") nu are valori" << endl;
    }
}

HKEY Get_Key()
{
	HKEY Pointer;
	long xd = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall", 0, KEY_READ, &Pointer);
	return Pointer;
}
void Get_Name(HKEY Pointer)
{
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

    TCHAR  Valoare[MAX_VALUE_NAME] = {'\0'};
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
            retCode = RegEnumKeyExW(
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
            
            Access_Key(i + 1,NumeCheie,Pointer);
            cout << endl << endl;
        }
    }
    

}

    

int main()
{
    Get_Name(Get_Key());
    RegCloseKey(Get_Key());
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
