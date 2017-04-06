//
// Created by Ficos on 26/03/2017.
//

#include <cassert>
#include <fstream>
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
        rst.hit_obj = this;
    } else {
        if (b < 0 || perpend > radius2) rst.result = MISS;
        else {
            rst.dist = b - (float) sqrt(radius2 - perpend);
            rst.result = HIT;
            rst.normal = (ray.getOrigin() + ray.getDirection() * rst.dist - center) / radius;
            rst.hit_obj = this;
        }
    }
    return rst;
}

Color Sphere::getColor(const VecF &pos) const {
    // fixme: implement sphere texture mapping
    return this->getMaterial().getIntrinsic_color();
}

IntersectResult Plane::intersect(const Ray &ray) const {
    IntersectResult rst = IntersectResult();
    float d = normal.dot(ray.getDirection());
    if (fabsf(d) > EPSILON) {
        float dist = (shift - normal.dot(ray.getOrigin())) / d;
        if (dist > 0){
            rst.dist = dist;
            rst.result = HIT;
            rst.normal = normal;
            rst.hit_obj = this;
        }
    }
    return rst;
}

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
    if (rst.result == HIT) {
        rst.normal = normal;
        rst.hit_obj = this;
    }
    return rst;
}

Color Triangle::getColor(const VecF &pos) const {
    // fixme: implement triangle texture mapping
    return this->getMaterial().getIntrinsic_color();
}

VecF Triangle::getMixPoint(const float &alpha, const float &beta) const{
    float gamma = 1 - alpha - beta;
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
        rst.hit_obj = this;
    }
    return rst;
}

Color MeshTriangle::getColor(const VecF &pos) const {
    // fixme: implement mesh triangle texture mapping
    return this->getMaterial().getIntrinsic_color();
}

IntersectResult AABB::intersect(const Ray &ray) const {
    IntersectResult rst = IntersectResult();
    VecF dir = ray.getDirection(), origin = ray.getOrigin();
    VecF v1 = bottom_left, v2 = upper_right;
    float dist[6] = {0};
    VecF ip[6];
    if (fabsf(dir.x) > EPSILON) {
        float rc = 1.0f / dir.x;
        dist[0] = (v1.x - origin.x) * rc;
        dist[3] = (v2.x - origin.x) * rc;
    }
    if (fabsf(dir.y) > EPSILON) {
        float rc = 1.0f / dir.y;
        dist[1] = (v1.y - origin.y) * rc;
        dist[4] = (v2.y - origin.y) * rc;
    }
    if (fabsf(dir.z) > EPSILON) {
        float rc = 1.0f / dir.z;
        dist[2] = (v1.z - origin.z) * rc;
        dist[5] = (v2.z - origin.z) * rc;
    }
    for (int i = 0; i < 6; i++){
        if (dist[i] > 0) {
            ip[i] = origin + dist[i] * dir;
            if ((ip[i].x > (v1.x - EPSILON)) && (ip[i].x < (v2.x + EPSILON)) &&
                (ip[i].y > (v1.y - EPSILON)) && (ip[i].y < (v2.y + EPSILON)) &&
                (ip[i].z > (v1.z - EPSILON)) && (ip[i].z < (v2.z + EPSILON))) {
                if (rst.result == MISS || rst.dist > dist[i]) {
                    rst.dist = dist[i];
                    rst.result = HIT;
                }
            }
        }
    }
    return rst;
}

Color AABB::getColor(const VecF &pos) const {
    std::cerr << "AABB should not have any color" << std::endl;
    exit(1);
}


#define LINE_MAX_LEN 1000

Mesh::Mesh(const string &name, const std::string file) : Object(Material(), name) {
    // load .obj file
    std::ifstream fin(file);
    if (!fin.is_open()) {
        std::cerr << "file not found, exit." << std::endl;
        exit(1);
    }
    char input[LINE_MAX_LEN];
    int count_face = 0;
    std::vector<int> count_normal;
    Material triangle_material = Material();
    triangle_material.setDiffuse_prob(0.0f);
    triangle_material.setReflection_prob(1.0f);
    triangle_material.setKs(0.8f);
    triangle_material.setIntrinsic_color( CYAN );
    // null point (to deal with the 1-base problem)
    count_normal.push_back(0);
    points.push_back(VecF(0, 0, 0));
    normal.push_back(VecF(0));
    std::vector<VecF> given_normal;
    bool hasGivenNormal = false;
    given_normal.push_back(VecF(0));

    VecF bottom_left(INFDIST);
    VecF upper_right(-INFDIST);
    while (fin.getline(input, LINE_MAX_LEN, '\n')) {
        if (input[0] == '#' or strlen(input) <= 1)
            continue;
        switch (input[0]){
            case 'v': {
                if (input[1] == ' ') {
                    // v x y z
                    float x, y, z;
                    sscanf(input + 2, "%f %f %f", &x, &y, &z);
                    if (x < bottom_left.x) bottom_left.x = x;
                    if (y < bottom_left.y) bottom_left.y = y;
                    if (z < bottom_left.z) bottom_left.z = z;
                    if (x > upper_right.x) upper_right.x = x;
                    if (y > upper_right.y) upper_right.y = y;
                    if (z > upper_right.z) upper_right.z = z;
                    points.push_back(VecF(x, y, z));
                    count_normal.push_back(0);
                    normal.push_back(VecF(0));
                }
                else if (input[1] == 'n'){
                    float x, y, z;
                    sscanf(input + 3, "%f %f %f", &x, &y, &z);
                    given_normal.push_back(VecF(x, y, z));
                    hasGivenNormal = true;
                }
                else if (input[1] == 't') {
                    // do nothing
                } else {
                    std::cerr << input << std::endl;
                    std::cerr << "unrecognized format" << std::endl;
                    exit(1);
                }
                break;
            }
            case 'f': {
                int p[3];
                if (!hasGivenNormal){
                    sscanf(input + 2, "%d %d %d", &p[0], &p[1], &p[2]);
                } else {
                    int n[3];
                    sscanf(input + 2, "%d//%d %d//%d %d//%d", &p[0], &n[0], &p[1], &n[1], &p[2], &n[2]);
                    for (int i = 0; i < 3; ++i) {
                        normal[p[i]] = given_normal[n[i]];
                    }
                }
                MeshTriangle *tri = new MeshTriangle(triangle_material, "triangle_" + std::to_string(count_face),
                                                     p[0], p[1], p[2], this);
                if (p[0] != p[1] && p[1] != p[2] && p[2] != p[0]) {
                    faces.push_back(tri);
                }
                if (!hasGivenNormal) {
                    for (int i = 0; i < 3; ++i) {
                        ++count_normal[p[i]];
                        normal[p[i]] += tri->getLocal_normal();
                    }
                    ++count_face;
                }
                break;
            }
            default:
                break;
        }
    }
    if (!hasGivenNormal){
        for (int i = 0; i < normal.size(); ++i){
            normal[i] /= (float)count_normal[i];
            normal[i].normalize();
        }
    }
    bbox = AABB("mesh bbox", bottom_left, upper_right);
//    std::cout << bottom_left << ", " << upper_right << std::endl;
}

MeshTriangle::MeshTriangle(const Material &material, const string &name, int p0, int p1, int p2, const Mesh *mesh) : Object(
        material, name), p0(p0), p1(p1), p2(p2), mesh(mesh) {

    VecF point0 = mesh->points[p0];
    VecF point1 = mesh->points[p1];
    VecF point2 = mesh->points[p2];
    local_normal = (point1 - point0).cross(point2 - point0);
    local_normal.normalize();
}

IntersectResult Mesh::intersect(const Ray &ray) const {
    IntersectResult result = bbox.intersect(ray);
    if (result.result == MISS) return result;
    result = IntersectResult();
    for (ObjectList::const_iterator iter = faces.begin(); iter != faces.end(); ++iter){
        IntersectResult temp_result = (*iter)->intersect(ray);
        if (temp_result.result != MISS) {
            if (temp_result.dist < result.dist) {
                result = temp_result;
            }
        }
    }
    return result;
}

Color Mesh::getColor(const VecF &pos) const {
    std::cerr << "Mesh should not have any color" << std::endl;
    exit(1);
}
