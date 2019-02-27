#include "Vector3.h"

namespace Raytracer{
    Vector3::Vector3():x(0.0),y(0.0),z(0.0){};
    Vector3::Vector3(float a_X,float a_Y,float a_Z ):x(a_X),y(a_Y),z(a_Z){}
    Vector3::Vector3(const Vector3f& v):x(v(0)),y(v(1)),z(v(2)){}

    void Vector3::Set(float a_X,float a_Y,float a_Z){x = a_X;y = a_Y;z = a_Z;}
    void Vector3::Normalize(){float l = 1.0 / Length();x *= l;y *= l;z *= l;}
    float Vector3::Length() const {return (float)sqrt( x * x + y * y + z * z );}
    float Vector3::SqrLength() const {return x * x + y * y + z * z;}
    float Vector3::Dot(Vector3 a_V) const {return x * a_V.x + y * a_V.y + z * a_V.z;}
    Vector3 Vector3::Normal() const{
        float l = Length();
        return Vector3(x / l, y / l, z / l);
    }
    float Vector3::Max() const{
        return max(x,max(y,z));
    }
    Vector3 Vector3::Cross(const Vector3& b) const {return Vector3(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x );}
    void Vector3::operator += (const Vector3& a_V) {x += a_V.x;y += a_V.y;z += a_V.z;}
    void Vector3::operator += (Vector3* a_V){x += a_V->x;y += a_V->y;z += a_V->z;}
    void Vector3::operator -= (const Vector3& a_V){x -= a_V.x;y -= a_V.y;z -= a_V.z;}
    void Vector3::operator -= (Vector3* a_V){x -= a_V->x;y -= a_V->y;z -= a_V->z;}
    void Vector3::operator *= (float f){x *= f;y *= f;z *= f;}
    void Vector3::operator *= (const Vector3& a_V){x *= a_V.x;y *= a_V.y;z *= a_V.z;}
    void Vector3::operator *= (Vector3* a_V){x *= a_V->x;y *= a_V->y;z *= a_V->z;}
    void Vector3::operator /= (float f){x /= f;y /= f;z /= f;}
    Vector3 Vector3::operator- () const {return Vector3(-x, -y, -z);}

    istream& operator >> (istream& in, Vector3& v){
        in >> v.x >> v.y >> v.z;
        return in;
    }
    ostream& operator << (ostream& out, const Vector3& v){
        out << "[ " << v.x << " , " << v.y << " , " << v.z << " ]";
		return out;
    }
    Vector3 operator + (const Vector3& v1,const Vector3& v2) {return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);}
    Vector3 operator - (const Vector3& v1, const Vector3& v2){return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);}
    Vector3 operator + (const Vector3& v1, Vector3* v2){return Vector3(v1.x + v2->x, v1.y + v2->y, v1.z + v2->z);}
    Vector3 operator - (const Vector3& v1, Vector3* v2){return Vector3(v1.x - v2->x, v1.y - v2->y, v1.z - v2->z);}
    Vector3 operator * (const Vector3& v, float f){return Vector3(v.x * f, v.y * f, v.z * f);}
    Vector3 operator * (const Vector3& v1,const Vector3& v2){return Vector3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);}
    Vector3 operator * (float f, const Vector3& v){return Vector3(v.x * f, v.y * f, v.z * f);}
    Vector3 operator / (const Vector3& v, float f){
        return Vector3(v.x / f, v.y / f, v.z / f);
    }
    Vector3 operator * (const Matrix3f& m, const Vector3& v){
        return Vector3(m * Vector3f(v.x,v.y,v.z));
    }
    Vector3 Max(const Vector3& v1, const Vector3& v2){
        return Vector3(max(v1.x,v2.x),max(v1.y,v2.y),max(v1.z,v2.z));
    }
    Vector3 Min(const Vector3& v1, const Vector3& v2){
        return Vector3(min(v1.x,v2.x),min(v1.y,v2.y),min(v1.z,v2.z));
    }
    Vector3 Exp(const Vector3& v){
        return Vector3(expf(v.x),expf(v.y),expf(v.z));
    }
}
