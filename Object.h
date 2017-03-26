//
// Created by Ficos on 26/03/2017.
//

#ifndef RAYTRACING_OBJECT_H
#define RAYTRACING_OBJECT_H

#include "Vector.h"
#include "Ray.h"
#include <cstdlib>
#include <string>
#include <iostream>
using std::string;

class Material{
public:
    Material(): color(Color((char)255, (char)255, (char)255)), reflection(0.0f),
                diffusion(0.2f) {}

    Material(const Color &color, float reflection, float diffusion) : color(color), reflection(reflection),
                                                                      diffusion(diffusion) {}

    const Color &getColor() const {
        return color;
    }

    void setColor(const Color &color) {
        Material::color = color;
    }

    float getReflection() const {
        return reflection;
    }

    void setReflection(float reflection) {
        Material::reflection = reflection;
    }

    float getDifussion() const {
        return diffusion;
    }

    void setDifussion(float difussion) {
        Material::diffusion = difussion;
    }

private:
    Color color;
    float reflection;
    float diffusion;
};

// simple isotropic objects
class Object {
public:
    Object(const Material &material, const string &name, bool light) : material(material), name(name), light(light) {}
    Object() {}

    virtual int intersect(const Ray &ray, float &dist) = 0;
    virtual VecF getNormal(VecF &pos) = 0;
    virtual Color getColor(VecF &) { return material.getColor(); }
    bool isLight() const { return light; }
    const string &getName() const { return name; }
    void setName(const string &name) { Object::name = name; }
    const Material &getMaterial() const { return material; }
    void setMaterial(const Material &material) { Object::material = material; }
    void setLight(bool light) { Object::light = light; }

protected:
    Material material;
    string name;
    bool light;
};

class Sphere: public Object{
public:
    Sphere(const Material &material, const string &name, bool light, const VecF &center, float radius) : Object(
            material, name, light), center(center), radius(radius), radius2(radius * radius) {}

    const VecF &getCenter() const {
        return center;
    }

    float getRadius() const {
        return radius;
    }

    int intersect(const Ray &ray, float &dist) override;

    VecF getNormal(VecF &pos) override;

    float getRadius2() const {
        return radius2;
    }

private:
    VecF center;
    float radius, radius2;
};

class Plane: public Object{
public:
    Plane(const Material &material, const string &name, bool light, const VecF &normal, const float &shift) : Object(
            material, name, light), normal(normal), shift(shift) {}

    int intersect(const Ray &ray, float &dist) override;

    VecF getNormal(VecF &pos) override;

private:
    VecF normal;
    float shift;
};

#endif //RAYTRACING_OBJECT_H
