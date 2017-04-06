# RayTracing
An implementation on ray tracing algorithm for CS230 (Virtual Reality and Interactive 3D Graphics Display), ACM Class, SJTU

## Features
* Supported effects:
    * color bleeding
    * transparent object
    * soft shadow
* Global illumination with Monte Carlo Path Tracing
* Multi-thread rendering
* Support loading .obj model
* Simple texture mapping

## System requirement
Ubuntu 14.04+
CMake version 3.5.1+

## Usages
### build
```
    mkdir bin
    cd bin
    cmake ..
    make
```
### Rendering Images
1. Box with ball
    * Global Illumination with Monte Carlo Path Tracing
    * with color bleeding, transparent object, soft shadow effects
    * all diffuse surfaces are Lambertian surface
    * using Intel(R) Xeon(R) CPU E5-2630 v4 @ 2.20GHz (20 cores) in 5731.2s
    ![]()
2.

## Reference
* https://www.scratchapixel.com/
* Shirley, P., & Morley, R. K. (2008). Realistic ray tracing. AK Peters, Ltd..
* http://www.flipcode.com/archives/Raytracing_Topics_Techniques-Part_1_Introduction.shtml

