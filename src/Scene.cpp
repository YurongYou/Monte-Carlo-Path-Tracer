//
// Created by Ficos on 26/03/2017.
//

#include <fstream>
#include <cstring>
#include "Scene.h"
#include "utility/ObjLoader.h"

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
    triangle_light.setEmission(WHITE);
    addObject(new Triangle(triangle_light, "triangle1",
                                       VecF(x1, high, z1), VecF(x1, high, z2), VecF(x2, high, z1)));
    addObject(new Triangle(triangle_light, "triangle2",
                                       VecF(x2, high, z1), VecF(x1, high, z2), VecF(x2, high, z2)));

    plane_material.setDiffuse_prob(1.0f);
    plane_material.setIntrinsic_color( WHITE * 0.2 );
    addObject(new Plane(plane_material, "floor", VecF(0, 1, 0), low));

    plane_material.setIntrinsic_color( GREEN );
    addObject(new Plane(plane_material, "left wall", VecF(1, 0, 0), left));

    plane_material.setIntrinsic_color( RED );
    addObject(new Plane(plane_material, "right wall", VecF(-1, 0, 0), -right));

    plane_material.setIntrinsic_color( WHITE );
    addObject(new Plane(plane_material, "back wall", VecF(0, 0, -1), -back));

    plane_material.setIntrinsic_color( WHITE * 0.8 );
    addObject(new Plane(plane_material, "ceiling", VecF(0, -1, 0), -high - 0.05f));

    Material sphere_material = Material();
    sphere_material.clear();
    sphere_material.setIntrinsic_color(WHITE);
    sphere_material.setRefraction_index(1.5f);
    sphere_material.setDiffuse_prob(0.0f);
    sphere_material.setRefraction_prob(0.0f);
    sphere_material.setReflection_prob(1.0f);
    sphere_material.setBase_reflection_rate(0.8f);
    float radius1 = 1.0f;
    addObject(new Sphere(sphere_material, "refractive sphere",
                         VecF(left + radius1 + 1, low + radius1, back - radius1 - 1), radius1));
//
//
    sphere_material.clear();
    sphere_material.setIntrinsic_color(WHITE);
    sphere_material.setDiffuse_prob(0.0f);
    sphere_material.setRefraction_prob(0.9f);
    sphere_material.setReflection_prob(0.1f);
    float radius2 = 1.0f;
    addObject(new Sphere(sphere_material, "transparent sphere", VecF(1.7f, -1.0f, 1.0f), radius2));
}

void Scene::castTest(std::string file){
    if (!object_list.empty() || !light_list.empty()) {
        object_list.clear();
        light_list.clear();
        mesh.normal.clear();
        mesh.points.clear();
    }

    Material plane_material = Material();

    // floor

    float high = 5;
    float low = -2;
    float left = -3;
    float x1 = -2, z1 = -2.0f, x2 = 2, z2 = 0.0f;

    Material triangle_light = Material();
    triangle_light.setEmission(WHITE);
    addObject(new Triangle(triangle_light, "triangle1",
                           VecF(x1, high, z1), VecF(x1, high, z2), VecF(x2, high, z1)));
    addObject(new Triangle(triangle_light, "triangle2",
                           VecF(x2, high, z1), VecF(x1, high, z2), VecF(x2, high, z2)));


//    Material point_light = Material();
//    point_light.setEmission( WHITE);
//    addObject(new Sphere(point_light, "point light for test", VecF(2, high - 1.0f, -2.0f), 0.1f));
//    addObject(new Sphere(point_light, "point light for test", VecF(1, high - 1.0f, -2.0f), 0.1f));
//    addObject(new Sphere(point_light, "point light for test", VecF(-1, high - 1.0f, -2.0f), 0.1f));
//    addObject(new Sphere(point_light, "point light for test", VecF(-2, high - 1.0f, -2.0f), 0.1f));
//     walls
    plane_material.setDiffuse_prob(1.0f);
    plane_material.setIntrinsic_color( WHITE * 0.2 );
    addObject(new Plane(plane_material, "floor", VecF(0, 1, 0), low));

//    Material sphere_material = Material();
//    sphere_material.clear();
//    sphere_material.setIntrinsic_color(WHITE);
//    sphere_material.setRefraction_index(1.5f);
//    sphere_material.setDiffuse_prob(1.0f);
//    sphere_material.setRefraction_prob(0.0f);
//    sphere_material.setReflection_prob(0.0f);
//    sphere_material.setBase_reflection_rate(0.8f);
//    float radius1 = 1.0f;
//    addObject(new Sphere(sphere_material, "refractive sphere",
//                         VecF(0,0,4), radius1));
    ObjLoader::loadObj(file, *this);
}

void Scene::MixTest(std::string file) {
    if (!object_list.empty() || !light_list.empty()) {
        object_list.clear();
        light_list.clear();
        mesh.normal.clear();
        mesh.points.clear();
    }

    float high = 10;
    float low = 0;
    float back = 4;
    float left = -3 + 2 - 3;
    float right = 3 + 2 + 3;
    float x1 = left + 2.0f, z1 = -1.0f, x2 = right - 2.0f, z2 = 2.0f;

    Material plane_material = Material();
    Material triangle_light = Material();
    triangle_light.setEmission(WHITE);
    addObject(new Triangle(triangle_light, "triangle1",
                           VecF(x1, high, z1), VecF(x1, high, z2), VecF(x2, high, z1)));
    addObject(new Triangle(triangle_light, "triangle2",
                           VecF(x2, high, z1), VecF(x1, high, z2), VecF(x2, high, z2)));

    plane_material.setDiffuse_prob(1.0f);
    plane_material.setIntrinsic_color( WHITE * 0.4 );
    addObject(new Plane(plane_material, "floor", VecF(0, 1, 0), low));

    plane_material.setIntrinsic_color( GREEN );
    addObject(new Plane(plane_material, "left wall", VecF(1, 0, 0), left));

    plane_material.setIntrinsic_color( RED );
    addObject(new Plane(plane_material, "right wall", VecF(-1, 0, 0), -right));

    plane_material.setIntrinsic_color( WHITE );
    addObject(new Plane(plane_material, "back wall", VecF(0, 0, -1), -back));

    plane_material.setIntrinsic_color( WHITE * 0.8 );
    addObject(new Plane(plane_material, "ceiling", VecF(0, -1, 0), -high - 0.05f));

//    float high = 10;
//    float low = 0;
//    float left = -3;
//    float x1 = -2, z1 = -4.0f, x2 = 2, z2 = -1.0f;
//
//    Material triangle_light = Material();
//    triangle_light.setEmission(WHITE);
//    addObject(new Triangle(triangle_light, "triangle1",
//                           VecF(x1, high, z1), VecF(x1, high, z2), VecF(x2, high, z1)));
//    addObject(new Triangle(triangle_light, "triangle2",
//                           VecF(x2, high, z1), VecF(x1, high, z2), VecF(x2, high, z2)));

//    Material point_light = Material();
//    point_light.setEmission( WHITE * 0.3);
//    addObject(new Sphere(point_light, "point light for test", VecF(2 + 2, 10, -5.0f), 0.1f));
//    addObject(new Sphere(point_light, "point light for test", VecF(2 - 2, 10, -5.0f), 0.1f));
//    addObject(new Sphere(point_light, "point light for test", VecF(2 + 1, 10, -5.0f), 0.1f));
//    addObject(new Sphere(point_light, "point light for test", VecF(2 - 1, 10, -5.0f), 0.1f));
//    addObject(new Sphere(point_light, "point light for test", VecF(2, 10, -5.0f), 0.1f));

//    plane_material.setKs(0.8f);
//    plane_material.setIntrinsic_color( WHITE * 0.5 );
//    addObject(new Plane(plane_material, "floor", VecF(0, 1, 0), low));

    Material sphere_material = Material();
    sphere_material.clear();
    sphere_material.setIntrinsic_color(WHITE);
    sphere_material.setDiffuse_prob(0.0f);
    sphere_material.setRefraction_index(1.33f);
    sphere_material.setRefraction_prob(0.6f);
    sphere_material.setReflection_prob(0.4f);
    float radius1 = 1.6f;
    addObject(new Sphere(sphere_material, "diffuse sphere",
                         VecF(5, low + radius1, -2), radius1));

//    sphere_material.clear();
//    sphere_material.setDiffuse_prob(1.0f);
//    sphere_material.setKs(0.8f);
//    sphere_material.setIntrinsic_color( CYAN );
////    sphere_material.setReflection_prob(0.5f);
//    float radius2 = 2.0f;
//    addObject(new Sphere(sphere_material, "diffuse sphere",
//                         VecF(0, low + radius2, 0), radius2));


    ObjLoader::loadObj(file, *this);
}



void Scene::MeshTest(std::string file) {
    if (!object_list.empty() || !light_list.empty()) {
        object_list.clear();
        light_list.clear();
        mesh.normal.clear();
        mesh.points.clear();
    }
    ObjLoader::loadObj(file, *this);
    float x1 = -1.0f, z1 = 0.5f, x2 = 1.0f, z2 = 2.0f;
    float high = 3;
    Material triangle_light = Material();
    triangle_light.setEmission(WHITE * 6);
    addObject(new Triangle(triangle_light, "triangle1",
                           VecF(x1, high, z1), VecF(x1, high, z2), VecF(x2, high, z1)));
    addObject(new Triangle(triangle_light, "triangle2",
                           VecF(x2, high, z1), VecF(x1, high, z2), VecF(x2, high, z2)));
    Material plane_material = Material();
    plane_material.setDiffuse_prob(1.0f);
    plane_material.setIntrinsic_color( WHITE * 0.6 );
    addObject(new Plane(plane_material, "floor", VecF(0, 1, 0), -1.0f));
}
