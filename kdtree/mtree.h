#ifndef MTREE_H
#define MTREE_H
#include "all.h"
#include "vector3.h"
#include "model.h"
#include "triangle.h"
namespace Raytracer{
    int str2int(const char* str, int& i);
    class MNode{
    public:
        vector<Triangle*> subtriangles;
        float splitPoint;
        AABB aabb;
        MNode *lc, *rc;
        MNode();
        bool IsLeaf();
    };

    class MTree{
    private:
        int Intersect(const Ray& ray, Intersection* isc, MNode* p, int axis) const;
        float FindPlane(const vector<Triangle*>& tris, int axis, const AABB& bb);
    public:
        MTree();
        vector<Triangle*> triangles;
        MNode* root;
        void Build();
        void Split(MNode*& p, const vector<Triangle*>& tris, int axis,const AABB& bb);
        int Intersect(const Ray& ray, Intersection* isc) const;
    };
}
#endif
