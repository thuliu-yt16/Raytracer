#ifndef BEZIER3_H
#define BEZIER3_H
#include "all.h"
#include "vector3.h"
#include "model.h"
namespace Raytracer{
    #define ITERTIMES 20
    #define ITERDEPTH 20
    float B(int i,float t);
    float dB(int i,float t);

    class Bezier3:public Model{
    private:
        Vector3f* m_Patch;
        int m_Patches;
        AABB* aabbs;
    public:
        Bezier3();
        int GetType() const;
        int Intersect(const Ray& a_Ray, Intersection* isc) const;
        void LoadModel(const string& FileName);
        void ReadModel(ifstream& fin);
    };
}
#endif
