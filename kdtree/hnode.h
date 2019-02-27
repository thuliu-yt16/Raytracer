#ifndef HNODE_H
#define HNODE_H
#include "vector3.h"
namespace Raytracer{
    class HNode{
    public:
        int num_l;
        int num_r;
        int num;
        float splitPoint;
        HNode* lc;
        HNode* rc;
        HNode* parent;
        float maxR;
        HNode(HNode* p, float r);
        bool IsLeaf();
        ~HNode();
    };
}

#endif
