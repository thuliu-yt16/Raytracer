#include "hkdtree.h"

namespace Raytracer{
    HKDTree::HKDTree(){
        root = NULL;
    }

    void HKDTree::Split(HNode* p,int l, int r, int axis, float maxR){
        if(!p)
            return;
        p -> num_l = l;
        p -> num_r = r;
        if(r - l + 1 <= NODE_VOLUME){
            delete p -> lc, p -> rc;
            p -> lc = p -> rc = NULL;
            return;
        }
        sort(hitpointList.begin() + l, hitpointList.begin() + r + 1,
        [axis](const HitPoint&a, const HitPoint& b){return a.position.cell[axis] < b.position.cell[axis];});

        int m = (l + r) >> 1;
        p -> num = m;
        p -> splitPoint = hitpointList[p -> num].position.cell[axis];
        p -> lc = new HNode(p, maxR);
        p -> rc = new HNode(p, maxR);

        Split(p -> lc, l, m - 1, (axis + 1) % 3, maxR);
        Split(p -> rc, m + 1, r, (axis + 1) % 3, maxR);
    }
    void HKDTree::Build(float maxR){
        if(!root){
            root = new HNode(NULL, maxR);
        }
        Split(root, 0, hitpointList.size() - 1, 0, maxR);
    }
    void HKDTree::Search(const Vector3& center, HNode* p, vector<pair<HNode*,int> >& res, int axis){
        if(!p)
            return;
        float leftBound = center.cell[axis] - p -> maxR;
        float rightBound = leftBound + 2 * p -> maxR;
        if(p -> IsLeaf()){
            for(unsigned int i = p -> num_l; i <= p -> num_r; i ++){
                if(SQRDISTANCE(hitpointList[i].position, center) < hitpointList[i].R * hitpointList[i].R){
                    res.push_back(make_pair(p,i));
                }
            }
        }
        else if(p -> splitPoint < leftBound){
            Search(center, p -> rc, res, (axis + 1) % 3);
        }
        else if(p -> splitPoint > rightBound){
            Search(center, p -> lc, res, (axis + 1) % 3);
        }
        else{
            int num = p -> num;
            HitPoint& h = hitpointList[num];
            if(SQRDISTANCE(h.position, center) < h.R * h.R){
                res.push_back(make_pair(p,num));
            }
            Search(center, p -> lc, res, (axis + 1) % 3);
            Search(center, p -> rc, res, (axis + 1) % 3);
        }
    }
    void HKDTree::addHitPoint(const HitPoint& hitpoint){
        hitpointList.push_back(hitpoint);
    }
    void HKDTree::clearHitPoint(){
        hitpointList.clear();
    }
    HKDTree::~HKDTree(){
        delete root;
        root = NULL;
        hitpointList.clear();
    }
    float HKDTree::calculateMaxR(HNode* p){
        float mr = 0;
        if(p -> IsLeaf()){
            for(unsigned int i = p -> num_l; i <= p -> num_r; i ++){
                mr = max(hitpointList[i].R, mr);
            }
        }
        else{
            mr = max(hitpointList[p -> num].R, mr);
        }
        mr = max(mr,max(p -> lc ? p -> lc -> maxR : 0, p -> rc ? p -> rc -> maxR : 0));
        return mr;
    }
    void HKDTree::updateMaxR(HNode* p){
        HNode* currNode = p;
        while(currNode){
            p -> maxR = calculateMaxR(p);
            currNode = currNode -> parent;
        }
    }
    void HKDTree::updateHitPoint(const Photon& photon){
        vector<pair<HNode*, int> > res;
        res.clear();
        Search(photon.ray.origin, root, res, 0);
        for(auto i : res){
            HitPoint &hitpoint = hitpointList[i.second];
            if(photon.ray.direction.Dot(hitpoint.normal) < 0){
                Color deltaflux = photon.photonColor * hitpoint.GetBRDF(photon);
                if(hitpoint.photons == 0){
                    hitpoint.flux = deltaflux;
                    hitpoint.photons = 1;
                }
                else{
                    float p = (hitpoint.photons + PPM_ALPHA) / (hitpoint.photons + 1);
                    hitpoint.flux = (hitpoint.flux + deltaflux) * p;
                    hitpoint.R *= sqrt(p);
                    hitpoint.photons += PPM_ALPHA;
                    updateMaxR(i.first);
                }
            }
        }
    }
}
