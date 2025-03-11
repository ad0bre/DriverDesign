#include <Windows.h>
#include <winreg.h>
#include <stdio.h>

void queryKey(
	HKEY rootKey,
	const char* subKey,
	const char* variable
)
{
	HKEY hKey;
	if (RegOpenKeyExA(rootKey, subKey, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
	{
		printf("Could not open key: %s\n", subKey);
		return;
	}

	char subKeyName[256];
	DWORD subkeyNameLen;
	DWORD index = 0;

	FILETIME lastWriteTime;

    while (1) {
        subkeyNameLen = sizeof(subKeyName);
        if (RegEnumKeyExA(hKey, index, subKeyName, &subkeyNameLen, NULL, NULL, NULL, &lastWriteTime) != ERROR_SUCCESS)
            break;

        char fullSubKeyPath[512];
        snprintf(fullSubKeyPath, sizeof(fullSubKeyPath), "%s\\%s", subKey, subKeyName);

        HKEY hSubKey;
        if (RegOpenKeyExA(rootKey, fullSubKeyPath, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS) {
            char valueData[256] = { 0 };
            DWORD valueSize = sizeof(valueData);
            DWORD type;

            if (RegQueryValueExA(hSubKey, variable, NULL, &type, (LPBYTE)valueData, &valueSize) == ERROR_SUCCESS) {
                if (type == REG_SZ || type == REG_EXPAND_SZ) {
                    valueData[valueSize - 1] = '\0';
                    printf("Subkey: %s, %s: %s\n", subKeyName, variable, valueData);
                }
            }
            RegCloseKey(hSubKey);
        }

        index++;
    }

    RegCloseKey(rootKey);
}

int main(void)
{
    queryKey(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services", "ImagePath");
    return 0;
}
