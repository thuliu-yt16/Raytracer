#ifndef HITPOINT_H
#define HITPOINT_H
#include "all.h"
#include "primitive.h"
#include "vector3.h"
#include "photon.h"
namespace Raytracer{
    class HitPoint{
    public:
        Vector3 position;
        Vector3 normal;
        Vector3 direction;
        Color wgt;
        float R;
        float photons;
        Color flux;
        Primitive* primitive;
        int pixel_x,pixel_y;
        HitPoint();
        HitPoint(const Vector3& o, const Vector3& n, const Vector3& dir, const Vector3 &weight, float r, Primitive* prim, int x, int y, Color c);
        float GetBRDF(const Photon& photon);

        friend ostream& operator << (ostream& out, const HitPoint& hitpoint){
            out << "HitPoint:" << endl;
            out << "\tposition : " << hitpoint.position << endl;
            out << "\tnormal : " << hitpoint.normal << endl;
            out << "\tdirection : " << hitpoint.direction << endl;
            out << "\twgt : " << hitpoint.wgt << endl;
            out << "\tR : " << hitpoint.R << endl;
            out << "\tN : " << hitpoint.photons << endl;
            out << "\tflux : " << hitpoint.flux << endl;
            out << "\t(x,y) : ( " << hitpoint.pixel_x << " , " << hitpoint.pixel_y << " )" << endl;
            return out;
        }

    };
}

#endif
