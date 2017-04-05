//
// Created by Ficos on 26/03/2017.
//

#include <fstream>
#include <cassert>
#include "Engine.h"
#include "utility/Ray.h"
#include <atomic>
#include <vector>
#include <queue>
#include <random>
#include "concurrentqueue.h"

using namespace std;


Engine::Engine(int image_width, int image_height, float world_x1, float world_y1, float world_x2, float world_y2)
        : image_width(image_width), image_height(image_height), world_x1(world_x1), world_y1(world_y1),
          world_x2(world_x2), world_y2(world_y2) {
    scene = new Scene();
    w_dx = (world_x2 - world_x1) / image_width;
    w_dy = (world_y2 - world_y1) / image_height;
    gen = std::default_random_engine(static_cast<unsigned int>(time(NULL)));
    uni = std::uniform_real_distribution<float>(0, 1);
}

VecF Engine::uniformHemisphereSample() {
    float cosTheta = uni(gen);
    float sinTheta = sqrtf(1 - cosTheta * cosTheta);
    float phi = 2 * PI * uni(gen);
    return VecF(sinTheta * cosf(phi), cosTheta, sinTheta * sinf(phi));
}

Engine::~Engine() {
    delete scene;
}

Engine::TraceResult Engine::rayTrace(const Ray &ray, const float& r_index, const int& depth, const TraceConfig& config) {
    TraceResult trace_rst;
    trace_rst.color = BLACK;
    trace_rst.hit_object = NULL;
    trace_rst.dist = -1;
    if (depth > TRACEDEPTH) return trace_rst;
    IntersectResult min_dist_result;
    min_dist_result.dist = INFDIST;
    min_dist_result.result = MISS;
    const ObjectList& obj_list = scene->getObejct_list();
    const ObjectList& light_list = scene->getLight_list();

    for (ObjectList::const_iterator iter = obj_list.begin(); iter != obj_list.end(); ++iter){
        IntersectResult temp_result = (*iter)->intersect(ray);
        if (temp_result.result != MISS) {
            if (temp_result.dist < min_dist_result.dist) {
                min_dist_result = temp_result;
                trace_rst.hit_object = (*iter);
                trace_rst.dist = temp_result.dist;
            }
        }
    }
    if (!trace_rst.hit_object) return trace_rst;
    if (!trace_rst.hit_object->getMaterial().getEmission().isEqual(BLACK)){
        trace_rst.color = trace_rst.hit_object->getMaterial().getEmission();
        return trace_rst;
    }
    // old model
//    if (!hitLight and !trace_rst.hit_object) return trace_rst;
//    hit_point = ray.getOrigin() + ray.getDirection() * min_dist_result.dist;
//    if (hitLight) {
//        trace_rst.color = light_hit->getIntensity(hit_point, ray.getDirection());
//    } else {
//        VecF N = trace_rst.hit_object->getNormal(hit_point);
        // direct diffusion
//        for (LightList::const_iterator iter = light_list.begin(); iter != light_list.end(); ++iter) {
//            const Light *light = (*iter);
//            const PointLight *point_light = dynamic_cast<const PointLight *>(light);
//            if (point_light) {
//                float shade = 1.0f;
//                VecF L = point_light->getCenter() - hit_point;
//                float origin_dist = L.length();
//                L.normalize();
//                VecF hit_point_light = point_light->getCenter() - L * point_light->getShape().getRadius();
//                // check if there are other surfaces blocking the light
//                Ray r = Ray(hit_point + L * EPSILON, L);
//                for (ObjectList::const_iterator iter2 = obj_list.begin(); iter2 != obj_list.end(); ++iter2) {
//                    Object const *temp_obj_shade = (*iter2);
//                    IntersectResult shade_test_result = temp_obj_shade->intersect(r);
//                    if (shade_test_result.result != MISS && (shade_test_result.dist < origin_dist)) {
//                        shade = 0;
//                        break;
//                    }
//                }
//                if (trace_rst.hit_object->getMaterial().getDiffusion() > 0) {
//                    float dot = N.dot(L);
//                    if ((dynamic_cast<const Plane*>(trace_rst.hit_object) || dynamic_cast<const Triangle*>(trace_rst.hit_object)) && dot < 0){
//                        dot = -dot;
//                    }
//                    if (dot > 0) {
//                        float diffusion = dot * trace_rst.hit_object->getMaterial().getDiffusion() * shade;
//                        trace_rst.emission += diffusion * light->getIntensity(hit_point_light , L) * trace_rst.hit_object->getColor(hit_point);
//                    }
//                }
//                if (trace_rst.hit_object->getMaterial().getSpecular() > 0.0f) {
//                    VecF V = ray.getDirection();
//                    VecF R = L - 2.0f * L.dot(N) * N;
//                    float dot = V.dot(R);
//                    if (dot > 0) {
//                        float specular = powf(dot, 20) * trace_rst.hit_object->getMaterial().getSpecular() * shade;
//                        trace_rst.emission += specular * light->getIntensity(hit_point_light, L);
//                    }
//                }
//            }
//        }
        // indirect diffusion
        // N is the normal of the plane of hit point
//        float indirect_diffusion = trace_rst.hit_object->getMaterial().getDiffusion();
//        if (indirect_diffusion > 0){
//            VecF Nx, Nz, Ny = N;
//            if (fabs(Ny.x) > fabs(Ny.y)) Nx = VecF(Ny.z, 0, -Ny.x);
//            else Nx = VecF(0, -Ny.z, Ny.y);
//            Nx.normalize();
//            Nz = Ny.cross(Nx);
//            Nz.normalize();
//            Color color_indirect_diffusion(0);
////            for (int i = 0; i < DIFFSAMPLE; ++i) {
//                VecF sample = uniformHemisphereSample();
//                VecF sample_transformed = VecF(
//                        sample.x * Nx.x + sample.y * Ny.x + sample.z * Nz.x,
//                        sample.x * Nx.y + sample.y * Ny.y + sample.z * Nz.y,
//                        sample.x * Nx.z + sample.y * Ny.z + sample.z * Nz.z
//                );
//                assert(sample_transformed.dot(Ny) > -EPSILON);
//                TraceResult diffuse = rayTrace(Ray(hit_point + sample_transformed * EPSILON, sample_transformed), r_index, depth + 1);
//                color_indirect_diffusion += sample.y * diffuse.emission / (1 / (2 * PI));
////            }
////            color_indirect_diffusion /= (DIFFSAMPLE));
//            trace_rst.emission += indirect_diffusion * trace_rst.hit_object->getMaterial().getColor() * color_indirect_diffusion / PI;
//        }
//        float reflection = trace_rst.hit_object->getMaterial().getReflection();
//        if (reflection > 0){
//            VecF N = trace_rst.hit_object->getNormal(hit_point);
//            VecF R = ray.getDirection() - 2.0f * ray.getDirection().dot(N) * N;
//            if (depth < TRACEDEPTH){
//                Engine::TraceResult reflect_result = rayTrace(Ray(hit_point + R * EPSILON, R), r_index, depth + 1);
//                trace_rst.emission += reflection * reflect_result.emission;
//            }
//        }
//        float refraction = trace_rst.hit_object->getMaterial().getRefraction();
//        if (refraction > 0.0f && depth < TRACEDEPTH){
//            float index_next = trace_rst.hit_object->getMaterial().getRefraction_index();
//            VecF N = trace_rst.hit_object->getNormal(hit_point);
//            if (min_dist_result.result == INSIDE){
//                index_next = 1.0;
//                N *= -1.0f;
//            }
//            float n = r_index / index_next;
//            float cosI = -N.dot(ray.getDirection());
//            float cosT2 = 1.0f - n * n * (1.0f - cosI * cosI);
//            if (cosT2 > 0.0f){
//                VecF T = n * (ray.getDirection() + N * cosI) - N * sqrtf( cosT2 );
//                Engine::TraceResult refract_result = rayTrace( Ray( hit_point + T * EPSILON, T ), index_next, depth + 1);
//                float dist = refract_result.dist;
//                VecF beer = ( (-trace_rst.hit_object->getColor(hit_point) + 1) * (-0.02) * dist).exp();
//                trace_rst.emission += refraction * refract_result.emission * beer;
//            }
//        }
//    }
    VecF hit_point = ray.getOrigin() + ray.getDirection() * min_dist_result.dist;
    if (!config.isGlobalIllumination){
        // old model
        VecF N = trace_rst.hit_object->getNormal(hit_point);
        for (ObjectList::const_iterator iter = light_list.begin(); iter != light_list.end(); ++iter) {
            const Object *light = (*iter);
            const Sphere *point_light = dynamic_cast<const Sphere *>(light);
            if (point_light) {
                float shade = 1.0f;
                VecF L = point_light->getCenter() - hit_point;
                float origin_dist = L.length();
                L.normalize();
//                VecF hit_point_light = point_light->getCenter() - L * point_light->getRadius();
                // check if there are other surfaces blocking the light
                Ray r = Ray(hit_point + L * EPSILON, L);
                for (ObjectList::const_iterator iter2 = obj_list.begin(); iter2 != obj_list.end(); ++iter2) {
                    Object const *temp_obj_shade = (*iter2);
                    IntersectResult shade_test_result = temp_obj_shade->intersect(r);
                    if (shade_test_result.result != MISS && (shade_test_result.dist < origin_dist)) {
                        shade = 0;
                        break;
                    }
                }
                if (trace_rst.hit_object->getMaterial().getDiffuse_prob() > 0) {
                    float dot = N.dot(L);
                    if ((dynamic_cast<const Plane*>(trace_rst.hit_object) || dynamic_cast<const Triangle*>(trace_rst.hit_object)) && dot < 0){
                        dot = -dot;
                    }
                    if (dot > 0) {
                        float diffusion = dot * trace_rst.hit_object->getMaterial().getDiffuse_prob() * shade;
                        trace_rst.color += diffusion * light->getMaterial().getEmission() * trace_rst.hit_object->getMaterial().getIntrinsic_color();
                    }
                }
                if (trace_rst.hit_object->getMaterial().getReflection_prob() > 0.0f) {
                    VecF V = ray.getDirection();
                    VecF R = L - 2.0f * L.dot(N) * N;
                    float dot = V.dot(R);
                    if (dot > 0) {
                        float specular = powf(dot, 20) * trace_rst.hit_object->getMaterial().getReflection_prob() * shade;
                        trace_rst.color += specular * light->getMaterial().getEmission();
                    }
                }
            }
        }
    } else {
        // MCPT model
        float p = uni(gen);
        if (p < trace_rst.hit_object->getMaterial().getDiffuse_prob()){
            // do diffusion
            VecF Nx, Nz, Ny = trace_rst.hit_object->getNormal(hit_point);
            if (fabs(Ny.x) > fabs(Ny.y)) Nx = VecF(Ny.z, 0, -Ny.x);
            else Nx = VecF(0, -Ny.z, Ny.y);
            Nx.normalize();
            Nz = Ny.cross(Nx);
            Nz.normalize();
            VecF sample = uniformHemisphereSample();
            VecF sample_transformed = VecF(
                    sample.x * Nx.x + sample.y * Ny.x + sample.z * Nz.x,
                    sample.x * Nx.y + sample.y * Ny.y + sample.z * Nz.y,
                    sample.x * Nx.z + sample.y * Ny.z + sample.z * Nz.z
            );
            assert(sample_transformed.dot(Ny) > -EPSILON);
            TraceResult diffuse = rayTrace(Ray(hit_point + sample_transformed * EPSILON, sample_transformed), r_index, depth + 1, config);
            trace_rst.color = trace_rst.hit_object->getMaterial().getIntrinsic_color()
                              * diffuse.color / trace_rst.hit_object->getMaterial().getDiffuse_prob();
        } else if (p < trace_rst.hit_object->getMaterial().getDiffuse_prob()
                       + trace_rst.hit_object->getMaterial().getReflection_prob()){
            // do reflection
            if (depth < TRACEDEPTH){
                VecF N = trace_rst.hit_object->getNormal(hit_point);
                VecF R = ray.getDirection() - 2.0f * ray.getDirection().dot(N) * N;
                Engine::TraceResult reflect_result = rayTrace(Ray(hit_point + R * EPSILON, R), r_index, depth + 1, config);
                trace_rst.color = reflect_result.color / trace_rst.hit_object->getMaterial().getReflection_prob();
            }
        } else if  (p < trace_rst.hit_object->getMaterial().getDiffuse_prob()
                        + trace_rst.hit_object->getMaterial().getReflection_prob()
                        + trace_rst.hit_object->getMaterial().getRefraction_prob()){
            // do refraction
            if (depth < TRACEDEPTH){
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
                    Engine::TraceResult refract_result = rayTrace( Ray( hit_point + T * EPSILON, T ),
                                                                   index_next, depth + 1, config);
                    float dist = refract_result.dist;
                    VecF beer = ( (-trace_rst.hit_object->getColor(hit_point) + 1) * (-0.02) * dist).exp();
                    trace_rst.color = refract_result.color * beer / trace_rst.hit_object->getMaterial().getRefraction_prob();
                }
            }
        }
    }

    return trace_rst;
}

inline float clamp(float x){ return x<0 ? 0 : x>1 ? 1 : x; }
inline int toInt(float x){ return int(pow(clamp(x),1/2.2)*255+.5); }

void Engine::drawPicture(const Color* canvas) {
    std::ofstream ofs("./render_image.ppm", std::ios::out | std::ios::binary);
    ofs << "P6\n" << image_width << " " << image_height << "\n255\n";
    for (int i = 0; i < image_width * image_height; ++i) {
        ofs << (unsigned char) toInt(canvas[i].x) <<
            (unsigned char) toInt(canvas[i].y) <<
            (unsigned char) toInt(canvas[i].z);
    }
    ofs.close();
    fprintf(stdout, "Drawing Finished\n");
}

void Engine::render(TraceConfig& config) {
    scene->MeshTest("/Users/youyurong/CLionProjects/RayTracing/models/teapot.obj");
//    scene->CornellBox();
    Color* canvas = new Color[image_height * image_width];
    VecF view_point(0, 0, -5.0f);
    int total = image_height * image_width;

    std::atomic<int> cnt_rendered;
//    std::mutex mtx;
    moodycamel::ConcurrentQueue<std::pair<int, int>> q;
    cnt_rendered = 0;
    auto func = [&] {
        for (std::pair<int, int> item; ;) {
            if (!q.try_dequeue(item))
                return;

            int x = item.first, y = item.second;
            float sx = world_x1 + w_dx * x;
            float sy = world_y1 + w_dy * y;
            Color collect(0);
//            for (int i = 0; i < DIFFSAMPLE; ++i) {
//                float tweak_x = uni(gen) * w_dx - w_dx / 2;
//                float tweak_y = uni(gen) * w_dy - w_dy / 2;
//                VecF dir = VecF(sx + tweak_x, sy + tweak_y, 0) - view_point;
//                TraceResult pixel = rayTrace(Ray(view_point, dir), 1, 1);
//                collect += pixel.emission;
//            }
            float x_sub_range =  w_dx / (float)SAMPLE_SUB_PIX;
            float y_sub_range =  w_dy / (float)SAMPLE_SUB_PIX;
            for (int i = 0; i < SAMPLE_SUB_PIX; ++i) {
                for (int j = 0; j < SAMPLE_SUB_PIX; ++j) {
                    for (int k = 0; k < SAMPLENUM; ++k) {
                        float tweak_x = uni(gen) * x_sub_range;
                        float tweak_y = uni(gen) * y_sub_range;
                        VecF dir = VecF(sx + i * x_sub_range + tweak_x, sy + j * y_sub_range + tweak_y, 0) - view_point;
                        TraceResult pixel = rayTrace(Ray(view_point, dir), 1, 1, config);
                        collect += pixel.color / (float)(SAMPLE_SUB_PIX * SAMPLE_SUB_PIX * SAMPLENUM);
                    }
                }
            }
            canvas[y * image_width + x] = collect;
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