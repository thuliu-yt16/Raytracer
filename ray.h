#ifndef RAY_H
#define RAY_H
#include "all.h"
#include "vector3.h"
namespace Raytracer{
    class Ray{
    public:
        Vector3 origin;
        Vector3 direction;
        float distance;
        Ray();
        Ray(const Vector3& _o,const Vector3& _dir,float a_Dist);
        Ray Reflected(const Vector3 &O, const Vector3 &N) const;
        Ray Refracted(const Vector3 &O, const Vector3 &N, float n) const;
        friend ostream& operator << (ostream& out, const Ray& ray){
            out << "Ray:" << endl;
            out << "origin : " << ray.origin << endl;
            out << "direction : " << ray.direction << endl;
            out << "distance : " << ray.distance << endl;
            return out;
        }
    };
}
#endif
