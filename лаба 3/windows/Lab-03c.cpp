#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <iomanip>

int main() {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hSnapshot == INVALID_HANDLE_VALUE) {
        std::cerr << "Error creating process snapshot!" << std::endl;
        return 1;
    }

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    std::cout << "List of running processes:" << std::endl;
    std::cout << std::left
        << std::setw(30) << "Process Name"
        << std::setw(10) << "PID"
        << std::setw(10) << "Parent PID"
        << std::endl;
    std::cout << std::string(50, '-') << std::endl;

    // Get first process
    if (Process32First(hSnapshot, &pe)) {
        do {
            std::cout << std::left
                << std::setw(30) << pe.szExeFile
                << std::setw(10) << pe.th32ProcessID
                << std::setw(10) << pe.th32ParentProcessID
                << std::endl;
        } while (Process32Next(hSnapshot, &pe));
    }

    CloseHandle(hSnapshot);
    return 0;
}
