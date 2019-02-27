#include "primitive.h"

namespace Raytracer{
    Primitive::Primitive():model(NULL),material(NULL),Id(-1){}
    Primitive::Primitive(Model* _model, Material* _material, int id):model(_model),material(_material),Id(id){};
    Model* Primitive::GetModel() const{return model;}
    Material* Primitive::GetMaterial() const{return material;}

    void Primitive::ReadPrimitive(ifstream& fin){
        string op;
        string type;
        while(fin >> op){
            if(op == "end") break;
            if(op == "Material"){
                material = new Material;
                material -> ReadMaterial(fin);
            }
            if(op == "Model"){
                fin >> type;
                if(type == "Plane") model = new Plane;
                if(type == "Sphere") model = new Sphere;
                if(type == "AABB") model = new AABB;
                if(type == "OCBB") model = new OCBB;
                if(type == "Bezier3") model = new Bezier3;
                if(type == "Bezier2") model = new Bezier2;
                if(type == "Mesh") model = new Mesh;
                model -> ReadModel(fin);
            }
            if(op == "Id") fin >> Id;
        }
    }
    Primitive::~Primitive(){delete model;delete material;}
}
