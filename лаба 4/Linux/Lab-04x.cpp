#include <iostream>
#include <string>
#include <cctype>
#include <cstring>
#include <thread>
#include <chrono>
#include <sys/syscall.h>
#include <unistd.h>

using namespace std;

const char* username = "User-db4d49dd";

bool isABS(char c) {
    return isalpha(static_cast<unsigned char>(c));
}

string getOnlyLetters(const char* username) {
    string letters;
    for (int i = 0; i < strlen(username); i++) {
        if (isABS(username[i])) {
            letters += username[i];
        }
    }
    return letters;
}

void Lab_04x(int iterations) {
    string letters = getOnlyLetters(username);

    if (letters.empty()) {
        cout << "no letters" << endl;
        return;
    }

    pid_t pid = getpid();
    pid_t tid = syscall(SYS_gettid);

    for (int i = 0; i < iterations; i++) {
        char letter = letters[i % letters.length()];
        cout << pid << " - " << tid << " - " << i + 1 << " - " << letter << endl;
        this_thread::sleep_for(chrono::milliseconds(300 + rand() % 101));
    }
}

int main() {
    srand(time(nullptr));

    int iterations = strlen(username);
    cout << "Iterations number: " << iterations << endl;
    cout << "Username: " << username << endl;

    Lab_04x(iterations);
    return 0;
}
