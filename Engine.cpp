//
// Created by Ficos on 26/03/2017.
//

#include <fstream>
#include <cassert>
#include "Engine.h"
#include "Ray.h"

using namespace std;
Engine::Engine(int image_width, int image_height, float world_x1, float world_y1, float world_x2, float world_y2)
        : image_width(image_width), image_height(image_height), world_x1(world_x1), world_y1(world_y1),
          world_x2(world_x2), world_y2(world_y2) {
    scene = new Scene();
    w_dx = (world_x2 - world_x1) / image_width;
    w_dy = (world_y2 - world_y1) / image_height;
}

Engine::~Engine() {
    delete scene;
}

Object* Engine::rayTrace(const Ray &ray, Color &color, int depth) {
    if (depth > TRACEDEPTH) return NULL;
    float min_dist = INFDIST;
    float dist = 0;
    int result;
    int temp_result;
    VecF hit_point;
    Object* hit_object = NULL;

    for (int i = 0; i < scene->getNum_objects(); ++i) {
        Object* obj = scene->getObjects(i);
        temp_result = obj->intersect(ray, dist);
        if (temp_result){
            if (dist < min_dist){
                min_dist = dist;
                result = temp_result;
                hit_object = obj;
            }
        }
    }
    if (!hit_object) return NULL;
    hit_point = ray.getOrigin() + ray.getDirection() * min_dist;
    if (hit_object->isLight()){
        color = hit_object->getColor(hit_point);
    } else {
        for (int j = 0; j < scene->getNum_objects(); ++j) {
            Object* temp_obj = scene->getObjects(j);
            if (temp_obj->isLight()){
                float shade = 1.0f;
                if (dynamic_cast<Sphere*>(temp_obj)){
                    VecF L = dynamic_cast<Sphere*>(temp_obj)->getCenter() - hit_point;
                    float t_dist = L.length();
                    float origin_dist = t_dist;
                    L.normalize();
                    // check if there are other surfaces blocking the light
                    Ray r = Ray( hit_point + L * EPSILON, L );
                    for (int i = 0; i < scene->getNum_objects(); ++i) {
                        Object* temp_obj_shade = scene->getObjects(i);
                        if ((temp_obj_shade != temp_obj) && temp_obj_shade->intersect(r, t_dist) && (t_dist < origin_dist)){
                            shade = 0;
                            break;
                        }
                    }
                    VecF N = hit_object->getNormal(hit_point);
                    if (hit_object->getMaterial().getDifussion() > 0){
                        float dot = N.dot(L);
//                        if (shade > 0 && dot < 0){
//                            std::cout << "error " << dot <<  std::endl;
//                        }
                        if (dot > 0){
                            float diffusion = dot * hit_object->getMaterial().getDifussion() * shade;
                            color += diffusion * hit_object->getMaterial().getColor() * temp_obj->getMaterial().getColor();
                        }
                    }
                }

            }
        }
    }
    return hit_object;
}

void Engine::render() {
    scene->initScene();
    canvas = new Color[image_height * image_width];
    VecF view_point(0, 0, -3);
    float x_pos = world_x1, y_pos = world_y1;
    for (int y = 0; y < image_height; ++y, y_pos += w_dy) {
        x_pos = world_x1;
        for (int x = 0; x < image_width; ++x, x_pos += w_dx) {
            Color& nowColor = canvas[y * image_width + x];
            VecF dir = VecF(x_pos, y_pos, 0) - view_point;
            dir.normalize();
            Object* obj = rayTrace(Ray(view_point, dir), nowColor, 1);
        }
    }
    std::ofstream ofs("./render_image.ppm", std::ios::out | std::ios::binary);
    ofs << "P6\n" << image_width << " " << image_height << "\n255\n";
    for (int i = 0; i < image_width * image_height; ++i) {
        ofs << (unsigned char)(std::min(float(1), canvas[i].x) * 255) <<
            (unsigned char)(std::min(float(1), canvas[i].y) * 255) <<
            (unsigned char)(std::min(float(1), canvas[i].z) * 255);
    }
    ofs.close();
    delete [] canvas;
}