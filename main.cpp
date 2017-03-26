#include <iostream>
#include "Vector.h"
#include "constants.h"
#include "Object.h"
#include "Ray.h"

using namespace std;

int main() {
//    Sphere s = Sphere(Material(), "new", false, VecF(0, 0, 0), 3);
    float dist = 0;
//    cout << s.intersect(Ray(VecF(0, 0, 5), VecF(0, 0.5, -1)), dist) << ", dist: " << dist;
    Plane p = Plane(Material(), "new", false, VecF(0, 0, 1), 0);
    cout << p.intersect(Ray(-VecF(0, 1, 1), -VecF(1, 0, -1)), dist) << ", dist: " << dist;
    return 0;
}