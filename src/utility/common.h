//
// Created by Ficos on 26/03/2017.
//

#ifndef RAYTRACING_CONSTANTS_H
#define RAYTRACING_CONSTANTS_H

#define EPSILON			    0.001f
#define SAMPLE_SUB_PIX      1
#define SAMPLENUM           1
#define SAMPLE_AREA_LIGHT   32
#define PI	                3.141592653589793238462f

enum HitType {MISS, HIT, INSIDE};
#define INFDIST             1000000.0f

#endif //RAYTRACING_CONSTANTS_H