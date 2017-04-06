//
// Created by Ficos on 26/03/2017.
//

#ifndef RAYTRACING_OBJECT_H
#define RAYTRACING_OBJECT_H

#include "../utility/Vector.h"
#include "../utility/Ray.h"
#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>
#include "../utility/common.h"
#include "Texture.h"
#include "Material.h"

using std::string;

struct IntersectResult{
    HitType result = MISS;
    float dist = INFDIST;
    VecF normal = NULL;
};

// simple isotropic objects
class Object {
public:
    Object(const Material &material, const string &name) : material(material), name(name) {}

    Object() {}

    virtual IntersectResult intersect(const Ray &ray) const = 0;

//    virtual VecF getNormal(const VecF &pos) const = 0;

    virtual Color getColor(const VecF &pos) const = 0;

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

//    VecF getNormal(const VecF &pos) const override;

    float getRadius2() const {
        return radius2;
    }

    Color getColor(const VecF &pos) const override;

private:
    VecF center;
    float radius, radius2;
};

class Plane : public Object {
public:
    Plane(const Material &material, const string &name, const VecF &normal, const float &shift) : Object(
            material, name), normal(normal), shift(shift) {
        if (!(normal.cross(VecF(0, 1, 0)).isEqual(VecF(0)))){
            axis[0] = normal.cross(VecF(0, 1, 0));
        } else {
            axis[0] = normal.cross(VecF(1, 0, 0));
        }
        axis[0].normalize();
        axis[1] = axis[0].cross(normal);
        axis[1].normalize();
    }

    IntersectResult intersect(const Ray &ray) const override;

    Color getColor(const VecF &pos) const override;

//    VecF getNormal(const VecF &pos) const override;

private:
    VecF normal;
    float shift;
    // for texture
    VecF axis[2];
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

    VecF getMixPoint(const float& alpha, const float& beta) const;

    Color getColor(const VecF &pos) const override;

private:
    VecF points[3];
    VecF normal;
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
    }

    IntersectResult intersect(const Ray &ray) const override;

    const VecF &getLocal_normal() const {
        return local_normal;
    }

    Color getColor(const VecF &pos) const override;

private:
    int p0, p1, p2;
    const Mesh* mesh;
    VecF local_normal;
};

class AABB: public Object {
public:
    AABB(const string &name, const VecF &pos, const VecF &size) : Object(Material(), name),
                                                                  pos(pos), size(size) {}

    IntersectResult intersect(const Ray &ray) const override;

    Color getColor(const VecF &pos) const override;

private:
    // pos should be left-bottom corner of the AABB
    VecF pos, size;
};

class Mesh {
public:
    std::vector<VecF> points;
    std::vector<VecF> normal;
};

#endif //RAYTRACING_OBJECT_H
