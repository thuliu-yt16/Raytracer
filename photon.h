#ifndef PHOTON_H
#define PHOTON_H
#include "all.h"
#include "vector3.h"
#include "ray.h"

namespace Raytracer{
    class Photon{
    public:
        Ray ray;
        Color photonColor;
        Photon();
        Photon(const Ray& r, const Color& pc);
        Photon Diffused(const Vector3& P, const Vector3& N) const;
        Photon Reflected(const Vector3& P, const Vector3& N) const;
        Photon Refracted(const Vector3& P, const Vector3& N,float n) const;
        friend ostream& operator << (ostream& out, const Photon& p){
            out << "Photon:" << endl;
            out << "\tposition: " << p.ray.origin << endl;
            out << "\tdirection: " << p.ray.direction << endl;
            out << "\tcolor: " << p.photonColor << endl;
            return out;
        }
    };
}

#endif
