//
// Created by Ficos on 26/03/2017.
//

#ifndef RAYTRACING_CONSTANTS_H
#define RAYTRACING_CONSTANTS_H

#define EPSILON			0.001f
#define TRACEDEPTH		5
#define SAMPLE_SUB_PIX  8
#define SAMPLENUM       512
#define PI	3.141592653589793238462f

enum HitType {MISS, HIT, INSIDE};
#define INFDIST  1000000.0f

struct IntersectResult{
    HitType result;
    float dist;
};

#endif //RAYTRACING_CONSTANTS_H