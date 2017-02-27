#include "serials.h"
#include <windows.h>
#include <stimulator_gui/params.h>

QStringList listOfAvailableSerials() {
    QStringList r;
    QString com;
    for (int i = 1; i <= WINDOWS_MAX_COM; ++i) {
        HANDLE hComm;
        std::wstring wstringPath{L"\\\\.\\COM"}; // snazime se vytvorit \\.\COM1 ale v MS string formatu
        wstringPath.append(std::to_wstring(i));
        const wchar_t *FileFullPath = wstringPath.c_str();
        hComm = CreateFileW(FileFullPath,
                            GENERIC_READ | GENERIC_WRITE,
                            0,
                            0,
                            OPEN_EXISTING,
                            FILE_FLAG_OVERLAPPED,
                            0);
        if (hComm != INVALID_HANDLE_VALUE) { // pokud je handle platna, tak serial existuje
            com = "COM" + QString::number(i);
            r.append(com);
            CloseHandle(hComm);
        }
    }

    /* zalozni kod v pripade ze se ukaze ze vyse udedena metoda neni spolehliva:

    for (int i = 1; i <= 12; i++) {
        com = "COM" + QString::number(i);
        r.append(com);
    }
    */

    return r;
};
