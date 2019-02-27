#ifndef PPMRENDER_H
#define PPMRENDER_H
#include "all.h"
#include "hitpoint.h"
#include "engine.h"
#include "hkdtree.h"
namespace Raytracer{
    float GetBRDF(const Vector3& l, const Vector3& n, const Vector3& v, float diff, float spec);
    #define ORIGINR (1)
    #define PHOTON_NUMBERS (30000)
    #define MAX_ITER (5)
    class PPMRender{
    public:
        Engine* LEngine;
        HKDTree tree;
        PPMRender();
        void Init(Engine* engine);
        void rayTrace(const Ray& ray, int x, int y,int depth, Color weight, float rindex, bool inPrimitive);
        void photonTrace(const Photon& photon, int depth, float rindex, bool inPrimitive);
        void Render();
    };
}
#endif
