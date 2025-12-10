#include <windows.h>
#include <iostream>
#include <cstring>
#include <cctype>

using namespace std;
const char* username = "User-db4d49dd";

bool isABS(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
string getOnlyLetters(const char* username) {
    string letters = "";
    for (int i = 0; i < strlen(username); i++) {
        if (isABS(username[i])) {
            letters += username[i];
        }
    }
    return letters;
}

DWORD WINAPI Lab_04x(LPVOID lpParam) {
    int iterations = *(int*)lpParam;
    string letters = getOnlyLetters(username);
    DWORD pid = GetCurrentProcessId();
    DWORD tid = GetCurrentThreadId();

    cout << "Thread " << tid << " was started (" << iterations << " iterations)" << endl;

    for (int i = 0; i < iterations; i++) {
        char letter = letters[i % letters.length()];
        printf("%d - %d - %d - %c\n", pid, tid, i + 1, letter);
        Sleep(300 + (rand() % 101));
    }
    cout << "Thread " << tid << " was ended" << endl;
    return 0;
}

int main() {
    string letters = getOnlyLetters(username);
    cout << "Main thread PID: " << GetCurrentProcessId() << endl;
    cout << "Username: " << username << endl;

    int iter1 = 125;  
    int iter2 = 125;  
    int iterMain = 100; 

    HANDLE threads[2];
    DWORD threadIds[2];

    threads[0] = CreateThread(NULL, 0, Lab_04x, &iter1, 0, &threadIds[0]);
    if (threads[0] == NULL) {
        cout << "Error creating thread 1!" << endl;
        return 1;
    }

    threads[1] = CreateThread(NULL, 0, Lab_04x, &iter2, 0, &threadIds[1]);
    if (threads[1] == NULL) {
        cout << "Error creating thread 2!" << endl;
        CloseHandle(threads[0]);
        return 1;
    }

    cout << "Threads created:" << endl;
    cout << "Thread 1: " << iter1 << " iterations (TID: " << threadIds[0] << ")" << endl;
    cout << "Thread 2: " << iter2 << " iterations (TID: " << threadIds[1] << ")" << endl;
    cout << "Main thread: " << iterMain << " iterations" << endl;

    DWORD mainPid = GetCurrentProcessId();
    DWORD mainTid = GetCurrentThreadId();

    cout << "Main thread started work" << endl;

    for (int i = 0; i < iterMain; i++) {
        char letter = username[i % strlen(username)];
        printf("%d - %d - %d - %c\n", mainPid, mainTid, i + 1, letter);

        if (i == 19) { 
            cout << "!!! SUSPENDING Thread 1 at iteration 20 !!!" << endl;
            SuspendThread(threads[0]);
        }

        if (i == 59) { 
            cout << "!!! RESUMING Thread 1 at iteration 60 !!!" << endl;
            ResumeThread(threads[0]);
        }

        if (i == 39) { 
            cout << "!!! SUSPENDING Thread 2 at iteration 40 !!!" << endl;
            SuspendThread(threads[1]);
        }

        Sleep(300 + (rand() % 101));
    }
    cout << "!!! RESUMING Thread 2 after main thread completion !!!" << endl;
    ResumeThread(threads[1]);

    cout << "Main thread completed. Waiting for child threads." << endl;

    WaitForMultipleObjects(2, threads, TRUE, INFINITE);

    CloseHandle(threads[0]);
    CloseHandle(threads[1]);
    return 0;
}
