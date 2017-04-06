//
// Created by Ficos on 26/03/2017.
//

#ifndef RAYTRACING_CONSTANTS_H
#define RAYTRACING_CONSTANTS_H

#define EPSILON			    0.00001f

#define SAMPLE_AREA_LIGHT   16
#define PI	                3.141592653589793238462f

#define RAY_TRACING_WITH_CAST   0
#define PURE_MCPT               1
#define MIX                     2

enum HitType {MISS, HIT, INSIDE};
#define INFDIST             1000000.0f

struct TraceConfig {
    int num_worker = 4;
    int num_trace_depth = 5;
    int illumination_type = RAY_TRACING_WITH_CAST;
    int sub_pixel = 2;
    int sample_num = 1;
    int diffuse_sample_num = 32;
    std::string test;
    std::string obj = "../models/twist.obj";
    float mesh_diff = 1.0f;
    float mesh_refl = 0.0f;
    float mesh_ks   = 0.8f;
    TraceConfig() {}
};

#endif //RAYTRACING_CONSTANTS_H