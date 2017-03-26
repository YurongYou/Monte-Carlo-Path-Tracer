//
// Created by Ficos on 26/03/2017.
//

#ifndef RAYTRACING_SCENE_H
#define RAYTRACING_SCENE_H


#include "Object.h"

class Scene {
public:
    Scene(): num_objects(0), objects(NULL){}
    ~Scene(){
        delete[] objects;
    }
    void initScene();

    int getNum_objects() const {
        return num_objects;
    }

    Object* getObjects(int idx) const {
        return objects[idx];
    }

private:
    int num_objects;
    Object** objects;
};


#endif //RAYTRACING_SCENE_H
