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
#include <list>
#include "../utility/common.h"
#include "Texture.h"
#include "Material.h"

using std::string;

class Object;
class Mesh;
typedef std::list<const Object*> ObjectList;

struct IntersectResult{
    HitType result = MISS;
    float dist = INFDIST;
    VecF normal = NULL;
    const Object* hit_obj = NULL;
};

// simple isotropic objects
class Object {
public:
    Object(const Material &material, const string &name) : material(material), name(name) {}

    Object() {}

    virtual IntersectResult intersect(const Ray &ray) const = 0;

//    virtual VecF getNormal(const VecF &bottom_left) const = 0;

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

//    VecF getNormal(const VecF &bottom_left) const override;

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

//    VecF getNormal(const VecF &bottom_left) const override;

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

class AABB: public Object {
public:
    AABB() {}
    AABB(const string &name, const VecF &pos, const VecF &upper_right) : Object(Material(), name),
                                                                  bottom_left(pos), upper_right(upper_right) {}

    IntersectResult intersect(const Ray &ray) const override;

    Color getColor(const VecF &pos) const override;

private:
    // bottom_left should be left-bottom corner of the AABB
    VecF bottom_left, upper_right;
};

class Mesh : public Object{
friend class MeshTriangle;
public:
    Mesh(const string &name, const std::string file);

    IntersectResult intersect(const Ray &ray) const override;

    Color getColor(const VecF &pos) const override;

private:
    std::vector<VecF> points;
    std::vector<VecF> normal;
    ObjectList faces;
    AABB bbox;
};

class MeshTriangle: public Object{
public:
    MeshTriangle(const Material &material, const string &name, int p0, int p1, int p2, const Mesh *mesh);

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

#endif //RAYTRACING_OBJECT_H
