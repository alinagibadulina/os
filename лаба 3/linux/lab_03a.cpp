#include <iostream>
#include <unistd.h>      // fork(), exec(), getpid()
#include <sys/wait.h>    // waitpid()
#include <cstdlib>       // setenv(), exit()
#include <cstring>       // strerror()

int main() {
    pid_t pids[2];

    std::cout << "Parent PID: " << getpid() << std::endl;

    // ============================
    // 1. Первый дочерний процесс
    // Аргумент через командную строку
    // ============================
    pids[0] = fork();

    if (pids[0] == -1) {
        std::cerr << "Error creating first process: " << strerror(errno) << std::endl;
        return 1;
    }

    if (pids[0] == 0) {
        // Дочерний процесс 1
        std::cout << "Child 1 started, PID: " << getpid() << std::endl;

        char *args[] = { (char*)"./lab_03x", (char*)"4", nullptr };
        execvp(args[0], args);

        std::cerr << "Failed to start child 1: " << strerror(errno) << std::endl;
        exit(1);
    }

    // ============================
    // 2. Второй дочерний процесс
    // Параметр через переменную окружения ITER_NUM
    // ============================
    pids[1] = fork();

    if (pids[1] == -1) {
        std::cerr << "Error creating second process: " << strerror(errno) << std::endl;
        return 1;
    }

    if (pids[1] == 0) {
        std::cout << "Child 2 started, PID: " << getpid() << std::endl;

        // Локальная установка переменной окружения (только для этого процесса)
        setenv("ITER_NUM", "6", 1);

        char *args[] = { (char*)"./lab_03x", nullptr };
        execvp(args[0], args);

        std::cerr << "Failed to start child 2: " << strerror(errno) << std::endl;
        exit(1);
    }

    // ============================
    // Родительский процесс
    // ============================
    std::cout << "Waiting for child processes to finish..." << std::endl;

    for (int i = 0; i < 2; i++) {
        int status;
        pid_t finished = waitpid(pids[i], &status, 0);

        if (finished == -1) {
            std::cerr << "Error waiting for child " << i+1 << ": " << strerror(errno) << std::endl;
        } else if (WIFEXITED(status)) {
            std::cout << "Child " << i+1 << " (PID " << finished
                      << ") exited with code " << WEXITSTATUS(status) << std::endl;
        } else {
            std::cout << "Child " << i+1 << " (PID " << finished
                      << ") terminated abnormally" << std::endl;
        }
    }

    std::cout << "All child processes completed!" << std::endl;
    return 0;
}
