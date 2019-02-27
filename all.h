#ifndef ALL_H
#define ALL_H
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <Eigen/Dense>
using namespace std;
using namespace Eigen;
namespace Raytracer{
    #define float double
    #define EPSILON			(2e-4)
    #define TRACEDEPTH		(6)
    #define PI				(3.141592653589793238462)
    #define PPM_ALPHA (0.7)

    #define HIT (1)
    #define MISS (0)
    #define INPRIM (-1)
    #define INF (1e20)
    #define AIR_RINDEX (1.0)
    #define RAND() (rand() * 1.0 / RAND_MAX)

    class AABB;
    class Bezier3;
    class Camera;
    class Engine;
    class HitPoint;
    class Image;
    class Intersection;
    class Light;
    class Material;
    class Mesh;
    class MNode;
    class MTree;
    class Model;
    class OCBB;
    class Photon;
    class PointLight;
    class Plane;
    class PPMRender;
    class Primitive;
    class Ray;
    class Scene;
    class Sphere;
    class SurfaceLight;
    class Vector3;
    class Mesh;
    class MNode;
    class MTree;
    class Texture;
    class Triangle;
}

#endif
