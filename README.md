# ray-tracer

Simple brute-force ray tracer written in C++, no external libs (except for png/jpg output)

## Sample Images
![materials](https://github.com/SagewLivingstone/ray-tracer/blob/main/demo_materials.png?raw=true)

_Test of simple diffuse (lambertian) and metallic-reflective materials_

![checker texture and mats](https://github.com/SagewLivingstone/ray-tracer/blob/main/materials_render.png)

_Various materials and dielectrics_

![demo sphere scene](https://github.com/SagewLivingstone/ray-tracer/blob/main/demo_scene_spheres.png?raw=true)

_Demo sphere scene showing high-reflective and dielectric glass spheres, depth of field from high aperature simulation._

![](https://github.com/SagewLivingstone/ray-tracer/blob/main/perlin_and_marble.png?raw=true)

_Perlin noise turbulant texture and a marble texture using perlin noise on the phase of a sin wave_

## current todos:
* multithread aa rays
* ~perlin noise implementation~
* ~add library to output to png~
* ~textures and uv mapping~
  * ~bitmap image textures~
  * ~coordinate uv's~
* ~possible issue with gamma correction, crushing low rgb values read from bitmap textures?~
* ~emissive lighting~
* diffuse volumes
* tri primitive
* ~rect primitives~
