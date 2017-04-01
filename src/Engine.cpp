//
// Created by Ficos on 26/03/2017.
//

#include <fstream>
#include <cassert>
#include "Engine.h"
#include "Ray.h"
#include <atomic>
#include <vector>
#include <queue>
#include <random>
#include "concurrentqueue.h"

using namespace std;

VecF Engine::uniformHemisphereSample() {
    float cosTheta = uni(gen);
    float sinTheta = sqrtf(1 - cosTheta * cosTheta);
    float phi = 2 * PI * uni(gen);
    return VecF(sinTheta * cosf(phi), cosTheta, sinTheta * sinf(phi));
}

Engine::Engine(int image_width, int image_height, float world_x1, float world_y1, float world_x2, float world_y2)
        : image_width(image_width), image_height(image_height), world_x1(world_x1), world_y1(world_y1),
          world_x2(world_x2), world_y2(world_y2) {
    scene = new Scene();
    w_dx = (world_x2 - world_x1) / image_width;
    w_dy = (world_y2 - world_y1) / image_height;
    gen = std::default_random_engine(static_cast<unsigned int>(time(NULL)));
    uni = std::uniform_real_distribution<float>(0, 1);
}

Engine::~Engine() {
    delete scene;
}

Engine::TraceResult Engine::rayTrace(const Ray &ray, const float& r_index, const int& depth) {
    TraceResult trace_rst = {.color = Color(0), .hit_object = NULL};
    if (depth > TRACEDEPTH) return trace_rst;
    IntersectResult min_dist_result = {.dist = INFDIST, .result = MISS};
    VecF hit_point;
    const ObjectList& obj_list = scene->getObejct_list();

    for (ObjectList::const_iterator iter = obj_list.begin(); iter != obj_list.end(); ++iter){
            IntersectResult temp_result = (*iter)->intersect(ray);
            if (temp_result.result != MISS) {
                if (temp_result.dist < min_dist_result.dist) {
                    min_dist_result = temp_result;
                    trace_rst.hit_object = (*iter);
                }
            }
    }
    if (!trace_rst.hit_object) return trace_rst;
    hit_point = ray.getOrigin() + ray.getDirection() * min_dist_result.dist;
    if (trace_rst.hit_object->isLight()) {
        trace_rst.color = trace_rst.hit_object->getColor(hit_point);
    } else {
        VecF N = trace_rst.hit_object->getNormal(hit_point);
        for (ObjectList::const_iterator iter = obj_list.begin(); iter != obj_list.end(); ++iter) {
            const Object *temp_obj = (*iter);
            if (temp_obj->isLight()) {
                const Sphere *point_light = dynamic_cast<const Sphere *>(temp_obj);
                if (point_light) {
                    float shade = 1.0f;
                    VecF L = point_light->getCenter() - hit_point;
                    float origin_dist = L.length();
                    L.normalize();
                    // check if there are other surfaces blocking the light
                    Ray r = Ray(hit_point + L * EPSILON, L);
                    for (ObjectList::const_iterator iter2 = obj_list.begin(); iter2 != obj_list.end(); ++iter2) {
                        Object const *temp_obj_shade = (*iter2);
                        IntersectResult shade_test_result = temp_obj_shade->intersect(r);
                        if ((temp_obj_shade != temp_obj) && shade_test_result.result != MISS &&
                            (shade_test_result.dist < origin_dist)) {
                            shade = 0;
                            break;
                        }
                    }
                    if (trace_rst.hit_object->getMaterial().getDiffusion() > 0) {
                        float dot = N.dot(L);
                        if (dot > 0) {
                            float diffusion = dot * trace_rst.hit_object->getMaterial().getDiffusion() * shade;
                            trace_rst.color += diffusion * trace_rst.hit_object->getMaterial().getColor() *
                                               temp_obj->getMaterial().getColor() / PI;
                        }
                    }
                    if (trace_rst.hit_object->getMaterial().getSpecular() > 0.0f) {
                        VecF V = ray.getDirection();
                        VecF R = L - 2.0f * L.dot(N) * N;
                        float dot = V.dot(R);
                        if (dot > 0) {
                            float specular = powf(dot, 20) * trace_rst.hit_object->getMaterial().getSpecular() * shade;
                            trace_rst.color += specular * temp_obj->getMaterial().getColor();
                        }
                    }
                }
            }
        }
        // indirect diffusion
        // N is the normal of the plane of hit point
        float indirect_diffusion = trace_rst.hit_object->getMaterial().getDiffusion();
        if (indirect_diffusion > 0){
            VecF Nx, Nz, Ny = N;
            if (fabs(Ny.x) > fabs(Ny.y)) Nx = VecF(Ny.z, 0, -Ny.x);
            else Nx = VecF(0, -Ny.z, Ny.y);
            Nx.normalize();
            Nz = Ny.cross(Nx);
            Nz.normalize();
            Color color_indirect_diffusion(0);
            for (int i = 0; i < DIFFSAMPLE; ++i) {
                VecF sample = uniformHemisphereSample();
                VecF sample_transformed = VecF(
                        sample.x * Nx.x + sample.y * Ny.x + sample.z * Nz.x,
                        sample.x * Nx.y + sample.y * Ny.y + sample.z * Nz.y,
                        sample.x * Nx.z + sample.y * Ny.z + sample.z * Nz.z
                );
                assert(sample_transformed.dot(Ny) > -EPSILON);
                TraceResult diffuse = rayTrace(Ray(hit_point + sample_transformed * EPSILON, sample_transformed), r_index, depth + 1);
                color_indirect_diffusion += sample.y * diffuse.color;
            }
            color_indirect_diffusion /= (DIFFSAMPLE * (1 / (2 * PI)));
            trace_rst.color += trace_rst.hit_object->getMaterial().getColor() * color_indirect_diffusion;
        }
        float reflection = trace_rst.hit_object->getMaterial().getReflection();
        if (reflection > 0){
            VecF N = trace_rst.hit_object->getNormal(hit_point);
            VecF R = ray.getDirection() - 2.0f * ray.getDirection().dot(N) * N;
            if (depth < TRACEDEPTH){
                Color rcol( 0, 0, 0 );
                Engine::TraceResult reflect_result = rayTrace(Ray(hit_point + R * EPSILON, R), r_index, depth + 1);
                trace_rst.color += reflection * reflect_result.color * trace_rst.hit_object->getColor(hit_point);
            }
        }
        float refraction = trace_rst.hit_object->getMaterial().getRefraction();
        if (refraction > 0.0f && depth < TRACEDEPTH){
            float index_next = trace_rst.hit_object->getMaterial().getRefraction_index();
            VecF N = trace_rst.hit_object->getNormal(hit_point);
            if (min_dist_result.result == INSIDE){
                index_next = 1.0;
                N *= -1.0f;
            }
            float n = r_index / index_next;
            float cosI = -N.dot(ray.getDirection());
            float cosT2 = 1.0f - n * n * (1.0f - cosI * cosI);
            if (cosT2 > 0.0f){
                VecF T = n * (ray.getDirection() + N * cosI) - N * sqrtf( cosT2 );
                Engine::TraceResult refract_result = rayTrace( Ray( hit_point + T * EPSILON, T ), index_next, depth + 1);
                trace_rst.color += refract_result.color * refraction;
            }
        }
    }
    return trace_rst;
}

void Engine::drawPicture(const Color* canvas) {
    std::ofstream ofs("./render_image.ppm", std::ios::out | std::ios::binary);
    ofs << "P6\n" << image_width << " " << image_height << "\n255\n";
    for (int i = 0; i < image_width * image_height; ++i) {
        ofs << (unsigned char) (std::min(float(1), canvas[i].x) * 255) <<
            (unsigned char) (std::min(float(1), canvas[i].y) * 255) <<
            (unsigned char) (std::min(float(1), canvas[i].z) * 255);
    }
    ofs.close();
    fprintf(stdout, "Drawing Finished\n");
}

void Engine::render(TraceConfig& config) {
    scene->initScene();
    Color* canvas = new Color[image_height * image_width];
    VecF view_point(0, 0, -5);
    int total = image_height * image_width;

    std::atomic<int> cnt_rendered;
    std::mutex mtx;
    moodycamel::ConcurrentQueue<std::pair<int, int>> q;
    cnt_rendered = 0;
    auto func = [&] {
        for (std::pair<int, int> item; ;) {
            if (!q.try_dequeue(item))
                return;

            int x = item.first, y = item.second;
            float sx = world_x1 + w_dx * x;
            float sy = world_y1 + w_dy * y;
            VecF dir = VecF(sx, sy, 0) - view_point;
            TraceResult pixel = rayTrace(Ray(view_point, dir), 1, 1);

            canvas[y * image_width + x] = pixel.color;
            ++cnt_rendered;
        }
    };

    std::vector<std::pair<int, int>> xys;
    for (int y = 0; y < image_height; ++y)
        for (int x = 0; x < image_width; ++x)
            xys.emplace_back(x, y);
    std::random_shuffle(xys.begin(), xys.end());
    for (const auto &xy : xys) q.enqueue(xy);

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::thread> workers;
    for (int i = 0; i < config.num_worker; ++i) workers.emplace_back(func);

    while (true){
        int cnt = cnt_rendered.load();
        auto now = std::chrono::high_resolution_clock::now();
        auto sec = (now - start).count() / 1e9;
        fprintf(stderr, "\rrendered %d/%d pixels using %d workers in %.3fs...", cnt, total, config.num_worker, sec);
        if (cnt == total) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    for (auto &worker : workers) worker.join();

    fprintf(stdout, "\nCalculation Finished\n");
    drawPicture(canvas);
    delete[] canvas;
}