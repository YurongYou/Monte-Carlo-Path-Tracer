//
// Created by Ficos on 26/03/2017.
//

#include <cassert>
#include "Object.h"

IntersectResult Sphere::intersect(const Ray &ray) const {
    IntersectResult rst;
    VecF L = center - ray.getOrigin();
    float b = L.dot(ray.getDirection());
    // project to the ray
    float perpend = L.length2() - b * b;
    if (L.length() < radius) {
        rst.dist = b + (float) sqrt(radius2 - perpend);
        rst.result = INSIDE;
        rst.normal = (center - ray.getOrigin() - ray.getDirection() * rst.dist) / radius;
    } else {
        if (b < 0 || perpend > radius2) rst.result = MISS;
        else {
            rst.dist = b - (float) sqrt(radius2 - perpend);
            rst.result = HIT;
            rst.normal = (ray.getOrigin() + ray.getDirection() * rst.dist - center) / radius;
        }
    }
    return rst;
}

IntersectResult Plane::intersect(const Ray &ray) const {
    IntersectResult rst;
    float d = normal.dot(ray.getDirection());
    if (fabsf(d) > EPSILON) {
        float dist = (shift - normal.dot(ray.getOrigin())) / d;
        if (dist > 0){
            rst.dist = dist;
            rst.result = HIT;
            rst.normal = normal;
        }
    }
    return rst;
}

//Color Plane::getColor(VecF &pos) const override {
//    if (this->material.getTexture()){
//        VecF diff = pos - normal * shift;
//        float u = diff.dot(axis[0]);
//        float v = diff.dot(axis[1]);
//        return this->material.getTexture()->get_color(u, v);
//    } else {
//        return this->getMaterial().getIntrinsic_color();
//    }
//}

Color Plane::getColor(const VecF &pos) const {
    if (this->material.getTexture()){
        VecF diff = pos - normal * shift;
        float u = diff.dot(axis[0]);
        float v = diff.dot(axis[1]);
        return this->material.getTexture()->get_color(u, v);
    } else {
        return this->getMaterial().getIntrinsic_color();
    }
}

struct TriangleIntersectResult{
    IntersectResult result;
    float u, v;
};

// ref: https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection
static inline TriangleIntersectResult triangleIntersect(const Ray &ray, const VecF* points) {
    TriangleIntersectResult rst;
    rst.result.result = MISS;
    const VecF& dir = ray.getDirection();
    const VecF& origin = ray.getOrigin();

    VecF v0v1 = points[1] - points[0];
    VecF v0v2 = points[2] - points[0];
    VecF pvec = dir.cross(v0v2);
    float det = v0v1.dot(pvec);
    if (fabs(det) < EPSILON) return rst;

    float invDet = 1 / det;
    VecF tvec = origin - points[0];
    float u = tvec.dot(pvec) * invDet;
    if (u < 0 || u > 1) return rst;

    VecF qvec = tvec.cross(v0v1);
    float v = dir.dot(qvec) * invDet;
    if (v < 0 || u + v > 1) return rst;

    float dist = v0v2.dot(qvec) * invDet;
    if (dist > 0){
        rst.result.result = HIT;
        rst.result.dist = dist;
        rst.u = u;
        rst.v = v;
    }
    return rst;
}

IntersectResult Triangle::intersect(const Ray &ray) const {
    IntersectResult rst = triangleIntersect(ray, points).result;
    if (rst.result == HIT) rst.normal = normal;
    return rst;
}

VecF Triangle::getMixPoint(const float &alpha, const float &beta) const{
//    assert(alpha >= 0 && alpha <= 1);
//    assert(beta >= 0 && beta <= 1);
    float gamma = 1 - alpha - beta;
//    assert(gamma >= 0);
    return alpha * points[0] + beta * points[1] + gamma * points[2];
}

IntersectResult MeshTriangle::intersect(const Ray &ray) const {
    VecF points[3];
    points[0] = mesh->points[p0];
    points[1] = mesh->points[p1];
    points[2] = mesh->points[p2];
    TriangleIntersectResult temp_rst = triangleIntersect(ray, points);
    IntersectResult rst = temp_rst.result;
    if (rst.result == HIT) {
        rst.normal = ((1 - temp_rst.u - temp_rst.v) * mesh->normal[p0] +
                temp_rst.u * mesh->normal[p1] +
                temp_rst.v * mesh->normal[p2]);
        rst.normal.normalize();
//        rst.normal = local_normal;
    }
    return rst;
}