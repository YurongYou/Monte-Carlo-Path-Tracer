#include <iostream>
#include <thread>
#include "Engine.h"

using namespace std;

int main() {
    Engine::TraceConfig config;
    config.num_worker = std::thread::hardware_concurrency();
//    config.num_worker = 1;
    Engine(640, 480, -4, 3, 4, -3).render(config);
}