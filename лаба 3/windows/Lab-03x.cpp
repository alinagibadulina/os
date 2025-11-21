#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <string>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int iterations = 0;

    if (argc > 1) {
        iterations = atoi(argv[1]);
    }
    else {
        char* env_var = getenv("ITER_NUM");
        if (env_var != NULL) {
            iterations = atoi(env_var);
        }
    }

    if (iterations <= 0) {
        std::cerr << "Error: number of iterations not specified!" << std::endl;
        ExitProcess(1);
    }

    std::cout << "Number of iterations: " << iterations << std::endl;
    std::cout << "PID: " << GetCurrentProcessId() << std::endl;

    for (int i = 1; i <= iterations; i++) {
        std::cout << "Iteration " << i << ", PID: " << GetCurrentProcessId() << std::endl;
        Sleep(500);
    }
    std::cout << "Process " << GetCurrentProcessId() << " completed" << std::endl;
    return 0;
}
