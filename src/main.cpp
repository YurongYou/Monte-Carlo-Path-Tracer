#include <iostream>
#include <thread>
#include "Engine.h"

using namespace std;

int main(int argc, char *argv[]) {
    Engine::TraceConfig config;
    int i = 1;
    float fx = 2, fy = 4, fz = -12;
    float tx = 2, ty = 4, tz = 0;
    int image_height = 480;
    int image_width = 640;
    while (i < argc){
        if (strncmp(argv[i], "-MCPT", 5) == 0) {
            ++i;
            if (strncmp(argv[i], "true", 4) == 0){
                config.illumination_type = PURE_MCPT;
                config.sample_num = 1;
            }
            else if (strncmp(argv[i], "mix", 3) ==0 ){
                config.illumination_type = MIX;
                config.diffuse_sample_num = 32;
            }
        }
        if (strncmp(argv[i], "-test_case", 10) == 0){
            ++i;
            config.test = argv[i];
        }
        if (strncmp(argv[i], "-size", 6) == 0){
            ++i;
            image_height = std::stoi(argv[i]);
            ++i;
            image_width = std::stoi(argv[i]);
        }
        if (strncmp(argv[i], "-view", 6) == 0){
            // from
            ++i;
            fx = std::stoi(argv[i]);
            ++i;
            fy = std::stoi(argv[i]);
            ++i;
            fz = std::stoi(argv[i]);
            // to
            ++i;
            tx = std::stoi(argv[i]);
            ++i;
            ty = std::stoi(argv[i]);
            ++i;
            tz = std::stoi(argv[i]);
        }
        ++i;
    }
    config.num_worker = std::thread::hardware_concurrency();
    Engine(image_width, image_height, VecF(fx, fy, fz), VecF(tx, ty, tz)).render(config);
}