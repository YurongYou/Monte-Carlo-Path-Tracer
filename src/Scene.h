//
// Created by Ficos on 26/03/2017.
//

#ifndef RAYTRACING_SCENE_H
#define RAYTRACING_SCENE_H


#include <list>
#include "object/Object.h"

typedef std::list<const Object*> ObjectList;

class Scene {
public:
    void CornellBox();
    void simpleTest();
    void castTest(std::string file);
    void MixTest(std::string file);
    void ObjTest(std::string file);
    const ObjectList &getObject_list() const {
        return object_list;
    }
    void addObject(const Object *obj){
        object_list.push_back(obj);
        if (!obj->getMaterial().getEmission().isEqual(BLACK)){
            light_list.push_back(obj);
        }
    }

    const ObjectList &getLight_list() const {
        return light_list;
    }

    ObjectList object_list;
    ObjectList light_list;
};


#endif //RAYTRACING_SCENE_H