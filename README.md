# RayTracing
An implementation on ray tracing algorithm for CS230 (Virtual Reality and Interactive 3D Graphics Display), ACM Class, SJTU

## Features
* Supported effects:
    * color bleeding
    * transparent object
    * soft shadow
* Global illumination with Monte Carlo Path Tracing
* Multi-thread rendering
* load .obj model
* Simple texture mapping

## System requirement
Ubuntu v14.04+
CMake v3.2.2+

## Project Structure

## Usages
### Build
```
mkdir bin && cd bin
cmake .. && make
```

### Run
```
cd bin
./RayTracing -MCPT true [-test_case <twist_mesh|teapot_mesh|mix_twist_mesh>] [-size <height> <width>] [-view <position of viewpoint> <position of target>]
```

## Demos
1. Box with ball
    * Global Illumination with Monte Carlo Path Tracing
    * with **color bleeding**, **transparent object**, **soft shadow** effects
    * all diffuse surfaces are Lambertian surface
    * render with Intel(R) Xeon(R) CPU E5-2630 v4 @ 2.20GHz (20 cores) in 5731.2s
    ![](https://raw.githubusercontent.com/YurongYou/RayTracing/master/results/render_image_MCPT_highres.jpg?token=AM-ptWp2Mz87K9diVOlomkYMJkU9ndcRks5Y7ujlwA%3D%3D)
	* run with
			
			./RayTracing -MCPT true
	
## Reference
* https://www.scratchapixel.com/
* http://www.flipcode.com/archives/Raytracing_Topics_Techniques-Part_1_Introduction.shtml
* Shirley, P., & Morley, R. K. (2008). Realistic ray tracing. AK Peters, Ltd..

## TODOs
* [ ] Texture
* [ ] bounding box speed up

