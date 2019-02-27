#ifndef ENGINE_H
#define ENGINE_H
#include "all.h"
#include "scene.h"
#include "camera.h"
#include "ray.h"
#include "primitive.h"
#include "light.h"
#include "model.h"
#include "bezier3.h"
#include "material.h"
namespace Raytracer{
    class Engine{
    public:
        Scene* LScene;
        Camera* LCamera;
        Engine();
        void Init(Scene* scene,Camera* camera);
        ~Engine();
        Intersection firstIntersection(const Ray& ray);
        void GenerateImage(const string& filename);
        void ReadAll(ifstream& fin);
    };
}

#endif
