#include "intersection.h"
namespace Raytracer{
    Intersection::Intersection():intersection(Vector3(0,0,0)), normal(Vector3(0,0,0)),parameter(Vector3(0,0,0)),distance(INF),result(MISS),primitive(NULL){}
    Intersection::Intersection(int a_Result, const Vector3& a_Intersection, const Vector3& a_Normal, const Vector3& a_Parameter, float a_Dist):
            result(a_Result), intersection(a_Intersection), normal(a_Normal), parameter(a_Parameter), distance(a_Dist){}
}
