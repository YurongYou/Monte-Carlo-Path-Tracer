//
// Created by Ficos on 26/03/2017.
//

#include "Object.h"

IntersectResult Sphere::intersect(const Ray &ray) const {
    IntersectResult rst = {.result = MISS};
    VecF L = center - ray.getOrigin();
    float b = L.dot(ray.getDirection());
    // project to the ray
    float perpend = L.length2() - b * b;
    if (L.length() < radius) {
        rst.dist = b + (float) sqrt(radius2 - perpend);
        rst.result = INSIDE;
    } else {
        if (b < 0 || perpend > radius2) rst.result = MISS;
        else {
            rst.dist = b - (float) sqrt(radius2 - perpend);
            rst.result = HIT;
        }
    }
    return rst;
}

VecF Sphere::getNormal(VecF &pos) const {
    return (pos - center) / radius;
}

IntersectResult Plane::intersect(const Ray &ray) const {
    IntersectResult rst = {.result = MISS};
    float d = normal.dot(ray.getDirection());
    if (fabsf(d) > EPSILON) {
        float dist_to_plane = normal.dot(ray.getOrigin()) - shift;
        if (-dist_to_plane / d > 0){
            rst.dist = -dist_to_plane / d;
            rst.result = HIT;
        }
    }
    return rst;
}

VecF Plane::getNormal(VecF &pos) const {
    return normal;
}