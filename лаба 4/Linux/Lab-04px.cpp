#include <iostream>
#include <string>
#include <cctype>
#include <cstring>
#include <thread>
#include <chrono>
#include <unistd.h>  
#include <vector>
#include <stdexcept>
#include <sys/syscall.h> 

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

void Lab_04x(int iterations, const string& threadName) {
    try {
        string letters = getOnlyLetters(username);
        if (letters.empty()) {
            cerr << "[" << threadName << "] error: no letters in username" << endl;
            return;
        }

        pid_t pid = getpid();
        pid_t tid = syscall(SYS_gettid);

        cout << "[" << threadName << "] started (PID=" << pid
             << ", TID=" << tid << ", iterations=" << iterations << ")" << endl;

        for (int i = 0; i < iterations; i++) {
            char letter = letters[i % letters.length()];
            cout << pid << " - " << tid << " - " << i + 1 << " - " << letter << endl;
            this_thread::sleep_for(chrono::milliseconds(300 + rand() % 101));
        }

        cout << "[" << threadName << "] completed" << endl;
    }
    catch (const exception& e) {
        cerr << "[" << threadName << "] exception: " << e.what() << endl;
    }
    catch (...) {
        cerr << "[" << threadName << "] unknown error" << endl;
    }
}

int main() {
    srand(time(nullptr));

    cout << "Main process PID: " << getpid() << endl;
    cout << "Username: " << username << endl;
    cout << "Letters only: " << getOnlyLetters(username) << endl;

    try {
        thread t1(Lab_04x, 50, "Thread-1");
        thread t2(Lab_04x, 125, "Thread-2");

        cout << "Created threads:\n"
             << "Thread-1 → 50 iterations\n"
             << "Thread-2 → 125 iterations\n"
             << "Main thread → 100 iterations\n" << endl;

        Lab_04x(100, "Main-Thread");

        cout << "[Main] waiting for threads to complete..." << endl;

        if (t1.joinable()) t1.join();
        if (t2.joinable()) t2.join();

        cout << "[Main] all threads completed successfully." << endl;
    }
    catch (const exception& e) {
        cerr << "[Main] error: " << e.what() << endl;
    }
    catch (...) {
        cerr << "[Main] unknown error occurred" << endl;
    }

    return 0;
}
