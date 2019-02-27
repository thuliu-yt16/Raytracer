#include "mtree.h"

namespace Raytracer{
    int str2int(const char* str, int& i){
        int x = 0;
        while(str[i] >= '0' && str[i] <= '9'){
            x = x * 10 + str[i++] - '0';
        }
        return x;
    }

    MNode::MNode():lc(NULL),rc(NULL){}
    bool MNode::IsLeaf(){return !lc && !rc;}
    MTree::MTree():root(NULL){}
    float MTree::FindPlane(const vector<Triangle*>& tris, int axis, const AABB& bb){
        vector<pair<float,int>> planes;
        planes.clear();
        for(unsigned int i = 0; i < tris.size(); i ++){
            planes.push_back(make_pair(tris[i] -> aabb.minxyz.cell[axis],i + 1));
            planes.push_back(make_pair(tris[i] -> aabb.maxxyz.cell[axis],- i - 1));
        }
        sort(planes.begin(), planes.end(),
            [axis](const pair<float,int>& a, const pair<float,int>& b){return a.first < b.first;});

        int Nleft = -1;
        int Nright = tris.size();
        float s_ = bb.SurfaceArea(axis);
        float s = bb.SurfaceArea((axis + 1) % 3) + bb.SurfaceArea((axis + 2) % 3);
        float left = bb.minxyz.cell[axis];
        float right = bb.maxxyz.cell[axis];
        float total_length = right - left;
        float best_cost = INF;
        pair<float,int> best_plane = make_pair(0.0,0);
        float cost;
        for(auto plane : planes){
            if(plane.second > 0) Nleft ++;
            if(plane.second < 0) Nright --;
            if(plane.first > left && plane.first < right){
                cost = Nleft * (s_ + s * (plane.first - left) / total_length) + Nright * (s_ + s * (right - plane.first) / total_length);

                if(cost < best_cost){
                    best_cost = cost;
                    best_plane = plane;
                }
            }
        }
        return best_plane.first;
    }
    void MTree::Split(MNode*& p,const vector<Triangle*>& tris, int axis,const AABB& bb){
        p = new MNode();
        p -> aabb = bb;
        p -> subtriangles = tris;
        if(tris.size() <= 128){
            p -> rc = p -> lc = NULL;
            return;
        }
        vector<Triangle*> lefttris;
        lefttris.clear();
        vector<Triangle*> righttris;
        righttris.clear();
        float splitPoint = FindPlane(tris,axis,bb);
        p -> splitPoint = splitPoint;

        for(auto iter : tris){
            if(iter -> aabb.minxyz.cell[axis] < splitPoint)
                lefttris.push_back(iter);
            if(iter -> aabb.maxxyz.cell[axis] > splitPoint)
                righttris.push_back(iter);
        }

        Split(p -> lc, lefttris, (axis + 1) % 3, bb.Split(p -> splitPoint,axis,0));
        Split(p -> rc, righttris, (axis + 1) % 3, bb.Split(p -> splitPoint,axis,1));
    }
    void MTree::Build(){
        AABB aabb = triangles[0] -> aabb;
        for(unsigned int i = 1; i < triangles.size(); i ++){
            aabb.Expand(triangles[i] -> aabb);
        }
        Split(root, triangles, 0, aabb);
    }
    int MTree::Intersect(const Ray& ray, Intersection* isc) const{
        return Intersect(ray, isc, root, 0);
    }
    int MTree::Intersect(const Ray& ray, Intersection* isc, MNode* p, int axis) const{
        if(!p) return MISS;
        if(p -> IsLeaf()){
            Intersection local_isc;
            Intersection final_isc;
            float dis = INF;
            for(auto iter : p -> subtriangles){
                if(iter -> Intersect(ray,&local_isc)){
                    Vector3 inter = local_isc.intersection;
                    if(local_isc.distance < dis && p -> aabb.inBox(inter)){
                        dis = local_isc.distance;
                        final_isc = local_isc;
                    }
                }
            }
            *isc = final_isc;
            return isc -> result;
        }

        *isc = Intersection();
        Intersection local_isc;
        float dis = INF;
        if(p -> lc -> aabb.Intersect(ray)){
            if(Intersect(ray, &local_isc, p -> lc, (axis + 1) % 3)){
                Vector3 inter = local_isc.intersection;
                if(local_isc.distance < dis && p -> aabb.inBox(inter)){
                    dis = local_isc.distance;
                    *isc = local_isc;
                }
            }
        }
        if(p -> rc -> aabb.Intersect(ray)){
            if(Intersect(ray, &local_isc, p -> rc, (axis + 1) % 3)){
                Vector3 inter = local_isc.intersection;
                if(local_isc.distance < dis && p -> aabb.inBox(inter)){
                    dis = local_isc.distance;
                    *isc = local_isc;
                }
            }
        }
        return isc -> result;
    }

}
