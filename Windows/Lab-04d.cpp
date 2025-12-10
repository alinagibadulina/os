#include <windows.h>
#include <iostream>
#include <tlhelp32.h>
#include <string>
#include <cctype>

using namespace std;
const char* username = "User-db4d49dd";

bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

string getOnlyLetters(const char* username) {
    string letters = "";
    for (int i = 0; i < strlen(username); i++) {
        if (isAlpha(username[i])) {
            letters += username[i];
        }
    }
    return letters;
}

void PrintProcessThreads(DWORD processId) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        cout << "Error creating snapshot: " << GetLastError() << endl;
        return;
    }

    THREADENTRY32 te32;
    te32.dwSize = sizeof(THREADENTRY32);

    cout << "=== Threads of process " << processId << " ===" << endl;

    if (Thread32First(hSnapshot, &te32)) {
        int threadCount = 0;

        do {
            if (te32.th32OwnerProcessID == processId) {
                threadCount++;
                cout << "Thread ID: " << te32.th32ThreadID
                    << " | Priority: " << (int)te32.tpBasePri
                    << endl;
            }
        } while (Thread32Next(hSnapshot, &te32));

        cout << "Total threads: " << threadCount << endl;

    }
    else {
        cout << "Thread32First failed: " << GetLastError() << endl;
    }

    CloseHandle(hSnapshot);
    cout << "=========================" << endl;
}

DWORD WINAPI Lab_04x(LPVOID lpParam) {
    int iterations = *(int*)lpParam;
    string letters = getOnlyLetters(username);

    DWORD pid = GetCurrentProcessId();
    DWORD tid = GetCurrentThreadId();

    cout << "Thread " << tid << " started (" << iterations << " iterations)" << endl;

    for (int i = 0; i < iterations; i++) {
        char letter = letters[i % letters.length()];
        printf("%d - %d - %d - %c\n", pid, tid, i + 1, letter);
        Sleep(300 + (rand() % 101));
    }

    cout << "Thread " << tid << " ended" << endl;
    return 0;
}

int main() {
    string letters = getOnlyLetters(username);
    DWORD currentPid = GetCurrentProcessId();
    cout << "Current PID: " << currentPid << endl;
    cout << "Username: " << username << endl;
    cout << "Letters only: " << letters << endl;

    cout << "BEFORE creating worker threads:" << endl;
    PrintProcessThreads(currentPid);

    int iter1 = 10;
    int iter2 = 15;
    HANDLE workerThreads[2];

    workerThreads[0] = CreateThread(NULL, 0, Lab_04x, &iter1, 0, NULL);
    workerThreads[1] = CreateThread(NULL, 0, Lab_04x, &iter2, 0, NULL);

    if (workerThreads[0] == NULL || workerThreads[1] == NULL) {
        cout << "Error creating worker threads!" << endl;
        return 1;
    }

    Sleep(500);

    cout << "AFTER creating worker threads:" << endl;
    PrintProcessThreads(currentPid);

    DWORD mainTid = GetCurrentThreadId();
    cout << "Main thread working (TID: " << mainTid << ")" << endl;

    for (int i = 0; i < 5; i++) {
        char letter = letters[i % letters.length()];
        printf("%d - %d - %d - %c\n", currentPid, mainTid, i + 1, letter);
        Sleep(400);
    }

    WaitForMultipleObjects(2, workerThreads, TRUE, INFINITE);
    cout << "AFTER worker threads completed:" << endl;
    PrintProcessThreads(currentPid);

    CloseHandle(workerThreads[0]);
    CloseHandle(workerThreads[1]);

    cout << "All threads completed and handles closed!" << endl;
    return 0;
}
