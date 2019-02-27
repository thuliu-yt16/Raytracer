#include "hitpoint.h"
namespace Raytracer{
    HitPoint::HitPoint(){}
    HitPoint::HitPoint(const Vector3& o, const Vector3& n, const Vector3& dir, const Vector3& weight, float r, Primitive* prim, int x, int y, Color c):
        position(o),normal(n),direction(dir), wgt(weight),R(r),primitive(prim),flux(c),photons(0),pixel_x(x),pixel_y(y){}
    float HitPoint::GetBRDF(const Photon& photon){
        Material* pm = primitive -> GetMaterial();
        Vector3 l = photon.ray.direction;
        Vector3 r = l - 2 * DOT(l,normal) * normal;
        return (pm -> diff + abs(pm -> spec * pow(normal.Dot((- l - direction) * 0.5), 20))) * abs(l.Dot(normal));
    }
}
