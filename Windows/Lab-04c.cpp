#include <windows.h>
#include <iostream>
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

struct ThreadData {
    int iterations;
    int threadNumber;
    HANDLE threadHandle;
};

DWORD WINAPI Lab_04x(LPVOID lpParam) {
    ThreadData* data = (ThreadData*)lpParam;
    int iterations = data->iterations;
    int threadNumber = data->threadNumber;
    string letters = getOnlyLetters(username);

    DWORD pid = GetCurrentProcessId();
    DWORD tid = GetCurrentThreadId();

    cout << "Thread " << tid << " (Thread " << threadNumber << ") started (" << iterations << " iterations)" << endl;

    for (int i = 0; i < iterations; i++) {
        char letter = letters[i % letters.length()];
        printf("%d - %d - %d - %c\n", pid, tid, i + 1, letter);
        Sleep(300 + (rand() % 101));

        if (threadNumber == 2 && i == 39) {
            cout << "!!! TERMINATING Thread 2 at iteration 40 !!!" << endl;
            TerminateThread(data->threadHandle, 0);
            break;
        }
    }

    cout << "Thread " << tid << " (Thread " << threadNumber << ") ended" << endl;
    return 0;
}

int main() {
    string letters = getOnlyLetters(username);
    cout << "Main thread PID: " << GetCurrentProcessId() << endl;
    cout << "Username: " << username << endl;
    cout << "Letters only: " << letters << endl;

    ThreadData thread1Data = { 125, 1, NULL };
    ThreadData thread2Data = { 125, 2, NULL };

    HANDLE threads[2];

    threads[0] = CreateThread(NULL, 0, Lab_04x, &thread1Data, 0, NULL);
    if (threads[0] == NULL) {
        cout << "Error creating thread 1!" << endl;
        return 1;
    }
    thread1Data.threadHandle = threads[0];

    threads[1] = CreateThread(NULL, 0, Lab_04x, &thread2Data, 0, NULL);
    if (threads[1] == NULL) {
        cout << "Error creating thread 2!" << endl;
        CloseHandle(threads[0]);
        return 1;
    }
    thread2Data.threadHandle = threads[1];

    cout << "Threads created:" << endl;
    cout << "Thread 1: " << thread1Data.iterations << " iterations" << endl;
    cout << "Thread 2: " << thread2Data.iterations << " iterations (will be terminated at iteration 40)" << endl;
    cout << "Main thread: 100 iterations" << endl;

    DWORD mainPid = GetCurrentProcessId();
    DWORD mainTid = GetCurrentThreadId();

    cout << "Main thread started work" << endl;

    for (int i = 0; i < 100; i++) {
        char letter = letters[i % letters.length()];
        printf("%d - %d - %d - %c\n", mainPid, mainTid, i + 1, letter);

        if (i == 39) {
            cout << "!!! TERMINATING Thread 2 at main thread iteration 40 !!!" << endl;
            TerminateThread(threads[1], 0);
        }

        Sleep(300 + (rand() % 101));
    }

    cout << "Main thread completed. Waiting for child threads..." << endl;
    DWORD result = WaitForMultipleObjects(2, threads, TRUE, INFINITE);

    if (result == WAIT_FAILED) {
        cout << "Wait failed! Error: " << GetLastError() << endl;
    }
    else {
        cout << "All threads completed waiting!" << endl;
    }

    DWORD exitCode1, exitCode2;
    GetExitCodeThread(threads[0], &exitCode1);
    GetExitCodeThread(threads[1], &exitCode2);

    cout << "Thread 1 exit code: " << exitCode1 << endl;
    cout << "Thread 2 exit code: " << exitCode2 << endl;
    
    CloseHandle(threads[0]);
    CloseHandle(threads[1]);

    cout << "All thread handles closed!" << endl;
    return 0;
}
