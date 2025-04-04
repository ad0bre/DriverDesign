#include <windows.h>
#include <setupapi.h>
#include <initguid.h>
#include <devguid.h>
#include <usbiodef.h>
#include <devpkey.h>
#include <stdio.h>

#pragma comment(lib, "setupapi.lib")

void getUsbDeviceProperties(HDEVINFO hDevInfo, SP_DEVINFO_DATA devInfoData, DWORD property, const char* label)
{
    char buffer[1024];
    DWORD requiredSize = 0;
    DWORD dataType;

	if (SetupDiGetDeviceRegistryPropertyA(hDevInfo, &devInfoData, property, &dataType, (PBYTE)buffer, sizeof(buffer), &requiredSize))
	{
		printf("%s: %s\n", label, buffer);
	}
	else
	{
		printf("Failed to get property for %s\n", label);
	}
}

void listUSBDevices()
{
    HDEVINFO deviceInfoSet;
    SP_DEVINFO_DATA deviceInfoData;
    DWORD deviceIndex = 0;

    deviceInfoSet = SetupDiGetClassDevs(&GUID_DEVCLASS_USB, NULL, NULL,DIGCF_PRESENT);

    if (deviceInfoSet == INVALID_HANDLE_VALUE) 
    {
        printf("Could not get the device list.\n");
        return 1;
    }

    printf("Listing USB devices:\n");

    deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

    while (SetupDiEnumDeviceInfo(deviceInfoSet, deviceIndex, &deviceInfoData))
    {
        printf("Device %d:\n", deviceIndex + 1);

		getUsbDeviceProperties(deviceInfoSet, deviceInfoData, SPDRP_DEVICEDESC, "Device Description");

		getUsbDeviceProperties(deviceInfoSet, deviceInfoData, SPDRP_HARDWAREID, "Hardware ID");

		getUsbDeviceProperties(deviceInfoSet, deviceInfoData, SPDRP_MFG, "Manufacturer");

		getUsbDeviceProperties(deviceInfoSet, deviceInfoData, SPDRP_FRIENDLYNAME, "Friendly Name");

		getUsbDeviceProperties(deviceInfoSet, deviceInfoData, SPDRP_LOCATION_INFORMATION, "Location Information");

        printf("\n");

		deviceIndex++;
    }

}

int main() {
    listUSBDevices();
    return 0;
}
