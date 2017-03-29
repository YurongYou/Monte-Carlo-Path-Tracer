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

const Object* Engine::rayTrace(const Ray &ray, Color &color, float& dist, const float& r_index, int depth) {
    if (depth > TRACEDEPTH) return NULL;
    float min_dist = INFDIST;
    int result = 1;
    int temp_result;
    VecF hit_point;
    const ObjectList& obj_list = scene->getObejct_list();
    const Object* hit_object = NULL;

    for (ObjectList::const_iterator iter = obj_list.begin(); iter != obj_list.end(); ++iter){
            temp_result = (*iter)->intersect(ray, dist);
            if (temp_result) {
                if (dist < min_dist) {
                    min_dist = dist;
                    result = temp_result;
                    hit_object = (*iter);
                }
            }
    }

    if (!hit_object) return NULL;
    hit_point = ray.getOrigin() + ray.getDirection() * min_dist;
    if (hit_object->isLight()) {
        color = hit_object->getColor(hit_point);
    } else {
        for (ObjectList::const_iterator iter = obj_list.begin(); iter != obj_list.end(); ++iter) {
            const Object* temp_obj = (*iter);
            if (temp_obj->isLight()) {
                const Sphere *point_light = dynamic_cast<const Sphere *>(temp_obj);
                if (point_light) {
                    float shade = 1.0f;
                    VecF L = point_light->getCenter() - hit_point;
                    float t_dist = L.length();
                    float origin_dist = t_dist;
                    L.normalize();
                    // check if there are other surfaces blocking the light
                    Ray r = Ray(hit_point + L * EPSILON, L);
                    for (ObjectList::const_iterator iter2 = obj_list.begin(); iter2 != obj_list.end(); ++iter2) {
                        Object const *temp_obj_shade = (*iter2);
                        if ((temp_obj_shade != temp_obj) && temp_obj_shade->intersect(r, t_dist) &&
                            (t_dist < origin_dist)) {
                            shade = 0;
                            break;
                        }
                    }
                    VecF N = hit_object->getNormal(hit_point);
                    if (hit_object->getMaterial().getDiffusion() > 0) {
                        float dot = N.dot(L);
                        if (dot > 0) {
                            float diffusion = dot * hit_object->getMaterial().getDiffusion() * shade;
                            color += diffusion * hit_object->getMaterial().getColor() *
                                     temp_obj->getMaterial().getColor();
                        }
                    }
                    if (hit_object->getMaterial().getSpecular() > 0.0f) {
                        VecF V = ray.getDirection();
                        VecF R = L - 2.0f * L.dot(N) * N;
                        float dot = V.dot(R);
                        if (dot > 0) {
                            float specular = powf(dot, 20) * hit_object->getMaterial().getSpecular() * shade;
                            color += specular * temp_obj->getMaterial().getColor();
                        }
                    }
                }
            }
        }
        float reflection = hit_object->getMaterial().getReflection();
        if (reflection > 0){
            VecF N = hit_object->getNormal(hit_point);
            VecF R = ray.getDirection() - 2.0f * ray.getDirection().dot(N) * N;
            if (depth < TRACEDEPTH){
                Color rcol( 0, 0, 0 );
                float reflect_dist = 0;
                rayTrace(Ray(hit_point + R * EPSILON, R), rcol, reflect_dist, r_index, depth + 1);
                color += reflection * rcol * hit_object->getColor(hit_point);
            }
        }
        float refraction = hit_object->getMaterial().getRefraction();
        if (refraction > 0.0f && depth < TRACEDEPTH){
            float index_next = hit_object->getMaterial().getRefraction_index();
            if (result == INPRIM){
                // cout << hit_object->getMaterial().getRefraction_index() << endl;
                // assert(hit_object->getMaterial().getRefraction_index() > 1.0f);
                index_next = 1.0;
            }
            float n = r_index / index_next;
            // cout << n << endl;
            // cout << hit_object->getName() << ", " << hit_object->getMaterial().getRefraction_index() << ", " << n << endl;
            VecF N = hit_object->getNormal(hit_point) * (float)result;
            float cosI = -N.dot(ray.getDirection());
            float cosT2 = 1.0f - n * n * (1.0f - cosI * cosI);
            float refract_dist = 0;
            if (cosT2 > 0.0f){
                // VecF T = (n * ray.getDirection()) + (n * cosI - sqrtf( cosT2 )) * N;
                VecF T = n * (ray.getDirection() + N * cosI) - N * sqrtf( cosT2 );
                Color rcol( 0, 0, 0 );
                rayTrace( Ray( hit_point + T * EPSILON, T ), rcol, refract_dist, index_next, depth + 1);
//                Color abosrbance = hit_object->getColor(hit_point) * 0.1f * -refract_dist;
                color += rcol * refraction;
            }
        }
    }
    return hit_object;
}

void Engine::render() {
    scene->initScene();
    canvas = new Color[image_height * image_width];
    VecF view_point(0, 0, -5);
    float x_pos = world_x1, y_pos = world_y1;
    int total = image_height * image_width;
    int count = 0;
    float dist = 0;
    const Object* last_obj = NULL;
    const Object* curr_obj = NULL;
    const Object* object_last_line[image_width];
    for (int y = 0; y < image_height; ++y, y_pos += w_dy) {
        x_pos = world_x1;
        for (int x = 0; x < image_width; ++x, x_pos += w_dx) {
            Color &nowColor = canvas[y * image_width + x];
            VecF dir = VecF(x_pos, y_pos, 0) - view_point;
            curr_obj = rayTrace(Ray(view_point, dir), nowColor, dist, 1, 1);
            if (curr_obj != last_obj || object_last_line[x] != last_obj){
                object_last_line[x] = curr_obj;
                last_obj = curr_obj;
                Color super_sampling(0);
                for ( int tx = -1; tx < 2; tx++ ) for ( int ty = -1; ty < 2; ty++ ){
                        VecF sup_dir = VecF(x_pos + w_dx * tx / 2.0f, y_pos + w_dy * ty / 2.0f, 0) - view_point;
                        float dist;
                        rayTrace(Ray(view_point, sup_dir), super_sampling, dist, 1.0f, 1);
                    }
                nowColor = super_sampling / 9;
            }

            ++count;
//            cout << "status: " << count << " / " << total << ", " << (float)count / total << "\n";
        }
    }
    std::ofstream ofs("./render_image.ppm", std::ios::out | std::ios::binary);
    ofs << "P6\n" << image_width << " " << image_height << "\n255\n";
    for (int i = 0; i < image_width * image_height; ++i) {
        ofs << (unsigned char) (std::min(float(1), canvas[i].x) * 255) <<
            (unsigned char) (std::min(float(1), canvas[i].y) * 255) <<
            (unsigned char) (std::min(float(1), canvas[i].z) * 255);
    }
    ofs.close();
    delete[] canvas;
}