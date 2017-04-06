# MCPT Ray Tracer
An implementation on realistic ray tracing algorithm for CS230 (Virtual Reality and Interactive 3D Graphics Display).


## Features
* Supported effects:
    * **color bleeding**
    * **transparent object**
    * **soft shadow**
    * **caustic**
* Global illumination with Monte Carlo Path Tracing
* Multi-thread rendering
* load .obj model
* Anti-aliasing
* Simple texture mapping on plane
* Simple bounding box speed up

## System requirement
Ubuntu v14.04+
CMake v3.2.2+

## Project Structure
```
models/ 		# 3D models in .obj files for rendering
results/ 		# rendering results
vendor/ 		# third-party library (cameron314/concurrentqueue for multi-thread rendering)
src/			# source code
	/object		# dealing with 3D objects
	/utility 	# basic utility objects / functions / constants, including objLoader, ray class, vector class and some global constants
 	Engine.cpp	# core rendering engine
 	Engine.h	# header file for core rendering engine
 	scene.cpp	# 3D scene builder
 	scene.h		# header file for 3D scene builder
 	main.cpp	# entrance 	
```

## Usages
### Build
```
mkdir bin && cd bin
cmake .. && make
```

### Run
```
cd bin
./RayTracing	[-MCPT <true|false|mix>] 
			[-test_case <twist_mesh|teapot_mesh|mix_twist_mesh>] 
			[-size <height> <width>] 
			[-view <position of viewpoint> <position of target>]
```
* To render with MCPT, please toggle `-MCPT true`.
* The frame is set to be 5 units in front of the view point and not tilted. 
* The default size of the rendered image is `640x480`, and if specified in cli options, `height / width` should be `3 / 4`.


## Demos
1. Box with balls
    * Global Illumination with Monte Carlo Path Tracing
    * with **color bleeding**, **transparent object**, **soft shadow** and **caustic** effects
    * all diffuse surfaces are [Lambertian surfaces](https://en.wikipedia.org/wiki/Lambertian_reflectance)
    * render with Intel(R) Xeon(R) CPU E5-2630 v4 @ 2.20GHz (20 cores) in 5731.2s
    
    	![](https://raw.githubusercontent.com/YurongYou/RayTracing/master/results/render_image_MCPT_highres.jpg?token=AM-ptWp2Mz87K9diVOlomkYMJkU9ndcRks5Y7ujlwA%3D%3D)
	* run with
			
			./RayTracing -MCPT true -view 0 0 -5 0 0 0
2. Twist with ball, and texture
	* Global Illumination with Monte Carlo Path Tracing
	* with **color bleeding**, **soft shadow** effects
	* .obj 3D model (the twist)
	* all diffuse surfaces are Lambertian surfaces
	* texture mapping (ground and the back wall)
	* render with Intel(R) Xeon(R) CPU E5-2680 v4 @ 2.40GHz (46 cores) in 6422.3s
		
		![](https://raw.githubusercontent.com/YurongYou/RayTracing/master/results/render_image_texture_obj.jpg)
	* run with
			
			./RayTracing -MCPT true -test_case mix_twist_mesh -view 2 4 -12 2 4 0
			
			
## Further Possible Improvements
* MCPT with direct ray casting
* KD-tree speedup
* texture mapping on .obj 3D model

## Reference
* https://www.scratchapixel.com/
* http://www.flipcode.com/archives/Raytracing_Topics_Techniques-Part_1_Introduction.shtml
* Shirley, P., & Morley, R. K. (2008). Realistic ray tracing. AK Peters, Ltd..
* [wiki: path tracing](https://en.wikipedia.org/wiki/Path_tracing)

## TODOs
* [x] Texture
* [x] bounding box speed up


