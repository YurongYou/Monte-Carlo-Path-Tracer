//
// Created by Ficos on 01/04/2017.
//

#include <cassert>
#include "Light.h"

VecF PointLight::getIntensity(const VecF &pos, const VecF &dir) const {
    float cosI = fabsf(dir.dot(shape.getNormal(pos)));
//    std::cout << shape.getNormal(pos).length() << std::endl;
    return intensity * cosI;
}

IntersectResult PointLight::intersect(const Ray &ray) const {
    return shape.intersect(ray);
}

const Sphere &PointLight::getShape() const {
    return shape;
}

VecF TriangleLight::getIntensity(const VecF &pos, const VecF &dir) const {
    float cosI = fabsf(dir.dot(shape.getNormal(pos)));
    return intensity * cosI;
}

IntersectResult TriangleLight::intersect(const Ray &ray) const {
    return shape.intersect(ray);
}
