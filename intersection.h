#ifndef INTERSECTION_H
#define INTERSECTION_H
#include "all.h"
#include "vector3.h"
namespace Raytracer{
    class Intersection{
    public:
        int result;
        float distance;
        Vector3 intersection;
        Vector3 normal;
        Vector3 parameter;
        Primitive* primitive;
        Intersection();
        Intersection(int a_Result,const Vector3& a_Intersection, const Vector3& a_Normal, const Vector3& a_Parameter, float a_Dist);
        friend ostream& operator << (ostream& out, const Intersection& isc){
            out << "Intersection :" << endl;
            out << "\tresult: " << isc.result << endl;
            out << "\tdistance : " << isc.distance << endl;
            out << "\tintersection : " << isc.intersection << endl;
            out << "\tnormal : " << isc.normal << endl;
            out << "\tparameter : " << isc.parameter << endl;
            return out;
        }
    };
}

#endif
