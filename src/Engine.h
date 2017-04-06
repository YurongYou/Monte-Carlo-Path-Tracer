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
        int num_trace_depth = 5;
        int illumination_type = RAY_TRACING_WITH_CAST;
        int sub_pixel = 2;
        int sample_num = 1;
        int diffuse_sample_num = 32;
        std::string test;

        TraceConfig() {}
    };

    struct TraceResult {
        Color color;
        const Object* hit_object;
        float dist;
    };

    Engine(int image_width, int image_height, const VecF& from, const VecF& to);

    ~Engine();

    TraceResult rayTrace(const Ray &ray, const float& r_index, const int&  depth, const TraceConfig& config);

    void render(TraceConfig& config);

    void drawPicture(const Color* canvas);

    VecF uniformHemisphereSample();

private:
    Scene *scene;
    int image_width, image_height;
    VecF from;
    VecF left_right;
    VecF dx, dy;
    std::default_random_engine gen;
    std::uniform_real_distribution<float> uni;

    Color castRay(const Object* obj, const Object* light, const Ray &ray, const VecF& ori, const VecF &N, const VecF& hit) const;
};


#endif //RAYTRACING_ENGINE_H