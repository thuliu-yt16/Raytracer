#ifndef SCENE_H
#define SCENE_H
#include "all.h"
#include "primitive.h"
#include "light.h"
#include "material.h"
#include "model.h"
#include "bezier3.h"
#include "mesh.h"
#include "bezier2.h"
namespace Raytracer{
    class Scene{
    public:
        vector<Primitive*> primitives;
        vector<Light*> lights;
        Scene();
        void ReadScene(ifstream& fin);
        void Init();
        ~Scene();
    };
}

#endif
