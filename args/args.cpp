#include <stdio.h>
#include <windows.h>
#include <tlhelp32.h>
#include <Psapi.h>
int wmain(int argc, wchar_t *argv[ ], wchar_t *envp[ ] )
{
	printf("Path:\n");
	wchar_t sExeName[MAX_PATH];
	GetModuleFileName(NULL, sExeName, MAX_PATH);
	wprintf(L" %s\n\n", sExeName);
	printf("Command line:\n");
	wprintf(L" %s\n\n", GetCommandLine());
	printf("Current directory:\n");
	wchar_t sCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, sCurrentDir);
	wprintf(L" %s\n\n",sCurrentDir);
	printf("Parent:\n");

	int pid = -1;
    HANDLE hOwn = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe = { 0 };
    pe.dwSize = sizeof(PROCESSENTRY32);
    pid = GetCurrentProcessId();


    if(Process32First(hOwn, &pe))
	{
        do
		{
			if (pe.th32ProcessID == pid)
			{
				HANDLE hParent = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe.th32ParentProcessID);
				if (hParent) 
				{
					wchar_t sParentName[MAX_PATH];
					if (GetModuleFileNameEx(hParent, 0, sParentName, MAX_PATH))
					{
						wprintf(L" %s(%d)\n", sParentName, pe.th32ParentProcessID);
					}
					else
					{
						wprintf(L" Unknown(%d)\n", pe.th32ParentProcessID);
					}
					CloseHandle(hParent);
				}				
				break;
            }
        }
		while(Process32Next(hOwn, &pe));
    }

    CloseHandle(hOwn);


	printf("Arguments:\n");
	for( int i = 1; i < argc; i++ )
	{
		wprintf(L" %d: %s\n", i, argv[i] );
	}
	putchar('\n');

	printf("Environment:\n ");
	LPWCH lpvEnv = GetEnvironmentStrings();
	if (lpvEnv != NULL)
	{
		LPTSTR lpszVariable;
		for (lpszVariable = (LPTSTR) lpvEnv; *lpszVariable; lpszVariable++)
		{
		while (*lpszVariable)
			putchar(*lpszVariable++);
		printf("\n ");
		}
		FreeEnvironmentStrings(lpvEnv);
	}
	else
	{
		printf("GetEnvironmentStrings() failed.");
	}
	printf("\n\n");

	printf("Press ENTER to exit");
	getchar();
	return 0;
}