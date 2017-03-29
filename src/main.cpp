#include <iostream>
#include "Vector.h"
#include "common.h"
#include "Object.h"
#include "Ray.h"
#include "Engine.h"

using namespace std;

int main() {
    Engine(640, 480, -4, 3, 4, -3).render();
}