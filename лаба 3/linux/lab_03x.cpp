#include <iostream>
#include <unistd.h>   // getpid(), usleep()
#include <cstdlib>    // atoi(), getenv(), exit()

int main(int argc, char* argv[]) {
    int iterations = 0;

    if (argc > 1) {
        iterations = std::atoi(argv[1]);
    } else {
        char* env_var = std::getenv("ITER_NUM");
        if (env_var != nullptr) {
            iterations = std::atoi(env_var);
        }
    }

    if (iterations <= 0) {
        std::cerr << "Error: number of iterations not specified!" << std::endl;
        return 1;
    }

    std::cout << "Number of iterations: " << iterations << std::endl;
    std::cout << "PID: " << getpid() << std::endl;

    for (int i = 1; i <= iterations; i++) {
        std::cout << "Iteration " << i << ", PID: " << getpid() << std::endl;
        usleep(500000); // 500 мс = 0.5 секунды
    }

    std::cout << "Process " << getpid() << " completed" << std::endl;
    return 0;
}


