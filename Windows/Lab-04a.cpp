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

    cout << "Thread = " << tid << "was started the work (" << iterations << " iterations)" << endl;

    for (int i = 0; i < iterations; i++) {
        char letter = letters[i % letters.length()];
        printf("%d - %d - %d - %c\n", pid, tid, i + 1, letter);
        Sleep(300 + (rand() % 101));
    }
    cout << "Thread " << tid << " was ended the work" << endl;
    return 0;
}

int main() {
    string letters = getOnlyLetters(username);
    cout << "Main thread PID: " << GetCurrentProcessId() << endl;
    cout << "Username: " << username << endl;

    int iter1 = 50;
    int iter2 = 125;
    int iterMain = 100;

    HANDLE threads[2];

    threads[0] = CreateThread(NULL, 0, Lab_04x, &iter1, 0, NULL);
    if (threads[0] == NULL) {
        cout << "Error thread 1!" << endl;
        return 1;
    }

    threads[1] = CreateThread(NULL, 0, Lab_04x, &iter2, 0, NULL);
    if (threads[1] == NULL) {
        std::cout << "Error thread 2!" << std::endl;
        CloseHandle(threads[0]);
        return 1;
    }

    cout << "Threads created:" << endl;
    cout << "Thread 1: " << iter1 << " iterations" << endl;
    cout << "Thread 2: " << iter2 << " iterations" << endl;
    cout << "Main thread: " << iterMain << " iterations" << endl;

    Lab_04x(&iterMain);
    cout << "Main thread is ended. Wait threads 1 and 2" << endl;
    WaitForMultipleObjects(2, threads, TRUE, INFINITE);
    CloseHandle(threads[0]);
    CloseHandle(threads[1]);
    return 0;
}
