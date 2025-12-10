#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <sched.h>
#include <time.h>
#include <sys/resource.h>
#include <string>
#include <locale.h>
#include <mutex>
#include <sstream>

std::mutex printMutex;

typedef struct {
    int threadIndex;        
    int niceValue;
    int iterationsCompleted;
    int finished;
} ThreadData;

void safePrint(const std::string& s) {
    std::lock_guard<std::mutex> lock(printMutex);
    std::cout << s << std::flush;
}

void* ThreadFunction(void* lpParam) {
    ThreadData* data = (ThreadData*)lpParam;

    clock_t startTime = clock();

    const int TOTAL_ITERATIONS  = 1000000;
    const int REPORT_INTERVAL   = 1000;
    const int DELAY_US          = 200000;

    pid_t pid = getpid();
    pid_t tid = syscall(SYS_gettid);

    if (setpriority(PRIO_PROCESS, tid, data->niceValue) == -1) {
        perror("setpriority");
    }

    int actualNice = getpriority(PRIO_PROCESS, tid);

    {
        std::stringstream ss;
        ss << "[Поток " << data->threadIndex << "] Запущен | TID=" << tid
           << " | Nice=" << actualNice << "\n";
        safePrint(ss.str());
    }

    for (int i = 0; i <= TOTAL_ITERATIONS; i++) {
        data->iterationsCompleted = i;

        if (i % REPORT_INTERVAL == 0) {
            int cpu = sched_getcpu();

            std::stringstream ss;
            ss << "[Поток " << data->threadIndex << "] Итерация=" << i
               << " | PID=" << pid
               << " | TID=" << tid
               << " | Nice=" << actualNice
               << " | CPU=" << cpu << "\n";
            safePrint(ss.str());

            usleep(DELAY_US);
        }
    }

    clock_t endTime = clock();
    double elapsed = double(endTime - startTime) / CLOCKS_PER_SEC;

    {
        std::stringstream ss;
        ss << "[Поток " << data->threadIndex << "] Завершён | Время=" << elapsed << " сек\n";
        safePrint(ss.str());
    }

    data->finished = 1;
    return NULL;
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "ru_RU.UTF-8");

    if (argc != 4) {
        std::cout << "Использование: ./Lab-05c <маска_родственности> <nice1> <nice2>\n";
        return 1;
    }

    int cpu_mask = atoi(argv[1]);
    int nice1 = atoi(argv[2]);
    int nice2 = atoi(argv[3]);

    std::cout << "--- ПАРАМЕТРЫ ЗАПУСКА ---\n";
    std::cout << "Маска родства CPU: " << cpu_mask << "\n";
    std::cout << "Nice потока 1:     " << nice1 << "\n";
    std::cout << "Nice потока 2:     " << nice2 << "\n\n";

    if (cpu_mask != -1) {
        cpu_set_t set;
        CPU_ZERO(&set);
        CPU_SET(cpu_mask, &set);
        if (sched_setaffinity(0, sizeof(cpu_set_t), &set) == -1) {
            perror("sched_setaffinity");
            return 1;
        }
    }

    ThreadData data1 = { 1, nice1, 0, 0 };
    ThreadData data2 = { 2, nice2, 0, 0 };

    pthread_t threads[2];
    pthread_create(&threads[0], NULL, ThreadFunction, &data1);
    pthread_create(&threads[1], NULL, ThreadFunction, &data2);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    std::cout << "\n--- ИТОГ ---\n";
    std::cout << "Итерации потока 1: " << data1.iterationsCompleted << "\n";
    std::cout << "Итерации потока 2: " << data2.iterationsCompleted << "\n";
    std::cout << "Разница: " 
              << abs(data1.iterationsCompleted - data2.iterationsCompleted) << "\n";

    return 0;
}

