//
// Created by Ficos on 26/03/2017.
//

#include <fstream>
#include "Scene.h"

#define LINE_MAX_LEN 1000
void Scene::CornellBox(){
    if (!object_list.empty()) object_list.clear();

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
    object_list.push_back(new Triangle(triangle_light, "triangle1",
                                       VecF(x1, high, z1), VecF(x1, high, z2), VecF(x2, high, z1)));
    object_list.push_back(new Triangle(triangle_light, "triangle2",
                                       VecF(x2, high, z1), VecF(x1, high, z2), VecF(x2, high, z2)));
//     walls
    plane_material.setDiffuse_prob(1.0f);
    plane_material.setIntrinsic_color( WHITE * 0.6 );
    object_list.push_back(new Plane(plane_material, "floor", VecF(0, 1, 0), low));

    plane_material.setIntrinsic_color( GREEN );
    object_list.push_back(new Plane(plane_material, "left wall", VecF(1, 0, 0), left));

    plane_material.setIntrinsic_color( RED );
    object_list.push_back(new Plane(plane_material, "right wall", VecF(-1, 0, 0), -right));

    plane_material.setIntrinsic_color( WHITE );
    object_list.push_back(new Plane(plane_material, "back wall", VecF(0, 0, -1), -back));

    plane_material.setIntrinsic_color( WHITE * 0.8 );
    object_list.push_back(new Plane(plane_material, "ceiling", VecF(0, -1, 0), -high - 0.05f));

    Material sphere_material = Material();
    sphere_material.clear();
    sphere_material.setIntrinsic_color(WHITE);
    sphere_material.setRefraction_index(1.5f);
    sphere_material.setDiffuse_prob(0.0f);
    sphere_material.setRefraction_prob(0.0f);
    sphere_material.setReflection_prob(1.0f);
    sphere_material.setBase_reflection_rate(0.8f);
    float radius1 = 1.0f;
    object_list.push_back(new Sphere(sphere_material, "refractive sphere", VecF(left + radius1 + 1, low + radius1, back - radius1 - 1), radius1));


    sphere_material.clear();
    sphere_material.setIntrinsic_color(WHITE);
    sphere_material.setRefraction_index(1.5f);
    sphere_material.setDiffuse_prob(0.0f);
    sphere_material.setRefraction_prob(1.0f);
    sphere_material.setReflection_prob(0.0f);
    float radius2 = 1.0f;
    object_list.push_back(new Sphere(sphere_material, "transparent sphere", VecF(1.7f, -1.0f, 1.0f), radius2));

//    sphere_material.clear();
//    sphere_material.setIntrinsic_color(BLUE);
//    sphere_material.setDiffuse_prob(1.0f);
//    sphere_material.setRefraction_prob(0.0f);
//    sphere_material.setReflection_prob(0.0f);
//    float radius3 = 0.5f;
//    object_list.push_back(new Sphere(sphere_material, "transparent sphere", VecF(1.0f, 1.5f, 3.5f), radius3));
}

void Scene::MeshTest(std::string fname) {
//    if (!object_list.empty()) object_list.clear();
//    std::ifstream fin(fname);
//    if (!fin.is_open()) {
//        std::cerr << "file not found, exit." << std::endl;
//        exit(1);
//    }
//    static char input[LINE_MAX_LEN];
//    while (fin.getline(input, LINE_MAX_LEN, '\n')){
//        if (input[0] == '#' or strlen(input) <= 1)
//            continue;
//        switch (input[0]) {
//            case 'v':
//            {
//                if (input[1] == ' ') {	// v x y z
//                    float x, y, z;
//                    sscanf(input + 2, "%f %f %f", &x, &y, &z);
//                    mesh->add_vertex(Vec(x, y, z));
//                } else if (input[1] == 'n') {  // vn x y z
//                    //real_t x, y, z;
//                    //sscanf(input + 3, "%lf %lf %lf", &x, &y, &z);
//                    //mesh->set_norm(nnorm++, Vec(x, y, z));
//                    //don't use vn
//                } else {
//                    std::cerr << input << std::endl;
//                    std::cerr << "unrecognized format" << std::endl;
//                    exit(1);
//                }
//                break;
//            }
//            case 'f':
//            {				// XXX some model is 'f 1 2 3 4 5 6'
//                int x, y, z;
//                sscanf(input + 2, "%d %d %d", &x, &y, &z);
//                if (x != y && y != z && x != z) mesh->add_faceid(x - 1, y - 1, z - 1);
//                break;
//            }
//            default:
//                break;
//        }
//    }
}
