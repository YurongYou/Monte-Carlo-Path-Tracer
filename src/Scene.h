//
// Created by Ficos on 26/03/2017.
//

#ifndef RAYTRACING_SCENE_H
#define RAYTRACING_SCENE_H


#include <list>
#include "Object.h"

typedef std::list<const Object*> ObjectList;

class Scene {
public:
    void initScene();
    const ObjectList &getObejct_list() const {
        return object_list;
    }

private:
    ObjectList object_list;

};


#endif //RAYTRACING_SCENE_H