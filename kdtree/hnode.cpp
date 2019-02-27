#include "hnode.h"
namespace Raytracer{
    HNode::HNode(HNode* p,float r):parent(p),maxR(r),lc(NULL),rc(NULL),splitPoint(0),num_l(-1),num_r(-1){}
    bool HNode::IsLeaf(){
        return !lc && !rc;
    }
    HNode::~HNode(){
        delete lc, rc;
        lc = rc = NULL;
    }
}
