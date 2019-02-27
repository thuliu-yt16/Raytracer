#ifndef MESH_H
#define MESH_H
#include "all.h"
#include "model.h"
#include "triangle.h"
#include "mtree.h"
namespace Raytracer{
    class Mesh:public Model{
    public:
        Mesh();
        MTree tree;
        AABB aabb;
        Vector3 bc;
        Vector3 u,v,w;
        Matrix3f scale_matrix;
        void LoadModel(const string& Filename);
        int GetType() const;
        int Intersect(const Ray& a_Ray,Intersection* isc) const;
        Vector3 RelativeCoordinates(const Vector3& p) const;
        Vector3 AbsoluteCoordinates(const Vector3& p) const;
        void ReadModel(ifstream& fin);
    };
}
#endif
