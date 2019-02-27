#ifndef PRIMITIVE_H
#define PRIMITIVE_H
#include "all.h"
#include "model.h"
#include "material.h"
#include "bezier3.h"
#include "bezier2.h"
#include "mesh.h"
namespace Raytracer{
    class Primitive{
    public:
        Model* model;
        Material* material;
        int Id;
        Primitive();
        Primitive(Model* _model, Material* _material,int id);
        Model* GetModel() const;
        Material* GetMaterial() const;
        void ReadPrimitive(ifstream& fin);
        ~Primitive();
    };
}

#endif
