//
// Created by Ficos on 26/03/2017.
//

#include "Scene.h"

void Scene::initScene() {
    if (!object_list.empty()) object_list.clear();
    if (!light_list.empty()) light_list.clear();

    Material plane_material = Material();
//    plane_material.setDiffusion(1.0f);
//    plane_material.setReflection(0.0f);
//    plane_material.setSpecular(0.0f);

    Material light_material = Material();

    // floor

    float high = 3;
    float low = -2;
    float back = 5;
    float left = -3;
    float right = 3;
    float x1 = left + 1, z1 = 0.5f, x2 = right - 1, z2 = back - 1;


    Material triangle_light = Material();
    triangle_light.setEmission(WHITE);

    object_list.push_back(new Triangle(triangle_light, "triangle1",
                                       VecF(x1, high, z1), VecF(x1, high, z2), VecF(x2, high, z1)));
    object_list.push_back(new Triangle(triangle_light, "triangle2",
                                       VecF(x2, high, z1), VecF(x1, high, z2), VecF(x2, high, z2)));
//     walls
    plane_material.setDiffuse_prob(1.0f);
    plane_material.setIntrinsic_color( WHITE * 0.4 );
    object_list.push_back(new Plane(plane_material, "floor", VecF(0, 1, 0), low));

    plane_material.setIntrinsic_color( GREEN );
    object_list.push_back(new Plane(plane_material, "left wall", VecF(1, 0, 0), left));

    plane_material.setIntrinsic_color( RED );
    object_list.push_back(new Plane(plane_material, "right wall", VecF(-1, 0, 0), -right));

    plane_material.setIntrinsic_color( WHITE );
    object_list.push_back(new Plane(plane_material, "back wall", VecF(0, 0, -1), -back));

    plane_material.setIntrinsic_color( GRAY );
    object_list.push_back(new Plane(plane_material, "ceiling", VecF(0, -1, 0), -high));

    Material sphere_material = Material();
    sphere_material.clear();
    sphere_material.setIntrinsic_color(WHITE);
    sphere_material.setRefraction_index(1.5f);
    sphere_material.setDiffuse_prob(0.0f);
    sphere_material.setRefraction_prob(0.0f);
    sphere_material.setReflection_prob(1.0f);
    float radius1 = 1.0f;
    object_list.push_back(new Sphere(sphere_material, "refractive sphere", VecF(left + radius1 + 1, low + radius1, back - radius1 - 1), radius1));


    sphere_material.clear();
    sphere_material.setIntrinsic_color(WHITE);
    sphere_material.setRefraction_index(1.5f);
    sphere_material.setDiffuse_prob(0.0f);
    sphere_material.setRefraction_prob(1.0f);
    sphere_material.setReflection_prob(0.00f);
    float radius2 = 1.0f;
    object_list.push_back(new Sphere(sphere_material, "transparent sphere", VecF(1.5f, -1.0f, 2.0f), radius2));
}