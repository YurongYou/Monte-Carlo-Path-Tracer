//
// Created by Ficos on 26/03/2017.
//

#include <fstream>
#include <cstring>
#include "Scene.h"

#define LINE_MAX_LEN 1000
void Scene::CornellBox(){
    if (!object_list.empty() || !light_list.empty()) {
        object_list.clear();
        light_list.clear();
        mesh.normal.clear();
        mesh.points.clear();
    }

    Material plane_material = Material();

    // floor

    float high = 3;
    float low = -2;
    float back = 4;
    float left = -3;
    float right = 3;
    float x1 = left + 2.0f, z1 = 0.5f, x2 = right - 2.0f, z2 = 2.0f;


    Material triangle_light = Material();
    triangle_light.setEmission(WHITE * 6);
    addObeject(new Triangle(triangle_light, "triangle1",
                                       VecF(x1, high, z1), VecF(x1, high, z2), VecF(x2, high, z1)));
    addObeject(new Triangle(triangle_light, "triangle2",
                                       VecF(x2, high, z1), VecF(x1, high, z2), VecF(x2, high, z2)));
//     walls
    plane_material.setDiffuse_prob(1.0f);
    plane_material.setIntrinsic_color( WHITE * 0.6 );
    addObeject(new Plane(plane_material, "floor", VecF(0, 1, 0), low));

    plane_material.setIntrinsic_color( GREEN );
    addObeject(new Plane(plane_material, "left wall", VecF(1, 0, 0), left));

    plane_material.setIntrinsic_color( RED );
    addObeject(new Plane(plane_material, "right wall", VecF(-1, 0, 0), -right));

    plane_material.setIntrinsic_color( WHITE );
    addObeject(new Plane(plane_material, "back wall", VecF(0, 0, -1), -back));

    plane_material.setIntrinsic_color( WHITE * 0.8 );
    addObeject(new Plane(plane_material, "ceiling", VecF(0, -1, 0), -high - 0.05f));

    Material sphere_material = Material();
    sphere_material.clear();
    sphere_material.setIntrinsic_color(WHITE);
    sphere_material.setRefraction_index(1.5f);
    sphere_material.setDiffuse_prob(0.0f);
    sphere_material.setRefraction_prob(0.0f);
    sphere_material.setReflection_prob(1.0f);
    sphere_material.setBase_reflection_rate(0.8f);
    float radius1 = 1.0f;
    addObeject(new Sphere(sphere_material, "refractive sphere", VecF(left + radius1 + 1, low + radius1, back - radius1 - 1), radius1));


    sphere_material.clear();
    sphere_material.setIntrinsic_color(WHITE);
    sphere_material.setRefraction_index(1.5f);
    sphere_material.setDiffuse_prob(0.0f);
    sphere_material.setRefraction_prob(0.9f);
    sphere_material.setReflection_prob(0.1f);
    float radius2 = 1.0f;
    addObeject(new Sphere(sphere_material, "transparent sphere", VecF(1.7f, -1.0f, 1.0f), radius2));
}

void Scene::MeshTest(std::string fname) {
    if (!object_list.empty() || !light_list.empty()) {
        object_list.clear();
        light_list.clear();
        mesh.normal.clear();
        mesh.points.clear();
    }
    std::ifstream fin(fname);
    if (!fin.is_open()) {
        std::cerr << "file not found, exit." << std::endl;
        exit(1);
    }
    char input[LINE_MAX_LEN];
    int count_face = 0;
    std::vector<int> count_normal;
    Material triangle_material = Material();
    triangle_material.setDiffuse_prob(1.0f);
    triangle_material.setIntrinsic_color( GREEN );
    // null point (to deal with the 1-base problem)
    mesh.addVertex(VecF(0, 0, 0));
    count_normal.push_back(0);
    mesh.normal.push_back(VecF(0));
    while (fin.getline(input, LINE_MAX_LEN, '\n')) {
        if (input[0] == '#' or strlen(input) <= 1)
            continue;
        switch (input[0]){
            case 'v': {
                if (input[1] == ' ') {
                    // v x y z
                    float x, y, z;
                    sscanf(input + 2, "%f %f %f", &x, &y, &z);
//                    std::cout << "add vertex: " << VecF(x, y, z) << std::endl;
                    mesh.addVertex(VecF(x, y, z));
                    count_normal.push_back(0);
                    mesh.normal.push_back(VecF(0));
                } else if (input[1] == 'n') {
                    // do nothing
                    // we don't use vn here
                } else {
                    std::cerr << input << std::endl;
                    std::cerr << "unrecognized format" << std::endl;
                    exit(1);
                }
                break;
            }
            case 'f': {
                int p[3];
                sscanf(input + 2, "%d %d %d", &p[0], &p[1], &p[2]);
                MeshTriangle *tri = new MeshTriangle(triangle_material, "triangle_" + std::to_string(count_face),
                                                     p[0], p[1], p[2], &mesh);
                if (p[0] != p[1] && p[1] != p[2] && p[2] != p[0]) {
                    addObeject(tri);
                }
                for (int i = 0; i < 3; ++i) {
                    ++count_normal[p[i]];
                    mesh.normal[p[i]] += tri->getLocal_normal();
                }
                ++count_face;
                break;
            }
            default:
                break;
        }
    }
    for (int j = 0; j < mesh.normal.size(); ++j) {
        mesh.normal[j] /= (float)count_normal[j];
    }

    float x1 = -1.0f, z1 = 0.5f, x2 = 1.0f, z2 = 2.0f;
    float high = 3;
    Material triangle_light = Material();
    triangle_light.setEmission(WHITE * 6);
    addObeject(new Triangle(triangle_light, "triangle1",
                                       VecF(x1, high, z1), VecF(x1, high, z2), VecF(x2, high, z1)));
    addObeject(new Triangle(triangle_light, "triangle2",
                                       VecF(x2, high, z1), VecF(x1, high, z2), VecF(x2, high, z2)));
    Material plane_material = Material();
    plane_material.setDiffuse_prob(1.0f);
    plane_material.setIntrinsic_color( WHITE * 0.6 );
    addObeject(new Plane(plane_material, "floor", VecF(0, 1, 0), -1.0f));
}
