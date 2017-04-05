//
// Created by Ficos on 26/03/2017.
//

#ifndef RAYTRACING_ENGINE_H
#define RAYTRACING_ENGINE_H


#include <random>
#include "Scene.h"
#include <time.h>

class Engine {
public:
    struct TraceConfig {
        int num_worker = 4;
//        int num_trace_depth = 5;
        bool isGlobalIllumination = true;
        TraceConfig() {}
    };

    struct TraceResult {
        Color color;
        const Object* hit_object;
        float dist;
    };

    Engine(int image_width, int image_height, float world_x1, float world_y1, float world_x2, float world_y2);

    ~Engine();

    TraceResult rayTrace(const Ray &ray, const float& r_index, const int&  depth, const TraceConfig& config);

    void render(TraceConfig& config);

    void drawPicture(const Color* canvas);

    VecF uniformHemisphereSample();

private:
    Scene *scene;
    int image_width, image_height;
    float world_x1, world_y1, world_x2, world_y2, w_dx, w_dy;
    std::default_random_engine gen;
    std::uniform_real_distribution<float> uni;
};


#endif //RAYTRACING_ENGINE_H