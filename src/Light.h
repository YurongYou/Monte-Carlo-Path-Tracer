//
// Created by Ficos on 01/04/2017.
//

#ifndef RAYTRACING_LIGHT_H
#define RAYTRACING_LIGHT_H


#include "Vector.h"
#include "Ray.h"
#include "Object.h"

class Light {
public:
    // in coming dir
    virtual VecF getIntensity(const VecF &pos, const VecF &dir) const = 0;
    virtual IntersectResult intersect(const Ray &ray) const = 0;
};

class PointLight: public Light {
public:
    PointLight(const VecF &center, float radius, const VecF &intensity) : shape(Material(), "light_sphere_shape", center, radius),
                                                                          intensity(intensity){}

    VecF getIntensity(const VecF &pos, const VecF &dir) const override;

    IntersectResult intersect(const Ray &ray) const override;

    const VecF &getCenter() const {
        return shape.getCenter();
    }

private:
    Sphere shape;
public:
    const Sphere &getShape() const;

private:
    VecF intensity;
};

class TriangleLight: public Light{
public:
    TriangleLight(const VecF& p0, const VecF& p1, const VecF& p2, const VecF &intensity) : shape(Material(), "light_triangle_shape", p0, p1, p2),
                                                                                           intensity(intensity) {}

    VecF getIntensity(const VecF &pos, const VecF &dir) const override;

    IntersectResult intersect(const Ray &ray) const override;

private:
    Triangle shape;
    VecF intensity;
};
#endif //RAYTRACING_LIGHT_H
