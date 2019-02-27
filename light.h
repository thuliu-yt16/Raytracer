#ifndef LIGHT_H
#define LIGHT_H
#include "all.h"
#include "photon.h"
namespace Raytracer{
    class Light{
    public:
        Color LColor;
        float LPower;
        Light();
        Light(const Color& color);
        virtual Photon EmitPhoton() const = 0;
        virtual void ReadLight(ifstream& fin) = 0;
    };
    class PointLight: public Light{
    public:
        Vector3 origin;
        Vector3 direction;
        float angle;
        PointLight();
        PointLight(const Vector3& o,const Color& color = Color(1.0,1.0,1.0));
        Photon EmitPhoton() const;
        void ReadLight(ifstream& fin);
    };
    class SurfaceLight: public Light{
    private:
        float dis;
        float power;
    public:
        float angle;
        float cosangle;
        Vector3 center;
        Vector3 x,y,z;
        float w,h;
        SurfaceLight();
        SurfaceLight(const Vector3& c, const Vector3& _u, const Vector3& _v,float a, float _w, float _h,const Color& color = Color(1.f,1.f,1.f));
        Photon EmitPhoton() const;
        void ReadLight(ifstream& fin);
    };
    class SpotLight: public Light{
    private:
        Vector3 x;
        Vector3 y;
        float dis;
        float power;
        float cosangle;
    public:
        Vector3 center;
        Vector3 direction;
        float angle;
        SpotLight();
        SpotLight(const Vector3& c, const Vector3& dir, float a, const Color& color = Color(1.f,1.f,1.f));
        Photon EmitPhoton() const;
        void ReadLight(ifstream& fin);
    };
}
#endif
