#include "bezier3.h"

namespace Raytracer{
    float B(int i,float t){
        if(i == 0)
            return (1.0 - t) * (1.0 - t) * (1.0 - t);
        if(i == 1)
            return 3.0 * (1.0 - t) * (1.0 - t) * t;
        if(i == 2)
            return 3.0 * (1.0 - t) * t * t;
        if(i == 3)
            return t * t * t;
        return 0.0;
    }
    float dB(int i,float t){
        if(i == 0)
            return -3.0 * (1.0 - t) * (1.0 - t);
        if(i == 1)
            return 3.0 * (1.0 - t) * ( 1.0 - 3.0 * t);
        if(i == 2)
            return 3.0 * t * (2.0 - 3.0 * t);
        if(i == 3)
            return 3.0 * t * t;
        return 0.0;
    }
    float Rand(float range){
        return rand() * range / RAND_MAX;
    }
    Vector3f F(Vector3f& xi, Vector3f* Patch, Vector3f& p, Vector3f& w){
        Vector3f s = -p - w * xi(0);
        float B_i[] = {B(0,xi(1)), B(1,xi(1)), B(2,xi(1)), B(3,xi(1))};
        float B_j[] = {B(0,xi(2)), B(1,xi(2)), B(2,xi(2)), B(3,xi(2))};
        for(int i = 0; i < 4; i ++)
            for(int j = 0; j < 4; j ++)
                s += B_i[i] * B_j[j] * Patch[i * 4 + j];
        return s;
    }

    Vector3f aF_au(Vector3f& xi, Vector3f* Patch){
        Vector3f s(0.0,0.0,0.0);
        float dB_i[] = {dB(0,xi(1)), dB(1,xi(1)), dB(2,xi(1)), dB(3,xi(1))};
        float B_j[] = {B(0,xi(2)), B(1,xi(2)), B(2,xi(2)), B(3,xi(2))};
        for(int i = 0; i < 4;i ++)
            for(int j = 0 ; j < 4 ; j ++)
                s += dB_i[i] * B_j[j] * Patch[i * 4 + j];
        return s;
    }
    Vector3f aF_av(Vector3f& xi, Vector3f* Patch){
        Vector3f s(0.0,0.0,0.0);

        float B_i[] = {B(0,xi(1)), B(1,xi(1)), B(2,xi(1)), B(3,xi(1))};
        float dB_j[] = {dB(0,xi(2)), dB(1,xi(2)), dB(2,xi(2)), dB(3,xi(2))};
        for(int i = 0; i < 4;i ++)
            for(int j = 0 ; j < 4 ; j ++)
                s += B_i[i] * dB_j[j] * Patch[i * 4 + j];
        return s;
    }

    Vector3f NT(Vector3f& xi, Vector3f* Patch, Vector3f& p, Vector3f& w){
        Matrix3f F_;
        F_ << -w , aF_au(xi, Patch) , aF_av(xi, Patch);
        return xi - F_.inverse() * F(xi, Patch, p, w);
    }
    bool NTIteration(Vector3f &x0 , Vector3f* Patch, Vector3f& p, Vector3f& w, Vector3f& res){
        Vector3f last = x0;
        for(int i = 0; i < ITERDEPTH; i ++){
            x0 = NT(x0, Patch,p ,w);

            if(x0(1) < -0.5 || x0(1) > 1.5 || x0(2) < -0.5 || x0(2) > 1.5)
                return false;
            if((last - x0).dot(last - x0) < 1e-4){
                res = x0;
                //cout << res(0) << ' ' << res(1) << ' ' << res(2) << endl;
                return (res(0) > 0 &&  x0(1) >= -0.01 && x0(1) <= 1.0 + 0.01 && x0(2) >= -0.01 && x0(2) <= 1.0 + 0.01) ? true : false;
            }
            last = x0;
        }
        return false;
    }

    Vector3 P(float u, float v, Vector3f* Patch){
        Vector3f s(0.0,0.0,0.0);
        float Bu[] = {B(0,u),B(1,u),B(2,u),B(3,u)};
        float Bv[] = {B(0,v),B(1,v),B(2,v),B(3,v)};
        for(int i = 0; i < 4; i ++)
            for(int j = 0; j < 4; j ++)
                s += Bu[i] * Bv[j] * Patch[i * 4 + j];
        return Vector3(s(0), s(1), s(2));
    }

    Vector3 aP_au(float u, float v, Vector3f* Patch){
        Vector3f s(0.0,0.0,0.0);
        float dBu[] = {dB(0,u),dB(1,u),dB(2,u),dB(3,u)};
        float Bv[] = {B(0,v),B(1,v),B(2,v),B(3,v)};
        for(int i = 0; i < 4; i ++)
            for(int j = 0 ; j < 4 ; j ++)
                s += dBu[i] * Bv[j] * Patch[i * 4 + j];
        return Vector3(s(0), s(1), s(2));
    }
    Vector3 aP_av(float u, float v, Vector3f* Patch){
        Vector3f s(0.0,0.0,0.0);
        float Bu[] = {B(0,u),B(1,u),B(2,u),B(3,u)};
        float dBv[] = {dB(0,v),dB(1,v),dB(2,v),dB(3,v)};
        for(int i = 0; i < 4;i ++)
            for(int j = 0 ; j < 4 ; j ++)
                s += Bu[i] * dBv[j] * Patch[i * 4 + j];
        return Vector3(s(0), s(1), s(2));
    }
    Vector3f Initx0(const Vector3f& p, const Vector3f& w, Vector3f* Patch){
        float minD2 = INF;
        float t = 0.0;
        int Patch_Num = -1;
        for(int i = 0 ; i < 16 ; i ++){
            Vector3f from_o = Patch[i] - p;
            float temp_t = from_o.dot(w);
            float dis2 = from_o.dot(from_o) - temp_t * temp_t;
            if(dis2 < minD2){
                minD2 = dis2;
                t = temp_t;
                Patch_Num = i;
            }
        }
        return Vector3f(t, Patch_Num / 4 * (1 / 3.) , Patch_Num % 4 * (1 / 3.));
    }

    Bezier3::Bezier3():m_Patches(0),m_Patch(NULL),aabbs(NULL){}
    int Bezier3::GetType()const {return BEZIER;}

    int Bezier3::Intersect(const Ray& a_Ray, Intersection* isc) const {
        Vector3 dir = a_Ray.direction;
        Vector3 o = a_Ray.origin;
        float a_Dist = a_Ray.distance;
        Vector3f p(o.x, o.y, o.z);
        Vector3f w(dir.x, dir.y, dir.z);
        Vector3f res;
        float u, v;
        int Patch_Hit = -1;
        int result = MISS;
        //cout << m_Patches << endl;
        Intersection* aabbisc = new Intersection;
        Vector3f x0;
        for(int i = 0; i < m_Patches; i ++){
            //for(int j = 0; j < ITERTIMES ; j ++){
            if(aabbs[i].Intersect(a_Ray,aabbisc)){
                x0 = Vector3f(aabbisc -> distance,RAND(), RAND());
                if(NTIteration(x0, m_Patch + 16 * i, p, w, res)){
                    if(a_Dist > res(0)){
                        Patch_Hit = i;
                        a_Dist = res(0);
                        u = res(1);
                        v = res(2);
                        result = HIT;
                    }
                }
                else{
                    for(int j = 0; j < ITERTIMES ; j ++){
                        x0(1) = RAND();
                        x0(2) = RAND();
                        if(NTIteration(x0, m_Patch + 16 * i, p, w, res)){
                            if(a_Dist > res(0)){
                                Patch_Hit = i;
                                a_Dist = res(0);
                                u = res(1);
                                v = res(2);
                                result = HIT;
                                break;
                            }
                        }
                    }
                }
            }
        }
        if(result){
            Vector3f* m_Patch_Hit = m_Patch + 16 * Patch_Hit;
            isc -> result = HIT;
            //Change Dist
            isc -> distance = a_Dist;
            isc -> parameter = Vector3(u, v, 0.0);
            Vector3 a_Normal = aP_au(u, v, m_Patch_Hit).Cross(aP_av(u, v, m_Patch_Hit));
            isc -> normal = a_Normal.Normal();
            isc -> intersection = P(u, v, m_Patch_Hit);
        }
        return result;
    }
    void Bezier3::LoadModel(const string& FileName){
        ifstream in;
        in.open(FileName,ios::in);
		in >> m_Patches;
        m_Patch = new Vector3f[m_Patches * 16];
        aabbs = new AABB[m_Patches];
        int m, n;
        float x, y, z;
        for(int i = 0; i < m_Patches; i ++){
            in >> m >> n;
            float minx = INF,miny = INF,minz = INF;
            float maxx = -INF,maxy = -INF,maxz = -INF;
            for(int j = 0; j < 16; j ++){
                in >> x >> y >> z;
                x *= 1.5;
                y *= 1.5;
                z *= 1.5;
                swap(y,z);
                x -= 5;
                y -= 5;
                minx = min(x,minx);
                miny = min(y,miny);
                minz = min(z,minz);
                maxx = max(x,maxx);
                maxy = max(y,maxy);
                maxz = max(z,maxz);

                m_Patch[i * 16 + j] = Vector3f(x, y, z);
            }
            aabbs[i] = AABB(Vector3(minx,miny,minz),Vector3(maxx,maxy,maxz));
        }
    }
    void Bezier3::ReadModel(ifstream& fin){
        string op,file;
        while(fin >> op){
            if(op == "end") break;
            if(op == "bpt") fin >> file;
        }
        LoadModel(file);
    }
}
