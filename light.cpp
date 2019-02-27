#include "light.h"

namespace Raytracer{
    Light::Light():LColor(Color()){}
    Light::Light(const Color& color):LColor(color){}

    PointLight::PointLight(){}
    PointLight::PointLight(const Vector3& o,const Color& color):Light(color),origin(o){}

    Photon PointLight::EmitPhoton() const{
        Vector3 d = Vector3::RandUnitVec();
        return Photon(Ray(origin, d, INF), LColor * 4 * PI * LPower);
    }
    void PointLight::ReadLight(ifstream& fin){
        string op;
        LPower = 200;
        while(fin >> op){
            if(op == "end") break;
            if(op == "color") fin >> LColor;
            if(op == "origin") fin >> origin;
            if(op == "power") fin >> LPower;
        }
    }
    SurfaceLight::SurfaceLight(){}
    SurfaceLight::SurfaceLight(const Vector3& c, const Vector3& _u, const Vector3&_v, float a, float _w, float _h, const Color& color):
        center(c),x(_u),y(_v),z(_u.Cross(_v).Normal()),w(_w),h(_h),Light(color),angle(a){
            cosangle = cos(angle * PI / 180);
            dis = 1 / tan(angle * PI / 180);
            power = 2 * PI * (1 - cos(angle * PI / 180));
        }

    Photon SurfaceLight::EmitPhoton() const{
        Vector3 d = Vector3::RandUnitVec();

        float du = (RAND() - 0.5) * w;
        float dv = (RAND() - 0.5) * h;
        Vector3 o = center + du * x + dv * y;
        Vector3 direction;
        do{
            direction = Vector3::RandUnitVec();
        }while(direction.Dot(z) <= cosangle);
        return Photon(Ray(o, (direction).Normal(),INF), LColor * power * LPower);
    }
    void SurfaceLight::ReadLight(ifstream& fin){
        string op;
        LPower = 200;
        while(fin >> op){
            if(op == "end") break;
            if(op == "color") fin >> LColor;
            if(op == "center") fin >> center;
            if(op == "u") fin >> x;
            if(op == "v") fin >> y;
            if(op == "width") fin >> w;
            if(op == "height") fin >> h;
            if(op == "angle") fin >> angle;
            if(op == "power") fin >> LPower;
        }
        z = x.Cross(y).Normal();
        cosangle = cos(angle * PI / 180);
        dis = 1 / tan(angle * PI / 180);
        power = 2 * PI * (1 - cosangle);
    }
    SpotLight::SpotLight(){}
    SpotLight::SpotLight(const Vector3& c, const Vector3& dir, float a, const Color& color):
        center(c), direction(dir), angle(a),Light(color){
        cosangle = cos(angle * PI / 180);
        power = 2 * PI * (1 - cosangle);
    }
    Photon SpotLight::EmitPhoton() const{

        Vector3 dir;
        do{
            dir = Vector3::RandUnitVec();
        }while(dir.Dot(direction) <= cosangle);
        return Photon(Ray(center, dir.Normal(), INF),LColor * LPower * power);
    }
    void SpotLight::ReadLight(ifstream& fin){
        string op;
        LPower = 200;
        while(fin >> op){
            if(op == "end") break;
            if(op == "center") fin >> center;
            if(op == "direction") fin >> direction;
            if(op == "angle") fin >> angle;
            if(op == "power") fin >> LPower;
            if(op == "color") fin >> LColor;
        }
        cosangle = cos(angle * PI / 180);
        power = 2 * PI * (1 - cosangle);
    }
}
