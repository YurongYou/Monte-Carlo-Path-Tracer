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
#include "common.h"

using std::string;

class Material {
public:
    Material() : color(WHITE), reflection(0.0f), diffusion(0.2f), specular(0.5f), refraction(0.0f), refraction_index(1.0f) {}

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

    float getDiffusion() const {
        return diffusion;
    }

    void setDiffusion(float diffusion) {
        Material::diffusion = diffusion;
    }

    float getSpecular() const {
        return specular;
    }

    void setSpecular(float specular) {
        Material::specular = specular;
    }

    float getRefraction() const {
        return refraction;
    }

    void setRefraction(float refraction) {
        Material::refraction = refraction;
    }

    float getRefraction_index() const {
        return refraction_index;
    }

    void setRefraction_index(float refraction_index) {
        Material::refraction_index = refraction_index;
    }

    void clear(){
        color = WHITE;
        reflection = 0.0f;
        diffusion = 0.2f;
        specular = 0.5f;
        refraction = 0.0f;
        refraction_index = 1.0f;
    }
private:
    Color color;
    float reflection;
    float diffusion;
    float specular;
    float refraction;
    float refraction_index;
};

// simple isotropic objects
class Object {
public:
    Object(const Material &material, const string &name, bool light) : material(material), name(name), light(light) {}

    Object() {}

    virtual IntersectResult intersect(const Ray &ray) const = 0;

    virtual VecF getNormal(VecF &pos) const = 0;

    virtual Color getColor(VecF &) const { return material.getColor(); }

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

class Sphere : public Object {
public:
    Sphere(const Material &material, const string &name, bool light, const VecF &center, float radius) : Object(
            material, name, light), center(center), radius(radius), radius2(radius * radius) {}

    const VecF &getCenter() const {
        return center;
    }

    float getRadius() const {
        return radius;
    }

    IntersectResult intersect(const Ray &ray) const override;

    VecF getNormal(VecF &pos) const override;

    float getRadius2() const {
        return radius2;
    }

private:
    VecF center;
    float radius, radius2;
};

class Plane : public Object {
public:
    Plane(const Material &material, const string &name, bool light, const VecF &normal, const float &shift) : Object(
            material, name, light), normal(normal), shift(shift) {}

    IntersectResult intersect(const Ray &ray) const override;

    VecF getNormal(VecF &pos) const override;

private:
    VecF normal;
    float shift;
};

#endif //RAYTRACING_OBJECT_H
