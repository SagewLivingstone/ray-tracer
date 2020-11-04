# ray-tracer

Simple brute-force ray tracer written in C++, no external libs (future: png/jpg output lib)

## Sample Images
![materials](https://github.com/SagewLivingstone/ray-tracer/blob/main/demo_materials.png?raw=true)

_Test of simple diffuse (lambertian) and metallic-reflective materials_

![demo sphere scene](https://github.com/SagewLivingstone/ray-tracer/blob/main/demo_scene_spheres_low.png?raw=true)

_Demo sphere scene showing high-reflective and dielectric glass spheres, depth of field from high aperture. low-res because it takes a while to render on my laptop_

## current todos:
* multithread aa rays
* add library to output to png
* tri and rect primitives