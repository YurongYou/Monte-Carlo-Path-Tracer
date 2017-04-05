#include <iostream>
#include <thread>
#include "Engine.h"

using namespace std;

int main(int argc, char *argv[]) {
    Engine::TraceConfig config;
    int i = 1;
    while (i < argc){
        if (strncmp(argv[i], "-MCPT", 5) == 0) {
            ++i;
            if (strncmp(argv[i], "true", 4) == 0)
                config.isGlobalIllumination = true;
        }
        if (strncmp(argv[i], "-test_case", 10) == 0){
            ++i;
            config.test = argv[i];
        }
        ++i;
    }
    config.num_worker = std::thread::hardware_concurrency();
    Engine(640, 480, VecF(0, 2, -7), VecF(0, 2, 0)).render(config);
}