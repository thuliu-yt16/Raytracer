#ifndef BEZIER2_H
#define BEZIER2_H
#include "all.h"
#include "model.h"
#include "vector3.h"
#include "bezier3.h"
namespace Raytracer{
    class Bezier2: public Model{
    public:
        OCBB* ocbbs;
        int patches;
        Vector2f* patch;
        Vector3 bc;
        Vector3 u,v,w;
        Matrix2f scale_matrix;
        Bezier2();
        Bezier2(const Vector3& _bc, const Vector3& _u, const Vector3& _v);
        int GetType() const;
        int Intersect(const Ray& a_Ray, Intersection* isc) const;
        void LoadModel(const string& FileName);
        Vector3 RelativeCoordinates(const Vector3& p) const;
        Vector3 AbsoluteCoordinates(const Vector3& p) const;
        void ReadModel(ifstream& fin);
    };
}
#endif
