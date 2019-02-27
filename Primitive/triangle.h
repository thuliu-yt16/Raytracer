#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "all.h"
#include "vector3.h"
#include "model.h"
namespace Raytracer{
    class Triangle{
    public:
        Vector3 a,b,c;
        Vector3 normal_a,normal_b,normal_c;
        AABB aabb;
        Triangle();
        Triangle(const Vector3& _a, const Vector3 &_b, const Vector3& _c);
        Triangle(const Vector3& _a, const Vector3 &_b, const Vector3& _c, const Vector3& n1, const Vector3& n2, const Vector3& n3);
        void SetVertex(const Vector3& p1, const Vector3& p2, const Vector3& p3);
        void SetNormal(const Vector3& n1, const Vector3& n2, const Vector3& n3);

        int Intersect(const Ray& ray, Intersection* isc) const;
        friend ostream& operator << (ostream& out, const Triangle& t){
            out << "Triangle:" << endl;
            out << "\tA: " << t.a << endl;
            out << "\tB: " << t.b << endl;
            out << "\tC: " << t.c << endl;
            out << "\tN_A:" << t.normal_a << endl;
            out << "\tN_B:" << t.normal_b << endl;
            out << "\tN_C:" << t.normal_c << endl;
            return out;
        }
    };
}
#endif
