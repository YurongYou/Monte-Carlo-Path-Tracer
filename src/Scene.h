//
// Created by Ficos on 26/03/2017.
//

#ifndef RAYTRACING_SCENE_H
#define RAYTRACING_SCENE_H


#include <list>
#include "Object.h"
#include "Light.h"

typedef std::list<const Object*> ObjectList;
typedef std::list<const Light*> LightList;

class Scene {
public:
    void initScene();
    const ObjectList &getObejct_list() const {
        return object_list;
    }

    const LightList &getLight_list() const {
        return light_list;
    }

private:
    ObjectList object_list;
    LightList light_list;
};


#endif //RAYTRACING_SCENE_H