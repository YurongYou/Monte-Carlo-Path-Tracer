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


Engine::Engine(int image_width, int image_height, const VecF& from, const VecF& to): image_width(image_width), image_height(image_height), from(from) {
    scene = new Scene();
    gen = std::default_random_engine(static_cast<unsigned int>(time(NULL)));
    uni = std::uniform_real_distribution<float>(0, 1);

    VecF temp_P1 = VecF( -4,  3, 5);
    VecF temp_P2 = VecF(  4,  3, 5);
    VecF temp_P4 = VecF( -4, -3, 5);

    VecF z_axis = to - from;
    z_axis.normalize();
    VecF up = VecF(0, 1, 0);
    VecF x_axis = up.cross(z_axis);
    VecF y_axis = z_axis.cross(x_axis);

    temp_P1 = temp_P1.x * x_axis + temp_P1.y * y_axis + temp_P1.z * z_axis + from;
    temp_P2 = temp_P2.x * x_axis + temp_P2.y * y_axis + temp_P2.z * z_axis + from;
    temp_P4 = temp_P4.x * x_axis + temp_P4.y * y_axis + temp_P4.z * z_axis + from;
    left_right = temp_P1;
    dx = (temp_P2 - temp_P1) / (float)image_width;
    dy = (temp_P4 - temp_P1) / (float)image_height;
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

Color Engine::castRay(const Object *obj, const Object *light, const Ray &ray, const VecF &ori, const VecF &N, const VecF &hit) const {
    const ObjectList& obj_list = scene->getObject_list();
//    VecF N = obj->getNormal(ori);
    float shade = 1.0f;
    VecF L = hit - ori;
    float origin_dist = L.length();
    L.normalize();
    Ray r = Ray(ori + L * EPSILON, L);
    Color collect(0);
    for (ObjectList::const_iterator iter2 = obj_list.begin(); iter2 != obj_list.end(); ++iter2) {
        Object const *temp_obj_shade = (*iter2);
        if (temp_obj_shade == light) continue;
        IntersectResult shade_test_result = temp_obj_shade->intersect(r);
        if (shade_test_result.result != MISS && (shade_test_result.dist < origin_dist)) {
            shade = 0;
            break;
        }
    }
    if (obj->getMaterial().getDiffuse_prob() > 0) {
        float dot = N.dot(L);
        if ((dynamic_cast<const Plane*>(obj) || dynamic_cast<const Triangle*>(obj) || dynamic_cast<const MeshTriangle*>(obj)) && dot < 0){
            dot = -dot;
        }
        if (dot > 0) {
            float diffusion = dot * obj->getMaterial().getDiffuse_prob() * shade;
//            if (dynamic_cast<const MeshTriangle*>(obj)) {
//                cout << dot << endl;
//            }
            collect += diffusion * light->getMaterial().getEmission() * obj->getColor(ori);
        }
    }
    if (obj->getMaterial().getKs() > 0.0f) {
        VecF V = ray.getDirection();
        VecF R = L - 2.0f * L.dot(N) * N;
        float dot = V.dot(R);
        if (dot > 0) {
            float specular = powf(dot, 20) * obj->getMaterial().getKs() * shade;
            collect += specular * light->getMaterial().getEmission();
        }
    }
    return collect;
}

Engine::TraceResult Engine::rayTrace(const Ray &ray, const float& r_index, const int& depth, const TraceConfig& config) {
    TraceResult trace_rst;
    trace_rst.color = BLACK;
    trace_rst.hit_object = NULL;
    trace_rst.dist = -1;
    if (depth > config.num_trace_depth) return trace_rst;
    IntersectResult min_dist_result;
    min_dist_result.dist = INFDIST;
    min_dist_result.result = MISS;
    const ObjectList& obj_list = scene->getObject_list();
    const ObjectList& light_list = scene->getLight_list();

    for (ObjectList::const_iterator iter = obj_list.begin(); iter != obj_list.end(); ++iter){
        IntersectResult temp_result = (*iter)->intersect(ray);
        if (temp_result.result != MISS) {
            if (temp_result.dist < min_dist_result.dist) {
                min_dist_result = temp_result;
            }
        }
    }
    trace_rst.hit_object = min_dist_result.hit_obj;
    trace_rst.dist = min_dist_result.dist;
    if (!trace_rst.hit_object) return trace_rst;
    if (!trace_rst.hit_object->getMaterial().getEmission().isEqual(BLACK)){
        trace_rst.color = trace_rst.hit_object->getMaterial().getEmission();
        return trace_rst;
    }

    VecF hit_point = ray.getOrigin() + ray.getDirection() * min_dist_result.dist;
    VecF N = min_dist_result.normal;
    if (config.illumination_type == RAY_TRACING_WITH_CAST || config.illumination_type == MIX){
        // old model
        // ray casting
        if (trace_rst.hit_object->getMaterial().getDiffuse_prob() > 0){
            if (config.illumination_type == RAY_TRACING_WITH_CAST || depth > 3) {
                for (ObjectList::const_iterator iter = light_list.begin(); iter != light_list.end(); ++iter) {
                    const Object *light = (*iter);
                    const Sphere *point_light = dynamic_cast<const Sphere *>(light);
                    const Triangle *triangle_light = dynamic_cast<const Triangle *>(light);
                    if (point_light) {
                        // check if there are other surfaces blocking the light
                        trace_rst.color += castRay(trace_rst.hit_object, point_light, ray, hit_point, N,
                                                   point_light->getCenter());
                    } else if (triangle_light) {
                        Color area_cast(0);
                        // ref: http://math.stackexchange.com/questions/18686/uniform-random-point-in-triangle
                        for (int i = 0; i < SAMPLE_AREA_LIGHT; ++i) {
                            float r1 = uni(gen);
                            float r2 = uni(gen);
                            VecF mix = triangle_light->getMixPoint(1 - sqrtf(r1), sqrtf(r1) * (1 - r2) );
                            area_cast += castRay(trace_rst.hit_object, triangle_light, ray, hit_point, N, mix);
                        }
                        area_cast /= SAMPLE_AREA_LIGHT;
                        trace_rst.color += area_cast;
                    }
                }
            } else {
                VecF Nx, Nz, Ny = N;
                if (fabs(Ny.x) > fabs(Ny.y)) Nx = VecF(Ny.z, 0, -Ny.x);
                else Nx = VecF(0, -Ny.z, Ny.y);
                Nx.normalize();
                Nz = Ny.cross(Nx);
                Nz.normalize();
                for (int i = 0; i < config.diffuse_sample_num; ++i) {
                    VecF sample = uniformHemisphereSample();
                    VecF sample_transformed = VecF(
                            sample.x * Nx.x + sample.y * Ny.x + sample.z * Nz.x,
                            sample.x * Nx.y + sample.y * Ny.y + sample.z * Nz.y,
                            sample.x * Nx.z + sample.y * Ny.z + sample.z * Nz.z
                    );
                    TraceResult diffuse = rayTrace(Ray(hit_point + sample_transformed * EPSILON, sample_transformed), r_index, depth + 1, config);
                    trace_rst.color += trace_rst.hit_object->getMaterial().getDiffuse_prob() *
                                        trace_rst.hit_object->getMaterial().getIntrinsic_color()
                                      * diffuse.color / config.diffuse_sample_num;
                }
            }
        }
        // reflection
        float reflection = trace_rst.hit_object->getMaterial().getReflection_prob();
        if (reflection > 0 && min_dist_result.result != INSIDE){
            VecF R = ray.getDirection() - 2.0f * ray.getDirection().dot(N) * N;
            if (depth < config.num_trace_depth){
                Engine::TraceResult reflect_result = rayTrace(Ray(hit_point + R * EPSILON, R), r_index, depth + 1, config);
                trace_rst.color += reflection * reflect_result.color;
            }
        }
        // refraction
        float refraction = trace_rst.hit_object->getMaterial().getRefraction_prob();
        if (refraction > 0.0f && depth < config.num_trace_depth){
            float index_next = trace_rst.hit_object->getMaterial().getRefraction_index();
            if (min_dist_result.result == INSIDE){
                index_next = 1.0;
//                N *= -1.0f;
            }
            float n = r_index / index_next;
            float cosI = -N.dot(ray.getDirection());
            float cosT2 = 1.0f - n * n * (1.0f - cosI * cosI);
            if (cosT2 > 0.0f){
                VecF T = n * (ray.getDirection() + N * cosI) - N * sqrtf( cosT2 );
                Engine::TraceResult refract_result = rayTrace( Ray( hit_point + T * EPSILON, T ), index_next, depth + 1, config);
                float dist = refract_result.dist;
                VecF beer = ( (-trace_rst.hit_object->getColor(hit_point) + 1) * (-0.02) * dist).exp();
                trace_rst.color += refraction * refract_result.color * beer;
//                cout << refract_result.color << endl;
            }
        }
    } else if (config.illumination_type == PURE_MCPT) {
        // MCPT model
        float p = uni(gen);
        if (p < trace_rst.hit_object->getMaterial().getDiffuse_prob()){
            // do diffusion
//            if (depth < 3) {
                VecF Nx, Nz, Ny = N;
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
                trace_rst.color = trace_rst.hit_object->getColor(hit_point)
                                  * diffuse.color / trace_rst.hit_object->getMaterial().getDiffuse_prob();
//            } else {
//                for (ObjectList::const_iterator iter = light_list.begin(); iter != light_list.end(); ++iter) {
//                    const Object *light = (*iter);
//                    const Sphere *point_light = dynamic_cast<const Sphere *>(light);
//                    const Triangle *triangle_light = dynamic_cast<const Triangle *>(light);
//                    if (point_light) {
//                        // check if there are other surfaces blocking the light
//                        trace_rst.color += castRay(trace_rst.hit_object, point_light, ray, hit_point, N,
//                                                   point_light->getCenter());
//                    } else if (triangle_light) {
//                        Color area_cast(0);
//                        for (int i = 0; i < SAMPLE_AREA_LIGHT; ++i) {
//                            float alpha = uni(gen);
//                            float beta = uni(gen) * (1 - alpha);
//                            VecF mix = triangle_light->getMixPoint(alpha, beta);
//                            area_cast += castRay(trace_rst.hit_object, triangle_light, ray, hit_point, N, mix) / SAMPLE_AREA_LIGHT;
//                        }
//                        trace_rst.color += area_cast;
//                    }
//                }
//            }
        } else if (p < trace_rst.hit_object->getMaterial().getDiffuse_prob()
                       + trace_rst.hit_object->getMaterial().getReflection_prob()
                   && min_dist_result.result != INSIDE){
            // do reflection
            if (depth < config.num_trace_depth){
                VecF R = ray.getDirection() - 2.0f * ray.getDirection().dot(N) * N;
                Engine::TraceResult reflect_result = rayTrace(Ray(hit_point + R * EPSILON, R), r_index, depth + 1, config);
                trace_rst.color = reflect_result.color / trace_rst.hit_object->getMaterial().getReflection_prob();
            }
        } else if  (p < trace_rst.hit_object->getMaterial().getDiffuse_prob()
                        + trace_rst.hit_object->getMaterial().getReflection_prob()
                        + trace_rst.hit_object->getMaterial().getRefraction_prob() || min_dist_result.result == INSIDE){
            // do refraction
            if (depth < config.num_trace_depth){
                float index_next = trace_rst.hit_object->getMaterial().getRefraction_index();
                if (min_dist_result.result == INSIDE){
                    index_next = 1.0;
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
    if (config.test == "twist_mesh"){
        scene->castTest("../models/twist.obj");
    } else if (config.test == "teapot_mesh"){
        scene->castTest("../models/teapot.obj");
    } else if (config.test == "mix_twist_mesh"){
        scene->MixTest("../models/twist.obj");
    } else if (config.test == "load_obj"){
        scene->ObjTest(config.obj);
    } else if (config.test == "simple"){
        scene->simpleTest();
    }
    else{
        scene->CornellBox();
    }
    Color* canvas = new Color[image_height * image_width];
    int total = image_height * image_width;

    std::atomic<int> cnt_rendered;
    moodycamel::ConcurrentQueue<std::pair<int, int>> q;

    const VecF x_sub_range =  dx / (float)config.sub_pixel;
    const VecF y_sub_range =  dy / (float)config.sub_pixel;

    cnt_rendered = 0;
    auto func = [&] {
        for (std::pair<int, int> item; ;) {
            if (!q.try_dequeue(item))
                return;

            int x = item.first, y = item.second;
            VecF target = left_right + dx * x + dy * y;

            Color collect(0);

            for (int i = 0; i < config.sub_pixel; ++i) {
                for (int j = 0; j < config.sub_pixel; ++j) {
                    for (int k = 0; k < config.sample_num; ++k) {
                        VecF tweak_x = uni(gen) * x_sub_range;
                        VecF tweak_y = uni(gen) * y_sub_range;
                        VecF dir = target + (float)i * x_sub_range + tweak_x + (float)j * y_sub_range + tweak_y - from;
                        TraceResult pixel = rayTrace(Ray(from, dir), 1, 1, config);
                        collect += pixel.color / (float)(config.sub_pixel * config.sub_pixel * config.sample_num);
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