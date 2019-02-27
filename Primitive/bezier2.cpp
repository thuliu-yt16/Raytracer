#include "bezier2.h"
namespace Raytracer{

    Vector2f P(float t, Vector2f* p){
        return B(0,t) * p[0] + B(1,t) * p[1] + B(2,t) * p[2] + B(3,t) * p[3];
    }
    Vector2f dPdt(float t, Vector2f* p){
        return dB(0,t) * p[0] + dB(1,t) * p[1] + dB(2,t) * p[2] + dB(3,t) * p[3];
    }
    Vector3f S(float t, float theta, Vector2f* p){
        Vector2f pt = P(t,p);
        return Vector3f(pt(0) * cos(theta), pt(0) * sin(theta), pt(1));
    }
    Vector3f pSpt(float t, float theta, Vector2f* p){
        Vector2f dpdt = dPdt(t,p);
        return Vector3f(dpdt(0) * cos(theta), dpdt(0) * sin(theta), dpdt(1));
    }
    Vector3f pSptheta(float t, float theta, Vector2f* p){
        Vector2f pt = P(t,p);
        return Vector3f(-pt(0) * sin(theta), pt(0) * cos(theta),0);
    }
    Vector3f F(const Vector3f& xn, Vector2f* p, const Vector3f& o, const Vector3f& d){
        Vector3f s = - o - d * xn(0);
        return S(xn(1), xn(2),p) - o - d * xn(0);
    }
    Vector3f NT(const Vector3f& xn, Vector2f* p, const Vector3f& o, const Vector3f& d){
        Matrix3f F_;
        F_ << -d, pSpt(xn(1), xn(2), p), pSptheta(xn(1), xn(2), p);
        return xn - F_.inverse() * F(xn, p, o, d);
    }

    bool NTIteration(Vector3f& x0, Vector2f* p, const Vector3f& o, const Vector3f& d, Vector3f& res){
        for(int i = 0; i < ITERDEPTH; i ++){
            Vector3f f = F(x0, p, o, d);
            if(f.dot(f) < EPSILON * EPSILON){
                res = x0;
                return true;
            }
            x0 = NT(x0, p, o, d);
            if(x0(1) < -0.5 || x0(1) > 1.5)
                break;
        }
        return false;
    }

    Bezier2::Bezier2(){scale_matrix = Matrix2f::Identity();}
    Bezier2::Bezier2(const Vector3& _bc, const Vector3& _u, const Vector3& _v):bc(_bc),u(_u),v(_v),w(_u.Cross(_v).Normal()),patch(NULL),ocbbs(NULL){}

    int Bezier2::GetType() const{return BEZIER;}

    int Bezier2::Intersect(const Ray& a_Ray, Intersection* isc) const{
        Vector3 adir = a_Ray.direction;
        Vector3 aorigin = a_Ray.origin;
        Vector3 dir = RelativeCoordinates(adir);
        Vector3 origin = RelativeCoordinates(aorigin - bc);

        float tmax = a_Ray.distance;
        Vector3f o(origin.x,origin.y,origin.z);
        Vector3f d(dir.x,dir.y,dir.z);
        Vector3f res;
        float t, theta;
        int patch_hit = -1;
        int result = MISS;
        Intersection ocbbisc;
        Vector3f x0;
        for(int i = 0; i < patches; i ++){
            if(ocbbs[i].Intersect(Ray(origin, dir, tmax), &ocbbisc)){
                for(int j = 0; j < ITERTIMES; j ++){
                    x0 = Vector3f(ocbbisc.distance, RAND(), ocbbisc.parameter.x * 2 * PI);
                    if(NTIteration(x0, patch + 4 * i, o, d, res)){
                        if(res(0) > EPSILON && tmax > res(0) && res(1) > -EPSILON && res(1) < 1 + EPSILON){
                            patch_hit = i;
                            tmax = res(0);
                            t = res(1);
                            theta = res(2);
                            result = HIT;
                        }
                    }
                }
            }
        }
        if(result){
            Vector2f* p = patch + patch_hit * 4;
            isc -> distance = tmax;
            isc -> intersection = aorigin + adir * tmax;
            Vector3f psptheta = pSptheta(t, theta, p);
            Vector3f pspt = pSpt(t,theta,p);
            Vector3 n = Vector3(psptheta(0), psptheta(1), psptheta(2)).Cross(Vector3(pspt(0),pspt(1),pspt(2))).Normal();
            isc -> result = result = n.Dot(dir) < 0 ? HIT : INPRIM;
            isc -> normal = AbsoluteCoordinates(n);
            theta /= (2 * PI);
            theta -= floor(theta);
            isc -> parameter = Vector3(1 - (t + patch_hit) * 1.0 / patches, theta,0);
        }
        return result;

    }
    void Bezier2::LoadModel(const string& FileName){
        float unit;
        ifstream in;
        in.open(FileName,ios::in);
        in >> patches;
        patch = new Vector2f[patches * 4];
        ocbbs = new OCBB[patches];
        float x,y;
        for(int i = 0; i < patches; i ++){
            float maxx = -INF;
            float minx = INF;
            float miny = INF;
            float maxy = -INF;
            for(int p = 0; p < 4; p ++){
                in >> x >> y;
                Vector2f r = scale_matrix * Vector2f(x,y);
                x = r[0];
                y = r[1];
                maxx = max(maxx,x);
                minx = min(minx,x);
                miny = min(miny,y);
                maxy = max(maxy,y);
                patch[i * 4 + p] = Vector2f(x,y);
            }
            ocbbs[i] = OCBB(Vector3(0,0,miny), Vector3(1,0,0), Vector3(0,1,0), maxy - miny, 0, maxx);
        }
    }
    Vector3 Bezier2::RelativeCoordinates(const Vector3& p) const{
        return Vector3(u.Dot(p), v.Dot(p), w.Dot(p));
    }
    Vector3 Bezier2::AbsoluteCoordinates(const Vector3& p) const{
        return p.x * u + p.y * v + p.z * w;
    }
    void Bezier2::ReadModel(ifstream& fin){
        string op, file;
        float angle;
        Matrix3f rotate_matrix = Matrix3f::Identity();
        while(fin >> op){
            if(op == "end") break;
            if(op == "bpt") fin >> file;
            if(op == "center") fin >> bc;
            if(op == "u") fin >> u;
            if(op == "v") fin >> v;
            if(op == "Rotate"){
                while(fin >> op){
                    if(op == "end") break;
                    if(op == "x"){
                        fin >> angle;
                        float cosangle = cos(angle * PI / 180);
                        float sinangle = sin(angle * PI / 180);
                        Matrix3f m;
                        m << 1, 0, 0,
                            0, cosangle, -sinangle,
                            0, sinangle, cosangle;
                        rotate_matrix.applyOnTheLeft(m);
                    }
                    if(op == "y"){
                        fin >> angle;
                        float cosangle = cos(angle * PI / 180);
                        float sinangle = sin(angle * PI / 180);
                        Matrix3f m;
                        m << cosangle, 0, sinangle,
                            0, 1, 0,
                            -sinangle, 0, cosangle;
                        rotate_matrix.applyOnTheLeft(m);
                    }
                    if(op == "z"){
                        fin >> angle;
                        float cosangle = cos(angle * PI / 180);
                        float sinangle = sin(angle * PI / 180);
                        Matrix3f m;
                        m << cosangle, -sinangle, 0,
                            sinangle, cosangle, 0,
                            0, 0, 1;
                        rotate_matrix.applyOnTheLeft(m);
                    }
                }
            }
            if(op == "Scale"){
                float sx = 1, sy = 1;
                while(fin >> op){
                    if(op == "end") break;
                    if(op == "x") fin >> sx;
                    if(op == "y") fin >> sy;
                }
                Matrix2f m;
                m << sx, 0, 0, sy;
                scale_matrix.applyOnTheLeft(m);
            }
        }
        u = (rotate_matrix * u).Normal();
        v = (rotate_matrix * v).Normal();
        w = u.Cross(v).Normal();
        LoadModel(file);
    }

}
