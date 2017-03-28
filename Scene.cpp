//
// Created by Ficos on 26/03/2017.
//

#include "Scene.h"

void Scene::initScene() {
    if (!object_list.empty()) object_list.clear();

    Material plane_material = Material();
    plane_material.setDiffusion(1.0f);
    plane_material.setReflection(0.0f);

    Material sphere_material = Material();
    Material light_material = Material();

    // floor
    plane_material.setColor(GRAY);
    object_list.push_back(new Plane(plane_material, "floor", false, VecF(0, 1, 0), -4.4f));

    // walls
    plane_material.setColor(BLUE);
    object_list.push_back(new Plane(plane_material, "wall", false, VecF(0, 0, -1), -20.0f));

    plane_material.setColor(RED);
    object_list.push_back(new Plane(plane_material, "wall", false, VecF(-1, 0, 0), -20.0f));

    plane_material.setColor(GREEN);
    object_list.push_back(new Plane(plane_material, "wall", false, VecF(1, 0, 0), -12.0f));


    // big sphere
    sphere_material.setReflection(0.0f);
    sphere_material.setRefraction(0.8f);
    sphere_material.setRefraction_index(1.3f);
    sphere_material.setColor(WHITE);
    object_list.push_back(new Sphere(sphere_material, "transparent", false, VecF(-5.0f, -0.5f, 3), 1.0f));

    // small sphere
    sphere_material.setReflection(0.0f);
    sphere_material.setRefraction(0.0f);
    sphere_material.setDiffusion(0.7f);
    sphere_material.setColor(WHITE);
    object_list.push_back(new Sphere(sphere_material, "small sphere", false, VecF(-5.5f, -0.5f, 7), 2.5f));

//    // light source 1, white light
    object_list.push_back(new Sphere(light_material, "light 1", true, VecF(0, 20, -10), 0.1f));

//    // light source 1, white light
}