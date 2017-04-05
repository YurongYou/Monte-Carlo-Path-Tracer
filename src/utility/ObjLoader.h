//
// Created by Ficos on 05/04/2017.
//

#ifndef RAYTRACING_OBJLOADER_H
#define RAYTRACING_OBJLOADER_H


#include <string>
#include <fstream>
#include "../object/Object.h"
#include "../Scene.h"

#define LINE_MAX_LEN 1000

class ObjLoader {
public:
    static void loadObj(const std::string file, Scene& scene){
        std::ifstream fin(file);
        if (!fin.is_open()) {
            std::cerr << "file not found, exit." << std::endl;
            exit(1);
        }
        char input[LINE_MAX_LEN];
        int count_face = 0;
        std::vector<int> count_normal;
        Material triangle_material = Material();
        triangle_material.setDiffuse_prob(0.6f);
        triangle_material.setIntrinsic_color( WHITE );
        // null point (to deal with the 1-base problem)
        count_normal.push_back(0);
        scene.mesh.points.push_back(VecF(0, 0, 0));
        scene.mesh.normal.push_back(VecF(0));
        std::vector<VecF> given_normal;
        bool hasGivenNormal = false;
        given_normal.push_back(VecF(0));
        while (fin.getline(input, LINE_MAX_LEN, '\n')) {
            if (input[0] == '#' or strlen(input) <= 1)
                continue;
            switch (input[0]){
                case 'v': {
                    if (input[1] == ' ') {
                        // v x y z
                        float x, y, z;
                        sscanf(input + 2, "%f %f %f", &x, &y, &z);
                        scene.mesh.points.push_back(VecF(x, y, z));
                        count_normal.push_back(0);
                        scene.mesh.normal.push_back(VecF(0));
                    }
                    else if (input[1] == 'n'){
                        float x, y, z;
                        sscanf(input + 3, "%f %f %f", &x, &y, &z);
                        given_normal.push_back(VecF(x, y, z));
                        hasGivenNormal = true;
                    }
                    else if (input[1] == 't') {
                        // do nothing
                        // we don't use vn here
                    } else {
                        std::cerr << input << std::endl;
                        std::cerr << "unrecognized format" << std::endl;
                        exit(1);
                    }
                    break;
                }
                case 'f': {
                    int p[3];
                    if (!hasGivenNormal){
                        sscanf(input + 2, "%d %d %d", &p[0], &p[1], &p[2]);
                    } else {
                        int n[3];
                        sscanf(input + 2, "%d//%d %d//%d %d//%d", &p[0], &n[0], &p[1], &n[1], &p[2], &n[2]);
                        for (int i = 0; i < 3; ++i) {
                            scene.mesh.normal[p[i]] = given_normal[n[i]];
                        }
                    }
                    MeshTriangle *tri = new MeshTriangle(triangle_material, "triangle_" + std::to_string(count_face),
                                                         p[0], p[1], p[2], &scene.mesh);
                    if (p[0] != p[1] && p[1] != p[2] && p[2] != p[0]) {
                        scene.addObject(tri);
                    }
                    if (!hasGivenNormal) {
                        for (int i = 0; i < 3; ++i) {
                            ++count_normal[p[i]];
                            scene.mesh.normal[p[i]] += tri->getLocal_normal();
                        }
                        ++count_face;
                    }
                    break;
                }
                default:
                    break;
            }
        }
        if (!hasGivenNormal){
            for (int i = 0; i < scene.mesh.normal.size(); ++i){
//                std::cout << scene.mesh.normal[i] << std::endl;
//                std::cout << count_normal[i] << std::endl;
                scene.mesh.normal[i] /= (float)count_normal[i];
                scene.mesh.normal[i].normalize();
            }
        }
    }
};


#endif //RAYTRACING_OBJLOADER_H
