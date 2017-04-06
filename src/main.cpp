#include <iostream>
#include <thread>
#include "Engine.h"

using namespace std;

int main(int argc, char *argv[]) {
    TraceConfig config;
    int i = 1;
    float fx = 0, fy = 4, fz = -10;
    float tx = 0, ty = 4, tz = 0;
    int image_height = 480;
    int image_width = 640;
    while (i < argc){
        if (strncmp(argv[i], "-MCPT", 5) == 0) {
            ++i;
            if (strncmp(argv[i], "true", 4) == 0){
                config.illumination_type = PURE_MCPT;
                config.sample_num = 1024;
            }
            else if (strncmp(argv[i], "mix", 3) ==0 ){
                config.illumination_type = MIX;
                config.diffuse_sample_num = 32;
            }
        }
        else if (strncmp(argv[i], "-test_case", 10) == 0){
            ++i;
            config.test = argv[i];
        }
        else if (strncmp(argv[i], "-size", 6) == 0){
            ++i;
            image_height = std::stoi(argv[i]);
            ++i;
            image_width = std::stoi(argv[i]);
        }
        else if (strncmp(argv[i], "-view", 6) == 0){
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
        else if (strncmp(argv[i], "-obj", 4) == 0){
            ++i;
            config.obj = argv[i];
        }
        else if (strncmp(argv[i], "-meshtype", 9) == 0){
            ++i;
            config.mesh_diff = std::stof(argv[i]);
            ++i;
            config.mesh_ks = std::stof(argv[i]);
            ++i;
            config.mesh_refl = std::stof(argv[i]);
        } else if (strncmp(argv[i], "-sample_num", 10) == 0){
            ++i;
            config.sample_num = std::stoi(argv[i]);
        }
        ++i;
    }
    config.num_worker = std::thread::hardware_concurrency();
    Engine(image_width, image_height, VecF(fx, fy, fz), VecF(tx, ty, tz)).render(config);
}