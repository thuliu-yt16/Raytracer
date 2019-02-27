#ifndef HKDTREE_H
#define HKDTREE_H
#include "vector3.h"
#include "hnode.h"
#include "hitpoint.h"
namespace Raytracer{
    #define NODE_VOLUME 64
    class HKDTree{
    public:
        vector<HitPoint> hitpointList;
        HNode* root;
        HKDTree();
        void Split(HNode* p, int l, int r, int axis, float maxR);
        void addHitPoint(const HitPoint& HitPoint);
        void clearHitPoint();
        void Build(float maxR);
        void Search(const Vector3& center, HNode* p, vector<pair<HNode*,int> >& res, int axis);
        void updateHitPoint(const Photon& photon);
        void updateMaxR(HNode* p);
        float calculateMaxR(HNode* p);
        ~HKDTree();
    };
}

#endif
