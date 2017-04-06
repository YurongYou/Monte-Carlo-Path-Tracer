# RayTracing
An implementation on ray tracing algorithm for CS230 (Virtual Reality and Interactive 3D Graphics Display), ACM Class, SJTU

## Features
* Three effects supported:
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
2.

