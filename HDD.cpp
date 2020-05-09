#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <winioctl.h>
#include <stdlib.h>
#include <string>
#include <iostream>
using namespace std;
struct {
    UINT type;
    LPCSTR name;
}
DriveTypeFlags[] = {
  { DRIVE_UNKNOWN, "Unknown" },
  { DRIVE_NO_ROOT_DIR, "Invalid path" },
  { DRIVE_REMOVABLE, "Removable" },
  { DRIVE_FIXED, "Fixed" },
  { DRIVE_REMOTE, "Network drive" },
  { DRIVE_CDROM, "CD-ROM" },
  { DRIVE_RAMDISK, "RAM disk" },
  { 0, NULL},
};
BOOL GetDriveGeometry(DISK_GEOMETRY* pdg, const char* driveName) {
    HANDLE hDevice;
    BOOL bResult;
    DWORD junk;
    // формируем строку формата “\\.\C:” для получения хэндла диска
    char str[7];
    str[0] = str[1] = str[3] = '\\';
    str[2] = '.';
    str[4] = driveName[0];
    str[5] = driveName[1];
    str[6] = '\0';
    // получаем хэндл диска
    hDevice = CreateFileA(str, 0, FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_EXISTING, 0, NULL);
    if (hDevice == INVALID_HANDLE_VALUE)
        return FALSE;
    // Получаем данные о диске
    bResult = DeviceIoControl(hDevice, IOCTL_DISK_GET_DRIVE_GEOMETRY,
        NULL, 0, pdg, sizeof(*pdg), &junk, (LPOVERLAPPED)NULL);
    CloseHandle(hDevice);
    return bResult;
}
int _tmain(int argc, _TCHAR* argv[]) {
    DISK_GEOMETRY pdg;
    BOOL bResult;
    ULONGLONG DiskSize;
    char buf[100];
    // Получаем список дисков
    DWORD len = GetLogicalDriveStringsA(sizeof(buf) / sizeof(char), buf);
    LPCSTR sDrivePath;
    for (char* s = buf; *s; s += strlen(s) + 1) {
        sDrivePath = (LPCSTR)s;
        cout << "Disc " << s << "\n";
        bResult = GetDriveGeometry(&pdg, s);
        if (bResult) {
            printf("Cylinders = %I64d\n", pdg.Cylinders);
            cout << "Tracks/cylinder = " << pdg.TracksPerCylinder << "\n";
            cout << "Sectors/track = " << pdg.SectorsPerTrack << "\n";
            cout << "Bytes/sector = " << pdg.BytesPerSector << "\n";
            DiskSize = pdg.Cylinders.QuadPart * ((ULONG)pdg.TracksPerCylinder) *
                ((ULONG)pdg.SectorsPerTrack) * ((ULONG)pdg.BytesPerSector);
            cout << "Disk size = " << DiskSize << " (Bytes) = "
                << DiskSize / (1024 * 1024 * 1024) << " (Gb)\n";
        }
        else
            cout << "GetDriveGeometry failed. Error " << GetLastError() << "\n";
        // Получаем тип диска
        UINT uDriveType = GetDriveTypeA(sDrivePath);
        for (int i = 0; DriveTypeFlags[i].name; i++) {
            if (uDriveType == DriveTypeFlags[i].type) {
                cout << "Drive type : " << DriveTypeFlags[i].name << "\n";
                cout << "----------------------\n";
                break;
            }
        }
    }
    system("PAUSE");
    return EXIT_SUCCESS;
}