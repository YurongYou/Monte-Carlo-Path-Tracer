//
// Created by Ficos on 26/03/2017.
//

#include "Scene.h"

void Scene::initScene() {
    if (objects != NULL) {
        delete[] objects;
    }
    objects = new Object *[100];
    // floor
    objects[0] = new Plane(Material(Color(1.0f, 1.0f, 1.0f), 0.0, 1.0f), "floor", false, VecF(0, 1, 0), -4.4f);
    // big sphere
    objects[1] = new Sphere(Material(Color(0.7f, 0.7f, 0.7f), 0.6f, 0.0f), "big sphere", false, VecF(1, -0.8f, 3),
                            2.5f);
//    // small sphere
    objects[2] = new Sphere(Material(Color(0.7f, 0.7f, 1.0f), 0.1f, 1000000.0f), "small sphere", false,
                            VecF(-5.5f, -0.5f, 7), 2.5f);
//    // light source 1, white light
    objects[3] = new Sphere(Material(RED), "light 1", true, VecF(0, 5, 5), 0.1f);
//    // light source 1, white light
    objects[4] = new Sphere(Material(BLUE), "light 2", true, VecF(-10.0f, 5.0f, 5), 0.1f);
    num_objects = 5;
}