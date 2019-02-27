#ifndef MODEL_H
#define MODEL_H
#include "all.h"
#include "ray.h"
#include "vector3.h"
#include "intersection.h"
namespace Raytracer{
    class Model{
    public:
        enum{PLANE,SPHERE,AABB_,BEZIER,MESH,OCBB_};
        virtual int GetType() const = 0;
        virtual int Intersect(const Ray& a_Ray,Intersection* isc) const = 0;
        virtual void LoadModel(const string& FileName);
        virtual void ReadModel(ifstream& fin) = 0;
    };

    class Sphere:public Model{
    public:
        Sphere();
        Sphere(const Vector3& a_Center, float a_Radius);
        int GetType() const;
        int Intersect(const Ray& a_Ray,Intersection* isc) const;
        Vector3 GetCenter() const;
        float GetRadius() const;
        void ReadModel(ifstream& fin);
    private:
        float m_Radius,m_SqRadius,m_RRadius;
        Vector3 m_Center;
    };

    class Plane:public Model{
    public:
        Plane();
        Plane(const Vector3& a_Normal, float a_Distance, const Vector3& _u = Vector3(), const Vector3& _v = Vector3(), float b = 10);
        int GetType() const;
        int Intersect(const Ray& a_Ray,Intersection* isc) const;
        float GetDistance() const;
        void ReadModel(ifstream& fin);
    private:
        float m_Distance;
        float block;
        Vector3 u, v, center;
        Vector3 m_Normal;
    };
    class AABB:public Model{
    public:
        AABB();
        AABB(const Vector3& lb, const Vector3& ru);
        int GetType() const;
        void Set(const Vector3& Min, const Vector3& Max);
        int Intersect(const Ray& a_Ray,Intersection* isc) const;
        int Intersect(const Ray& ray) const;
        void Expand(const AABB& aabb);
        AABB Split(float splitPoint, int axis, int index) const;
        float SurfaceArea(int axis) const;
        bool inBox(const Vector3& p) const;
        Vector3 minxyz,maxxyz;
        void ReadModel(ifstream& fin);
        friend ostream& operator << (ostream& out, const AABB& bb){
            out << "AABB:" << endl;
            out << "\t[ " << bb.minxyz.x << " , " << bb.maxxyz.x << " ]" << endl;
            out << "\t[ " << bb.minxyz.y << " , " << bb.maxxyz.y << " ]" << endl;
            out << "\t[ " << bb.minxyz.z << " , " << bb.maxxyz.z << " ]" << endl;
            return out;
        }
    };
    class OCBB:public Model{
    public:
        OCBB();
        OCBB(const Vector3& bc, const Vector3& dir, float h, float r1, float r2);
        OCBB(const Vector3& bc, const Vector3& x, const Vector3& y, float h, float r1, float r2);
        int GetType() const;
        int Intersect(const Ray& a_Ray, Intersection* isc) const;
        Vector3 center,w,u,v;
        float H,R1,R2,sqrR1,sqrR2;
        Vector3 RelativeCoordinates(const Vector3& p) const;
        bool inBox(const Vector3& p) const;
        void ReadModel(ifstream& fin);
        friend ostream& operator << (ostream& out, const OCBB& bb){
            out << "OCBB:" << endl;
            out << "\tBottonCenter: " << bb.center << endl;
            out << "\tu: " << bb.u << endl;
            out << "\tv: " << bb.v << endl;
            out << "\tw: " << bb.w << endl;
            out << "\tH: " << bb.H << " R1: " << bb.R1 << " R2: " << bb.R2 <<endl;
            return out;
        }

    };

}
#endif
