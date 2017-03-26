//
// Created by Ficos on 26/03/2017.
//

#include "Object.h"
#include "constants.h"

int Sphere::intersect(const Ray &ray, float &dist) {
    VecF L = center - ray.getOrigin();
    float b = L.dot(ray.getDirection());
    // project to the ray
    float perpend = L.length2() - b * b;
    if (L.length() < radius) {
        dist = b + (float) sqrt(radius2 - perpend);
        return INPRIM;
    } else {
        if (b < 0 || perpend > radius2) return MISS;
        dist = b - (float) sqrt(radius2 - perpend);
        return HIT;
    }
    return MISS;
}

VecF Sphere::getNormal(VecF &pos) {
    return (pos - center) / radius;
}

int Plane::intersect(const Ray &ray, float &dist) {
    float d = normal.dot(ray.getDirection());
    if (d != 0) {
        float dist_to_plane = normal.dot(ray.getOrigin()) - shift;
        if (-dist_to_plane / d > 0){
            dist = -dist_to_plane / d;
            return HIT;
        }
    }
    return MISS;
}

VecF Plane::getNormal(VecF &pos) {
    return normal;
}