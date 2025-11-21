#include <windows.h>
#include <iostream>

int main() {
    STARTUPINFOA si[3];
    PROCESS_INFORMATION pi[3];
    BOOL success[3];

    // Инициализация структур
    for (int i = 0; i < 3; i++) {
        ZeroMemory(&si[i], sizeof(si[i]));
        si[i].cb = sizeof(si[i]);
        ZeroMemory(&pi[i], sizeof(pi[i]));
    }

    std::cout << "Parent PID: " << GetCurrentProcessId() << std::endl;

    // --- Установим глобальную переменную окружения ITER_NUM ---
    SetEnvironmentVariableA("ITER_NUM", "4");

    std::string exe = "Lab-03x.exe";

    // Первый процесс — только имя файла (без аргументов)
    std::cout << "\n1. Creating process (only filename)..." << std::endl;
    success[0] = CreateProcessA(
        exe.c_str(),  // lpApplicationName
        NULL,         // lpCommandLine — отсутствует
        NULL, NULL, FALSE, 0,
        NULL, NULL, &si[0], &pi[0]
    );

    // Второй процесс — как обычно, с аргументом
    std::cout << "2. Creating process (with command line argument)..." << std::endl;
    success[1] = CreateProcessA(
        NULL,
        (LPSTR)"Lab-03x.exe 6",
        NULL, NULL, FALSE, 0,
        NULL, NULL, &si[1], &pi[1]
    );

    // Третий процесс — NULL во втором параметре, локальное окружение
    std::cout << "3. Creating process (NULL command line, local env var)..." << std::endl;

    // локальная переменная окружения ITER_NUM = 8
    SetEnvironmentVariableA("ITER_NUM", "8");

    success[2] = CreateProcessA(
        exe.c_str(),  // lpApplicationName
        NULL,         // lpCommandLine = NULL
        NULL, NULL, FALSE, 0,
        NULL, NULL, &si[2], &pi[2]
    );

    // --- Выводим результаты ---
    for (int i = 0; i < 3; i++) {
        std::cout << "Process " << (i + 1) << ": "
            << (success[i] ? "SUCCESS" : "FAILED")
            << ", PID: " << (success[i] ? pi[i].dwProcessId : 0)
            << std::endl;
    }

    // --- Ждём завершения ---
    std::cout << "\nWaiting for all processes..." << std::endl;
    for (int i = 0; i < 3; i++) {
        if (success[i])
            WaitForSingleObject(pi[i].hProcess, INFINITE);
    }

    for (int i = 0; i < 3; i++) {
        if (success[i]) {
            CloseHandle(pi[i].hProcess);
            CloseHandle(pi[i].hThread);
        }
    }

    // Восстановим переменную окружения
    SetEnvironmentVariableA("ITER_NUM", NULL);

    std::cout << "All processes completed!" << std::endl;
    return 0;
}
