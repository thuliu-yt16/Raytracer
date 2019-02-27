#include "model.h"
namespace Raytracer{
    void Model::LoadModel(const string& FileName){}

    Sphere::Sphere(){}
    Sphere::Sphere(const Vector3& a_Center, float a_Radius): m_Center(a_Center), m_Radius(a_Radius), m_SqRadius(a_Radius * a_Radius),m_RRadius(1.0f / a_Radius){}
    int Sphere::GetType() const {return SPHERE;}

    int Sphere::Intersect(const Ray& a_Ray,Intersection* isc) const {
        Vector3 o = a_Ray.origin;
        Vector3 dir = a_Ray.direction;
        float a_Dist = a_Ray.distance;
        Vector3 v = m_Center - o;
        float b = DOT(v, dir);
        float h = m_SqRadius + b * b - SQRLENGTH(v);

        int result = MISS;
        if(h > 0){
            h = sqrtf(h);
            float i1 = b - h;
            float i2 = b + h;
            if(i2 > 0){
                if(i1 > 0){
                    if(a_Dist > i1){
                        Vector3 p = o + i1 * dir;
                        isc -> intersection = p;
                        Vector3 n = (p - m_Center).Normal();
                        isc-> normal = n;
                        float phi = acos(n.y) / PI + 0.5;
                        float theta = atan(n.x / n.z) / (2 * PI) + (n.z > 0 ? 0.25 : 0.75);
                        isc -> parameter = Vector3(phi, theta, 0);
                        isc -> distance = i1;
                        result = HIT;
                    }
                }
                else{
                    if(a_Dist > i2){
                        Vector3 p = o + i2 * dir;
                        isc -> intersection = p;
                        Vector3 n = (p - m_Center).Normal();
                        isc-> normal = n;
                        float phi = acos(n.y) / PI + 0.5;
                        float theta = atan(n.x / n.z) / (2 * PI) + (n.z > 0 ? 0.25 : 0.75);
                        isc -> parameter = Vector3(phi, theta, 0);
                        isc -> distance = i2;
                        result = INPRIM;
                    }
                }
            }
        }
        isc -> result = result;
        return result;
    }
    Vector3 Sphere::GetCenter() const {return m_Center;}
    float Sphere::GetRadius() const {return m_Radius;}
    void Sphere::ReadModel(ifstream& fin){
        string op;
        while(fin >> op){
            if(op == "end") break;
            if(op == "R") fin >> m_Radius;
            if(op == "center") fin >> m_Center;
        }
        m_SqRadius = m_Radius * m_Radius;
        m_RRadius = 1.0 / m_Radius;
    }

    Plane::Plane(){}
    Plane::Plane(const Vector3& a_Normal, float a_Distance, const Vector3& _u, const Vector3& _v, float b):m_Normal(a_Normal), m_Distance(a_Distance), u(_u), v(_v), block(b){
        NORMALIZE(m_Normal);
        center = - m_Normal * m_Distance;
    }

    int Plane::GetType() const {return PLANE;}

    int Plane::Intersect(const Ray& a_Ray,Intersection* isc) const {
        /*
              o
              ^
              |
              |  } d
              |
          **********
         **********
        **********
        */
        Vector3 dir = a_Ray.direction;
        Vector3 o = a_Ray.origin;
        float a_Dist = a_Ray.distance;
        float d = - DOT(m_Normal, dir);
        int result = MISS;
        if(d != 0){
            float d_p = (DOT(o, m_Normal) + m_Distance) / d;
            if(d_p > 0 && d_p < a_Dist){
                Vector3 in = o + d_p * dir;
                isc -> intersection = in;
                isc -> normal = m_Normal;
                isc -> distance = d_p;
                float x = (in - center).Dot(u) / block;
                float y = (in - center).Dot(v) / block;
                x -= floor(x);
                y -= floor(y);
                isc -> parameter = Vector3(x,y,0);
                result = d > 0 ? HIT : INPRIM;
            }
        }
        isc -> result = result;
        return result;
    }

    float Plane::GetDistance() const {return m_Distance;}
    void Plane::ReadModel(ifstream& fin){
        string op;
        block = 10;
        while(fin >> op){
            if(op == "end") break;
            if(op == "distance") fin >> m_Distance;
            if(op == "normal") fin >> m_Normal;
            if(op == "u") fin >> u;
            if(op == "v") fin >> v;
            if(op == "block") fin >> block;
            if(op == "center") fin >> center;
        }
    }
    AABB::AABB():minxyz(Vector3(0,0,0)),maxxyz(Vector3(0,0,0)){}
    AABB::AABB(const Vector3& lb, const Vector3& ru):minxyz(lb - EPSILON * Vector3(1,1,1)), maxxyz(ru + EPSILON * Vector3(1,1,1)){}
    int AABB::GetType() const{return AABB_;}
    void AABB::Set(const Vector3& Min, const Vector3& Max){
        minxyz = Min - EPSILON * Vector3(1,1,1);
        maxxyz = Max + EPSILON * Vector3(1,1,1);
    }
    void AABB::Expand(const AABB& aabb) {
        minxyz = Min(minxyz,aabb.minxyz);
        maxxyz = Max(maxxyz,aabb.maxxyz);
    }
    AABB AABB::Split(float splitPoint, int axis, int index) const{
        AABB bb = *this;
        if(index == 0){
            bb.maxxyz.cell[axis] = min(bb.maxxyz.cell[axis],splitPoint);
        }
        else{
            bb.minxyz.cell[axis] = max(bb.minxyz.cell[axis],splitPoint);
        }
        return bb;
    }
    float AABB::SurfaceArea(int axis) const{
        if(axis == 0){
            return (maxxyz.y - minxyz.y) * (maxxyz.z - minxyz.z);
        }
        if(axis == 1){
            return (maxxyz.x - minxyz.x) * (maxxyz.z - minxyz.z);
        }
        if(axis == 2){
            return (maxxyz.y - minxyz.y) * (maxxyz.x - minxyz.x);
        }
        return 0;
    }
    bool AABB::inBox(const Vector3& p) const{
        return p.x >= minxyz.x && p.x <= maxxyz.x &&
               p.y >= minxyz.y && p.y <= maxxyz.y &&
               p.z >= minxyz.z && p.z <= maxxyz.z;
    }
    int AABB::Intersect(const Ray& ray) const{
        Vector3 origin = ray.origin;
        Vector3 dir = ray.direction;
        float rdist = ray.distance;

        int result = MISS;
        float xMinDis = (minxyz.x - origin.x) / dir.x;
        float xMaxDis = (maxxyz.x - origin.x) / dir.x;
        float yMinDis = (minxyz.y - origin.y) / dir.y;
        float yMaxDis = (maxxyz.y - origin.y) / dir.y;
        float zMinDis = (minxyz.z - origin.z) / dir.z;
        float zMaxDis = (maxxyz.z - origin.z) / dir.z;
        if(xMinDis > xMaxDis) swap(xMinDis, xMaxDis);
        if(yMinDis > yMaxDis) swap(yMinDis, yMaxDis);
        if(zMinDis > zMaxDis) swap(zMinDis, zMaxDis);
        float tNear = max(xMinDis,max(yMinDis,zMinDis));
        float tFar = min(xMaxDis,min(yMaxDis,zMaxDis));
        if(tNear < tFar){
            if(tNear > 0){
                if(tNear < rdist)
                    result = HIT;
            }
            else if(0 < tFar && tFar < rdist){
                result = INPRIM;
            }
        }
        return result;
    }
    int AABB::Intersect(const Ray& a_Ray, Intersection* isc) const{
        Vector3 origin = a_Ray.origin;
        Vector3 dir = a_Ray.direction;
        float rdist = a_Ray.distance;
        int result = MISS;
        float xMinDis = dir.x != 0.0f ? (minxyz.x - origin.x) / dir.x : INF;
        float xMaxDis = dir.x != 0.0f ? (maxxyz.x - origin.x) / dir.x : INF;
        float yMinDis = dir.y != 0.0f ? (minxyz.y - origin.y) / dir.y : INF;
        float yMaxDis = dir.y != 0.0f ? (maxxyz.y - origin.y) / dir.y: INF;
        float zMinDis = dir.z != 0.0f ? (minxyz.z - origin.z) / dir.z : INF;
        float zMaxDis = dir.z != 0.0f ? (maxxyz.z - origin.z) / dir.z: INF;
        if(xMinDis > xMaxDis) swap(xMinDis, xMaxDis);
        if(yMinDis > yMaxDis) swap(yMinDis, yMaxDis);
        if(zMinDis > zMaxDis) swap(zMinDis, zMaxDis);
        float tNear = max(xMinDis,max(yMinDis,zMinDis));

        float tFar = min(xMaxDis,min(yMaxDis,zMaxDis));
        if(tNear < tFar){
            if(tNear > 0){
                if(tNear < rdist){
                    result = HIT;
                    isc -> distance = tNear;
                    isc -> intersection = origin + dir * tNear;
                    isc -> result = HIT;
                    Vector3 inter = isc -> intersection;
                    for(int i = 0; i < 3; i ++){
                        if(abs(inter.cell[i] - minxyz.cell[i] < EPSILON)){
                            isc -> normal = -1 * Vector3::Unit(i);
                            break;
                        }
                        if(abs(inter.cell[i] - maxxyz.cell[i] < EPSILON)){
                            isc -> normal = Vector3::Unit(i);
                            break;
                        }
                    }
                }
            }
            else{
                if(0 < tFar && tFar < rdist){
                    result = INPRIM;
                    isc -> distance = tFar;
                    isc -> intersection = origin + dir * tFar;
                    isc -> result = INPRIM;
                    Vector3 inter = isc -> intersection;
                    for(int i = 0; i < 3; i ++){
                        if(abs(inter.cell[i] - minxyz.cell[i] < EPSILON)){
                            isc -> normal = -1 * Vector3::Unit(i);
                            break;
                        }
                        if(abs(inter.cell[i] - maxxyz.cell[i] < EPSILON)){
                            isc -> normal = Vector3::Unit(i);
                            break;
                        }
                    }
                }
            }
            return result;
        }
    }
    void AABB::ReadModel(ifstream& fin){
        string op;
        while(fin >> op){
            if(op == "end") break;
            if(op == "min") fin >> minxyz;
            if(op == "max") fin >> maxxyz;
        }
    }
    OCBB::OCBB(){}
    int OCBB::GetType() const{return OCBB_;}
    OCBB::OCBB(const Vector3& bc, const Vector3& dir, float h, float r1, float r2):
        center(bc - EPSILON * dir),w(dir),H(h + 2 * EPSILON),R1(r1 - EPSILON),R2(r2 + EPSILON){
            R1 = max(0.0,R1);
            sqrR1 = R1 * R1;
            sqrR2 = R2 * R2;
            if(abs(dir.z) < EPSILON){
                u = Vector3(dir.y, -dir.x, 0).Normal();
            }
            else{
                u = Vector3(0, dir.z, -dir.y).Normal();
            }
            v = w.Cross(u);
        }
    OCBB::OCBB(const Vector3& bc, const Vector3& x, const Vector3& y, float h, float r1, float r2):
        center(bc - EPSILON * x.Cross(y).Normal()), u(x), v(y), w(x.Cross(y).Normal()), H(h + 2 * EPSILON), R1(r1 - EPSILON), R2(r2 + EPSILON){
            R1 = max(0.0, R1);
            sqrR1 = R1 * R1;
            sqrR2 = R2 * R2;
        }
    OCBB::Intersect(const Ray& a_Ray, Intersection* isc) const{
        Vector3 Rp = a_Ray.origin;
        float tmax = a_Ray.distance;
        Vector3 Rd = a_Ray.direction;
        Vector3 p = RelativeCoordinates(Rp - center);
        Vector3 d = RelativeCoordinates(Rd);
        float a = d.x * d.x + d.y * d.y;
        float a_ = 0.5 / a;
        float b = 2 * (p.x * d.x + p.y * d.y);
        float c1 = p.x * p.x + p.y * p.y - sqrR2;
        float c2 = c1 + sqrR2 - sqrR1;
        int result = MISS;
        float t = tmax;
        Vector3 risc;
        Vector3 temp_isc;
        Vector3 normal;
        bool hitcircle = false;
        if(abs(a) > EPSILON){
            float delta1 = b * b - 4 * a * c1;
            if(delta1 >= 0){
                delta1 = sqrt(delta1);
                float t11 = (-b - delta1) * a_;
                if(t11 > 0 && t11 < t){
                    temp_isc = p + d * t11;
                    if(temp_isc.z <= H && temp_isc.z >= 0){
                        isc -> intersection = Rp + Rd * t11;
                        isc -> normal = (temp_isc.x * u + temp_isc.y * v).Normal();
                        isc -> distance = t11;
                        float theta = temp_isc.y > 0 ? acos(temp_isc.x / R2) : (2 * PI - acos(temp_isc.x / R2));
                        isc -> parameter = Vector3(theta * 0.5 / PI, temp_isc.z / H, 0);
                        isc -> result = inBox(Rp) ? INPRIM : HIT;
                        return isc -> result;
                    }
                }
                float delta2 = b * b - 4 * a * c2;
                if(delta2 >= 0){
                    delta2 = sqrt(delta2);
                    float t21 = (-b - delta2) * a_;
                    float t22 = (-b + delta2) * a_;
                    if(t21 > 0 && t21 < t){
                        temp_isc = p + d * t21;
                        if(temp_isc.z <= H && temp_isc.z >= 0){
                            t = t21;
                            risc = temp_isc;
                            normal = - (temp_isc.x * u + temp_isc.y * v).Normal();
                            result = HIT;
                        }
                    }
                    if(t22 > 0 && t22 < t){
                        temp_isc = p + d * t22;
                        if(temp_isc.z <= H && temp_isc.z >= 0){
                            t = t22;
                            risc = temp_isc;
                            normal = - (temp_isc.x * u + temp_isc.y * v).Normal();
                            result = HIT;
                        }
                    }
                }
                float t12 = (-b + delta1) * a_;
                if(t12 > 0 && t12 < t){
                    temp_isc = p + d * t12;
                    if(temp_isc.z <= H && temp_isc.z >= 0){
                        t = t12;
                        risc = temp_isc;
                        normal = (temp_isc.x * u + temp_isc.y * v).Normal();
                        result = HIT;
                    }
                }
            }
            float tb = -p.z / d.z;
            float th = (H - p.z) / d.z;
            if(tb > 0 && tb < t){
                temp_isc = p + d * tb;
                float l = temp_isc.x * temp_isc.x + temp_isc.y * temp_isc.y;
                if(l <= sqrR2 && l >= sqrR1){
                    hitcircle = true;
                    t = tb;
                    risc = temp_isc;
                    normal = -w;
                    result = HIT;
                }
            }
            if(th > 0 && th < t){
                temp_isc = p + d * th;
                float l = temp_isc.x * temp_isc.x + temp_isc.y * temp_isc.y;
                if(l <= sqrR2 && l >= sqrR1){
                    hitcircle = true;
                    t = th;
                    risc = temp_isc;
                    normal = w;
                    result = HIT;
                }
            }
            if(result == HIT){
                isc -> intersection = Rp + Rd * t;
                isc -> normal = normal;
                isc -> distance = t;
                float theta,R_;
                R_ = sqrt(risc.x * risc.x + risc.y * risc.y);
                theta = risc.y > 0 ? acos(risc.x / R_) : (2 * PI - acos(risc.x / R_));
                if(hitcircle){
                    isc -> parameter = Vector3(theta * 0.5 / PI, R_ / R2, 0);
                }
                else{
                    isc -> parameter = Vector3(theta * 0.5 / PI, risc.z / H, 0);
                }
                isc -> result = inBox(Rp) ? INPRIM : HIT;
                return isc -> result;
            }
        }
        return MISS;
    }
    Vector3 OCBB::RelativeCoordinates(const Vector3& p) const{
        return Vector3(u.Dot(p), v.Dot(p), w.Dot(p));
    }
    bool OCBB::inBox(const Vector3& p) const{
        Vector3 re = RelativeCoordinates(p - center);
        float l = re.x * re.x + re.y * re.y;
        //DEBUG(re);
        return l >= sqrR1 && l <= sqrR2 && re.z >= 0 && re.z <= H;
    }
    void OCBB::ReadModel(ifstream& fin){
        Vector3 center,w,u,v;
        float H,R1,R2,sqrR1,sqrR2;
        string op;
        int type = 0;//0 for only w;
        while(fin >> op){
            if(op == "end") break;
            if(op == "center") fin >> center;
            if(op == "dir") fin >> w;
            if(op == "H") fin >> H;
            if(op == "R1") fin >> R1;
            if(op == "R2") fin >> R2;
            if(op == "u") {type ++; fin >> u;}
            if(op == "v") {type ++; fin >> v;}
        }
        if(type == 0){
            if(abs(w.z) < EPSILON){
                u = Vector3(w.y, -w.x, 0).Normal();
            }
            else{
                u = Vector3(0, w.z, -w.y).Normal();
            }
            v = w.Cross(u).Normal();
        }
        else if(type == 2){
            w = u.Cross(v).Normal();
        }
        R1 = max(0.0, R1);
        sqrR1 = R1 * R1;
        sqrR2 = R2 * R2;
    }
}
