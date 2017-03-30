#include <iostream>
#include <thread>
#include "Engine.h"

using namespace std;

int main() {
    Engine::TraceConfig config;
    config.num_worker = std::thread::hardware_concurrency();
    Engine(5120, 3840, -4, 3, 4, -3).render(config);
}