#include "ray.h"
namespace Raytracer{

    Ray::Ray():origin(Vector3(0.0,0.0,0.0)),direction(Vector3(0.0,0.0,0.0)) , distance(0.0){}
    Ray::Ray(const Vector3& _o, const Vector3& _dir , float a_Dist):origin(_o),direction(_dir), distance(a_Dist){}
    Ray Ray::Reflected(const Vector3& P, const Vector3& N) const{
        Vector3 dir = direction - 2.0 * DOT(direction,N) * N;
        return Ray(P + EPSILON * dir, dir,INF);
    }
    Ray Ray::Refracted(const Vector3& P, const Vector3& N, float n) const{
        float cosI = -DOT(N, direction);
        float cosT2 = 1.0 - n * n * (1.0 - cosI * cosI);
        if(cosT2 > 0.0){
            Vector3 T = (n * direction) + (n * cosI - sqrtf(cosT2)) * N;
            NORMALIZE(T);
            return Ray(P + EPSILON * T, T, INF);
        }
        else{
            return Reflected(P,N);
        }
    }
}
