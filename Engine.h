//
// Created by Ficos on 26/03/2017.
//

#ifndef RAYTRACING_ENGINE_H
#define RAYTRACING_ENGINE_H


#include "Scene.h"

class Engine {
public:
    Engine(int image_width, int image_height, float world_x1, float world_y1, float world_x2, float world_y2);

    ~Engine();

    Object *rayTrace(const Ray &ray, Color &color, int depth);

    void render();

private:
    Scene *scene;
    VecF *canvas;
    int image_width, image_height;
    float world_x1, world_y1, world_x2, world_y2, w_dx, w_dy;
};


#endif //RAYTRACING_ENGINE_H