#include "mesh.h"

namespace Raytracer{
    Mesh::Mesh(){
        tree = MTree();
        scale_matrix = Matrix3f::Identity();
    }
    void Mesh::LoadModel(const string& Filename){
        ifstream in;
        in.open(Filename,ios::in);
        vector<Triangle*>& triangles = tree.triangles;
        vector<Vector3> vertexs;
        vector<Vector3> normals;
        string ch;
        char buffer[100];
        int v[3],vn[3];
        Vector3 t;
        while(in >> ch){
            if(ch == "v"){
                in >> t;
                t = scale_matrix * t;
                vertexs.push_back(t);
            }
            else if(ch == "vn"){
                in >> t;
                normals.push_back(t);
            }
            else if(ch == "f"){
                for(int i = 0; i < 3; i ++){
                    in >> buffer;
                    int index = 0;
                    v[i] = str2int(buffer,index);
                    index += 2;
                    vn[i] = str2int(buffer,index);
                }
                triangles.push_back(new Triangle(vertexs[v[0] - 1],vertexs[v[1] - 1],vertexs[v[2] - 1],normals[vn[0] - 1],normals[vn[1] - 1],normals[vn[2] - 1]));
            }
        }
        tree.Build();
        aabb = tree.root -> aabb;
        cout << aabb << endl;

    }
    int Mesh::GetType() const{return MESH;}
    int Mesh::Intersect(const Ray& a_Ray,Intersection* isc) const{
        Vector3 o = RelativeCoordinates(a_Ray.origin - bc);
        Vector3 d = RelativeCoordinates(a_Ray.direction);
        Ray ray(o,d,a_Ray.distance);
        Intersection isc1;
        int result = tree.Intersect(ray,&isc1);
        isc -> result = isc1.result;
        isc -> normal = AbsoluteCoordinates(isc1.normal);
        isc -> intersection = a_Ray.origin + isc1.distance * a_Ray.direction;
        isc -> distance = isc1.distance;
        isc -> parameter = isc1.parameter;
        return result;
    }
    void Mesh::ReadModel(ifstream& fin){
        string op,file;
        Matrix3f rotate_matrix = Matrix3f::Identity();
        float angle;
        while(fin >> op){
            if(op == "end") break;
            if(op == "obj") fin >> file;
            if(op == "u") fin >> u;
            if(op == "v") fin >> v;
            if(op == "center") fin >> bc;
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
                float sx = 1, sy = 1, sz = 1;
                while(fin >> op){
                    if(op == "end") break;
                    if(op == "x") fin >> sx;
                    if(op == "y") fin >> sy;
                    if(op == "z") fin >> sz;
                }
                Matrix3f m;
                m << sx, 0, 0,
                    0, sy, 0,
                    0, 0, sz;
                scale_matrix.applyOnTheLeft(m);
            }
        }
        u = (rotate_matrix * u).Normal();
        v = (rotate_matrix * v).Normal();
        w = u.Cross(v);
        LoadModel(file);
    }
    Vector3 Mesh::RelativeCoordinates(const Vector3& p) const{
        return Vector3(p.Dot(u), p.Dot(v), p.Dot(w));
    }
    Vector3 Mesh::AbsoluteCoordinates(const Vector3& p) const{
        return p.x * u + p.y * v + p.z * w;
    }

}
