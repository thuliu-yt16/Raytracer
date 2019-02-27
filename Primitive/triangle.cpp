#include "triangle.h"

namespace Raytracer{
    Triangle::Triangle(){}
    Triangle::Triangle(const Vector3& _a,const Vector3& _b, const Vector3& _c):
    a(_a),b(_b),c(_c){
        aabb = AABB(Min(Min(a,b),c),Max(Max(a,b),c));
    }
    Triangle::Triangle(const Vector3& _a, const Vector3 &_b, const Vector3& _c, const Vector3& n1, const Vector3& n2, const Vector3& n3):
    a(_a),b(_b),c(_c),normal_a(n1),normal_b(n2),normal_c(n3){
        aabb = AABB(Min(Min(a,b),c),Max(Max(a,b),c));
    }
    void Triangle::SetNormal(const Vector3& n1, const Vector3& n2, const Vector3& n3){
        normal_a = n1; normal_b = n2; normal_c = n3;
    }
    void Triangle::SetVertex(const Vector3& p1, const Vector3& p2, const Vector3& p3){
        a = p1; b = p2; c = p3;
        aabb = AABB(Min(Min(a,b),c),Max(Max(a,b),c));
    }
    int Triangle::Intersect(const Ray& ray, Intersection* isc) const{
        Vector3 o = ray.origin;
        Vector3 d = ray.direction;
        float dis = ray.distance;
        Vector3 e1 = a - c;
        Vector3 e2 = b - c;
        Vector3 q = d.Cross(e2);
        float a_ = e1.Dot(q);
        int result = MISS;
        float f = 1 / a_;
        Vector3 s = o - c;
        float u = f * (s.Dot(q));
        if(u >= 0.0){
            Vector3 r = s.Cross(e1);
            float v = f * (d.Dot(r));
            if(v >= 0.0 && u + v <= 1.0){
                float t = f * (e2.Dot(r));
                if(t < dis && t >= 0){
                    float w = 1 - u - v;
                    isc -> distance = t;
                    isc -> normal = (u * normal_a + v * normal_b + w * normal_c).Normal();
                    isc -> intersection = u * a + v * b + w * c;
                    isc -> parameter = Vector3(u,v,0);
                    result = isc -> normal.Dot(d) < 0 ? HIT : INPRIM;
                }
            }
        }
        isc -> result = result;
        return result;
    }
}
