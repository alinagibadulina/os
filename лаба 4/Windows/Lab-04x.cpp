#include <windows.h>
#include <iostream>
#include <string>
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

    if (letters.empty()) {
        cout << "no letters" << endl;
        return 1;
    }

    DWORD pid = GetCurrentProcessId();
    DWORD tid = GetCurrentThreadId();

    for (int i = 0; i < iterations; i++) {
        char letter = letters[i % letters.length()];
        printf("%d - %d - %d - %c\n", pid, tid, i + 1, letter);
        Sleep(300 + (rand() % 101));
    }

    return 0;
}

int main() {
    int iterations = strlen(username); 
    cout << "Iterations number: " << iterations << endl;
    cout << "Username: " << username << endl;
    Lab_04x(&iterations);

    return 0;
}
