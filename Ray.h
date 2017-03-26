//
// Created by Ficos on 26/03/2017.
//

#ifndef RAYTRACING_RAY_H
#define RAYTRACING_RAY_H

#include "Vector.h"

class Ray {
public:
    Ray(const VecF &origin, const VecF &direction): origin(origin), direction(direction) {
        Ray::direction.normalize();
    }
    Ray(): origin(VecF(0)), direction(VecF(0)) {}

    const VecF &getOrigin() const {
        return origin;
    }

    void setOrigin(const VecF &origin) {
        Ray::origin = origin;
    }

    const VecF &getDirection() const {
        return direction;
    }

    void setDirection(const VecF &direction) {
        Ray::direction = direction;
        Ray::direction.normalize();
    }

    VecF origin;
    VecF direction;
};

#endif //RAYTRACING_RAY_H
