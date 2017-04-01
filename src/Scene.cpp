//
// Created by Ficos on 26/03/2017.
//

#include "Scene.h"

void Scene::initScene() {
    if (!object_list.empty()) object_list.clear();

    Material plane_material = Material();
    plane_material.setDiffusion(1.0f);
    plane_material.setReflection(0.0f);
    plane_material.setSpecular(0.0f);

    Material sphere_material = Material();
    Material light_material = Material();

    // floor
    plane_material.setColor( WHITE );
    object_list.push_back(new Plane(plane_material, "floor", false, VecF(0, 1, 0), -4.0f));
//
    // walls
//    plane_material.setColor( GREEN );
//
//    object_list.push_back(new Plane(plane_material, "wall", false, VecF(1, 0, 0), -4.0f));

//    // ceiling
//    plane_material.setColor( WHITE );
//    object_list.push_back(new Plane(plane_material, "ceiling", false, VecF(0, -1, 0), -7.4f));


//    for ( int x = 0; x < 8; x++ )
//        for ( int y = 0; y < 7; y++ ) {
//            sphere_material.setReflection(0.0f);
//            sphere_material.setRefraction(0.0f);
//            sphere_material.setDiffusion(0.6f);
//            sphere_material.setSpecular(0.6f);
//            sphere_material.setColor(Color( 0.3f, 1.0f, 0.4f ));
//            object_list.push_back(new Sphere(sphere_material, "grid sphere", false, VecF(-4.5f + x * 1.5f, -4.3f + y * 1.5f, 10), 0.3f));
//        }
//
//    sphere_material.clear();
//    sphere_material.setColor(WHITE);
//    sphere_material.setRefraction_index(1.3f);
//    sphere_material.setRefraction(0.8f);
//    sphere_material.setReflection(0.0f);
//    object_list.push_back(new Sphere(sphere_material, "transparent sphere", false, VecF(3.0f, 0.6f, 4), 3.0f));
//
//    sphere_material.clear();
//    sphere_material.setColor(WHITE);
//    sphere_material.setRefraction(0.0f);
//    sphere_material.setReflection(0.0f);
//    sphere_material.setDiffusion(1.0f);
//    object_list.push_back(new Sphere(sphere_material, "small sphere", false, VecF(0.0f, -1.0f, 7), 1.0f));

//
    sphere_material.clear();
    sphere_material.setColor(GREEN);
    sphere_material.setRefraction(1.0f);
    sphere_material.setRefraction_index(1.3f);
    sphere_material.setReflection(0.0f);
    sphere_material.setDiffusion(0.0f);
    object_list.push_back(new Sphere(sphere_material, "small sphere", false, VecF(-2.0f, 0.0f, 7), 2.0f));



    // light source 1
    light_material.setColor(Color( 0.8f, 0.8f, 0.8f ));
    object_list.push_back(new Sphere(light_material, "light 1", true, VecF(5, 5, 5), 1.0f));
    // light source 2
//    light_material.setColor(Color( 0.6f, 0.6f, 0.8f ));
//    object_list.push_back(new Sphere(light_material, "light 2", true, VecF(-3, 5, 1), 1.0f));
}