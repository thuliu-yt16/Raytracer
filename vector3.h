#ifndef VECTOR3_H
#define VECTOR3_H
#include "all.h"
namespace Raytracer{
    #define DOT(A,B)		(A.x*B.x+A.y*B.y+A.z*B.z)
    #define NORMALIZE(A)	{float l=1/sqrtf(A.x*A.x+A.y*A.y+A.z*A.z);A.x*=l;A.y*=l;A.z*=l;}
    #define LENGTH(A)		(sqrtf(A.x*A.x+A.y*A.y+A.z*A.z))
    #define SQRLENGTH(A)	(A.x*A.x+A.y*A.y+A.z*A.z)
    #define SQRDISTANCE(A,B) ((A.x-B.x)*(A.x-B.x)+(A.y-B.y)*(A.y-B.y)+(A.z-B.z)*(A.z-B.z))

    class Vector3{
    public:
        Vector3();
        Vector3(float a_X,float a_Y,float a_Z);
        Vector3(const Vector3f& v);
        void Set(float a_X,float a_Y,float a_Z);
        void Normalize();
        float Length() const;
        float SqrLength() const;
        float Dot(Vector3 a_V) const;
        Vector3 Normal() const;
        Vector3 Cross(const Vector3& b) const;
        float Max() const;
        void operator += (const Vector3& a_V);
        void operator += (Vector3* a_V);
        void operator -= (const Vector3& a_V);
        void operator -= (Vector3* a_V);
        void operator *= (float f);
        void operator *= (const Vector3& a_V);
        void operator *= (Vector3* a_V);
        void operator /= (float f);
        Vector3 operator- () const;
        friend istream& operator >> (istream& in, Vector3& v);
        friend ostream& operator << (ostream& out, const Vector3& v);
        friend Vector3 operator + (const Vector3& v1,const Vector3& v2);
        friend Vector3 operator - (const Vector3& v1,const Vector3& v2);
        friend Vector3 operator + (const Vector3& v1,Vector3* v2);
        friend Vector3 operator - (const Vector3& v1,Vector3* v2);
        friend Vector3 operator * (const Vector3& v,float f);
        friend Vector3 operator * (const Vector3& v1,const Vector3& v2);
        friend Vector3 operator * (float f,const Vector3& v);
        friend Vector3 operator / (const Vector3& v, float f);
        friend Vector3 operator * (const Matrix3f& m, const Vector3& v);
        friend Vector3 Max(const Vector3& v1, const Vector3& v2);
        friend Vector3 Min(const Vector3& v1, const Vector3& v2);
        friend Vector3 Exp(const Vector3& v);
        union{
            struct { float x, y, z; };
            struct { float r, g, b; };
            struct { float cell[3]; };
        };
        static Vector3 Unit(int axis){
            if(axis == 0)
                return Vector3(1,0,0);
            if(axis == 1)
                return Vector3(0,1,0);
            if(axis == 2)
                return Vector3(0,0,1);
            return Vector3(0,0,0);
        }
        static Vector3 RandUnitVec(){
            float u, v, r2;
            do {
                u = RAND() * 2 - 1;
                v = RAND() * 2 - 1;
            } while (u * u + v * v > 1);
            r2 = u * u + v * v;
            float x = 2 * u * sqrt(1 - r2);
            float y = 2 * v * sqrt(1 - r2);
            float z = 1 - 2 * r2;
            return Vector3(x,y,z);
        }
    };
    typedef Vector3 Color;


}
#endif
