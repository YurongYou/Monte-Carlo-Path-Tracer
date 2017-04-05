//
// Created by Ficos on 26/03/2017.
//

#ifndef RAYTRACING_OBJECT_H
#define RAYTRACING_OBJECT_H

#include "utility/Vector.h"
#include "utility/Ray.h"
#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>
#include "utility/common.h"

using std::string;

class Material {
public:
    Material() : emission(BLACK), intrinsic_color(WHITE),  diffuse_prob(0.8f),
                 reflection_prob(0.2f), refraction_prob(0.0f), base_reflection_rate(0.5f), refraction_index(1.0f) {}

    const Color &getEmission() const {
        return emission;
    }


    void clear(){
        emission = BLACK;
        intrinsic_color = WHITE;
        diffuse_prob = 0.8f;
        reflection_prob = 0.2f;
        refraction_prob = 0.0f;
        base_reflection_rate = 0.5f;
        refraction_index = 1.0f;
    }

public:
    void setEmission(const Color &emission) {
        Material::emission = emission;
    }

    const Color &getIntrinsic_color() const {
        return intrinsic_color;
    }

    void setIntrinsic_color(const Color &intrinsic_color) {
        Material::intrinsic_color = intrinsic_color;
    }

    float getDiffuse_prob() const {
        return diffuse_prob;
    }

    void setDiffuse_prob(float diffuse_prob) {
        Material::diffuse_prob = diffuse_prob;
    }

    float getReflection_prob() const {
        return reflection_prob;
    }

    void setReflection_prob(float reflection_prob) {
        Material::reflection_prob = reflection_prob;
    }

    float getRefraction_prob() const {
        return refraction_prob;
    }

    void setRefraction_prob(float refraction_prob) {
        Material::refraction_prob = refraction_prob;
    }

    float getBase_reflection_rate() const {
        return base_reflection_rate;
    }

    void setBase_reflection_rate(float base_reflection_rate) {
        Material::base_reflection_rate = base_reflection_rate;
    }

    float getRefraction_index() const {
        return refraction_index;
    }

    void setRefraction_index(float refraction_index) {
        Material::refraction_index = refraction_index;
    }

private:
    Color emission;
    Color intrinsic_color;
    float diffuse_prob;
    float reflection_prob;
    float refraction_prob;

    float base_reflection_rate;

    float refraction_index;
};

// simple isotropic objects
class Object {
public:
    Object(const Material &material, const string &name) : material(material), name(name) {}

    Object() {}

    virtual IntersectResult intersect(const Ray &ray) const = 0;

    virtual VecF getNormal(const VecF &pos) const = 0;

    virtual Color getColor(VecF &) const { return material.getIntrinsic_color(); }


    const string &getName() const { return name; }

    const Material &getMaterial() const { return material; }


protected:
    Material material;
    string name;
};

class Sphere : public Object {
public:
    Sphere(const Material &material, const string &name, const VecF &center, float radius) : Object(
            material, name), center(center), radius(radius), radius2(radius * radius) {}

    const VecF &getCenter() const {
        return center;
    }

    float getRadius() const {
        return radius;
    }

    IntersectResult intersect(const Ray &ray) const override;

    VecF getNormal(const VecF &pos) const override;

    float getRadius2() const {
        return radius2;
    }

private:
    VecF center;
    float radius, radius2;
};

class Plane : public Object {
public:
    Plane(const Material &material, const string &name, const VecF &normal, const float &shift) : Object(
            material, name), normal(normal), shift(shift) {}

    IntersectResult intersect(const Ray &ray) const override;

    VecF getNormal(const VecF &pos) const override;

private:
    VecF normal;
    float shift;
};

class Triangle: public Object{
public:
    Triangle(const Material &material, const string &name,
             const VecF& p0, const VecF& p1, const VecF& p2) :
            Object(material, name) {
        points[0] = p0;
        points[1] = p1;
        points[2] = p2;
        normal = (p1 - p0).cross(p2 - p0);
        normal.normalize();
    }

    IntersectResult intersect(const Ray &ray) const override;

    VecF getNormal(const VecF &pos) const override;

private:
    VecF points[3];
    VecF normal;
};

class Mesh {
public:
    std::vector<VecF> points;
    std::vector<VecF> normal;
    void addVertex(const VecF& v){
        points.emplace_back(v);
    }
};

class MeshTriangle: public Object{
public:
    MeshTriangle(const Material &material, const string &name, int p0, int p1, int p2, const Mesh *mesh) : Object(
            material, name), p0(p0), p1(p1), p2(p2), mesh(mesh) {

        VecF point0 = mesh->points[p0];
        VecF point1 = mesh->points[p1];
        VecF point2 = mesh->points[p2];
        local_normal = (point1 - point0).cross(point2 - point0);
        local_normal.normalize();
        float det = (-point0.z)*point1.y*point2.x + point0.y*point1.z*point2.x + point0.z*point1.x*point2.y - point0.x*point1.z*point2.y - point0.y*point1.x*point2.z + point0.x*point1.y*point2.z;
        inverse[0] = VecF((-point1.z)*point2.y + point1.y*point2.z, point1.z*point2.x - point1.x*point2.z, (-point1.y)*point2.x + point1.x*point2.y) / det;
        inverse[1] = VecF(point0.z*point2.y - point0.y*point2.z, (-point0.z)*point2.x + point0.x*point2.z, point0.y*point2.x - point0.x*point2.y) / det;
        inverse[2] = VecF((-point0.z)*point1.y + point0.y*point1.z, point0.z*point1.x - point0.x*point1.z, (-point0.y)*point1.x + point0.x*point1.y) / det;
    }

    IntersectResult intersect(const Ray &ray) const override;

    VecF getNormal(const VecF &pos) const override;

    const VecF &getLocal_normal() const {
        return local_normal;
    }

private:
    int p0, p1, p2;
    const Mesh* mesh;
    VecF local_normal;
    VecF inverse[3];
};

#endif //RAYTRACING_OBJECT_H
