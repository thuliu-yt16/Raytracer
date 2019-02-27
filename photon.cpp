#include "photon.h"

namespace Raytracer{
    Photon::Photon(){};
    Photon::Photon(const Ray& r, const Color& pc):ray(r),photonColor(pc){}
    Photon Photon::Diffused(const Vector3& P, const Vector3& N) const{
        Vector3 i = ray.direction.Cross(N).Normal();
        Vector3 j = N.Cross(i).Normal();
        Vector3 k = N.Normal();

        Vector3 dir = Vector3::RandUnitVec();
        Photon res = *this;
        res.ray = Ray(P, (dir.x * i + dir.y * j + abs(dir.z) * k).Normal(),INF);
        return res;
    }

    Photon Photon::Reflected(const Vector3& P, const Vector3& N) const{
        Photon res = *this;
        res.ray = res.ray.Reflected(P, N);
        return res;
    }
    Photon Photon::Refracted(const Vector3& P, const Vector3& N,float n) const{
        Photon res = *this;
        res.ray = res.ray.Refracted(P, N, n);
        return res;
    }
}
