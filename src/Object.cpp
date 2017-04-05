//
// Created by Ficos on 26/03/2017.
//

#include <cassert>
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

VecF Sphere::getNormal(const VecF &pos) const {
    return (pos - center) / radius;
}

IntersectResult Plane::intersect(const Ray &ray) const {
    IntersectResult rst = {.result = MISS};
    float d = normal.dot(ray.getDirection());
    if (fabsf(d) > EPSILON) {
        float dist = (shift - normal.dot(ray.getOrigin())) / d;
        if (dist > 0){
            rst.dist = dist;
            rst.result = HIT;
        }
    }
    return rst;
}


VecF Plane::getNormal(const VecF &pos) const {
    return normal;
}

static inline IntersectResult triangleIntersect(const Ray &ray, const VecF* points) {
    IntersectResult rst = {.result = MISS};
    const VecF& dir = ray.getDirection();
    const VecF& origin = ray.getOrigin();
    float a = points[0].x - points[1].x;
    float b = points[0].y - points[1].y;
    float c = points[0].z - points[1].z;
    float d = points[0].x - points[2].x;
    float e = points[0].y - points[2].y;
    float f = points[0].z - points[2].z;
    float g = dir.x;
    float h = dir.y;
    float i = dir.z;
    float j = points[0].x - origin.x;
    float k = points[0].y - origin.y;
    float l = points[0].z - origin.z;

    float EIHF = e * i - h * f;
    float GFDI = g * f - d * i;
    float DHEG = d * h - e * g;

    float denom = a * EIHF + b *  GFDI + c * DHEG;
    float beta = (j * EIHF + k * GFDI + l * DHEG) / denom;

    if (beta <= 0.0f || beta >= 1.0f) return rst;

    float AKJB = a * k - j * b;
    float JCAL = j * c - a * l;
    float BLKC = b * l - k * c;

    float gamma = (i * AKJB + h * JCAL + g * BLKC) / denom;
    if (gamma <= 0.0f || beta + gamma >= 1.0f) return rst;

    float dist = - (f * AKJB + e * JCAL + d * BLKC) / denom;
    if (dist > 0){
        rst.result = HIT;
        rst.dist = dist;
    }
    return rst;
}

IntersectResult Triangle::intersect(const Ray &ray) const {
    return triangleIntersect(ray, points);
}

VecF Triangle::getNormal(const VecF &pos) const {
    return normal;
}

IntersectResult MeshTriangle::intersect(const Ray &ray) const {
//    assert(!mesh);
    VecF points[3];
    points[0] = mesh->points[p0];
    points[1] = mesh->points[p1];
    points[2] = mesh->points[p2];
    return triangleIntersect(ray, points);
}

VecF MeshTriangle::getNormal(const VecF &pos) const {
    // compute the average normal (if available)
    if (mesh->normal.size() == 0) return local_normal;
    float alpha = inverse[0].dot(pos);
    float beta = inverse[1].dot(pos);
    float gamma = inverse[2].dot(pos);
    return alpha * mesh->normal[p0] + beta * mesh->normal[p1] + gamma * mesh->normal[p2];
}
