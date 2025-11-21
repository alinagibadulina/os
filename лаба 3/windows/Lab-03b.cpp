#include <windows.h>
#include <iostream>
#include <string>
#include <vector>

int main() {
    STARTUPINFOA si[3];
    PROCESS_INFORMATION pi[3];
    BOOL success[3];

    std::string exe_path = "Lab-03x.exe";

    std::cout << "Parent process PID: " << GetCurrentProcessId() << std::endl;

    // Массив количества итераций для 3 процессов
    const char* iterations[3] = { "4", "6", "8" };

    // Создаём процессы
    for (int i = 0; i < 3; i++) {
        ZeroMemory(&si[i], sizeof(si[i]));
        si[i].cb = sizeof(si[i]);
        ZeroMemory(&pi[i], sizeof(pi[i]));

        // Локальное окружение для процесса
        std::string env_str = "ITER_NUM=" + std::string(iterations[i]);
        std::vector<char> env_block(env_str.begin(), env_str.end());
        env_block.push_back('\0'); // двойной нулевой терминатор для CreateProcess
        env_block.push_back('\0');

        if (i == 0) {
            // 1-й процесс: файл без аргументов
            std::cout << "\n1. Creating process (no arguments)..." << std::endl;
            success[i] = CreateProcessA(
                exe_path.c_str(),  // lpApplicationName
                NULL,              // lpCommandLine
                NULL, NULL, FALSE, 0,
                env_block.data(),  // lpEnvironment
                NULL,
                &si[i],
                &pi[i]
            );
        }
        else if (i == 1) {
            // 2-й процесс: аргумент через командную строку
            std::cout << "2. Creating process (with argument)..." << std::endl;
            std::string cmd_str = exe_path + " " + iterations[i];
            std::vector<char> cmd(cmd_str.begin(), cmd_str.end());
            cmd.push_back('\0');
            success[i] = CreateProcessA(
                NULL,            // lpApplicationName
                cmd.data(),      // lpCommandLine
                NULL, NULL, FALSE, 0,
                env_block.data(), // lpEnvironment
                NULL,
                &si[i],
                &pi[i]
            );
        }
        else {
            // 3-й процесс: NULL в командной строке
            std::cout << "3. Creating process (NULL command line)..." << std::endl;
            success[i] = CreateProcessA(
                exe_path.c_str(), // lpApplicationName
                NULL,             // lpCommandLine
                NULL, NULL, FALSE, 0,
                env_block.data(), // lpEnvironment
                NULL,
                &si[i],
                &pi[i]
            );
        }
    }

    // Вывод результатов
    for (int i = 0; i < 3; i++) {
        std::cout << "Process " << (i + 1) << ": "
            << (success[i] ? "SUCCESS" : "FAILED")
            << ", PID: " << (success[i] ? pi[i].dwProcessId : 0)
            << std::endl;
    }

    // Ждём завершения
    std::cout << "\nWaiting for child processes to complete..." << std::endl;
    for (int i = 0; i < 3; i++) {
        if (success[i]) {
            WaitForSingleObject(pi[i].hProcess, INFINITE);
            CloseHandle(pi[i].hProcess);
            CloseHandle(pi[i].hThread);
        }
    }

    std::cout << "All child processes completed!" << std::endl;
    return 0;
}
