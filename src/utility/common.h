//
// Created by Ficos on 26/03/2017.
//

#ifndef RAYTRACING_CONSTANTS_H
#define RAYTRACING_CONSTANTS_H

#define EPSILON			    0.001f

#define SAMPLE_AREA_LIGHT   32
#define PI	                3.141592653589793238462f

#define RAY_TRACING_WITH_CAST   0
#define PURE_MCPT               1
#define MIX                     2

enum HitType {MISS, HIT, INSIDE};
#define INFDIST             1000000.0f

#endif //RAYTRACING_CONSTANTS_H