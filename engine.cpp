#include "engine.h"

namespace Raytracer{
    Engine::Engine():LScene(NULL),LCamera(NULL){}
    void Engine::Init(Scene* scene, Camera* camera){
        LScene = scene;
        LCamera = camera;
    }
    Intersection Engine::firstIntersection(const Ray& ray){
        Intersection final_isc, isc;
        float dis = INF;

        for(auto primitive : LScene -> primitives){
            primitive -> GetModel() -> Intersect(ray,&isc);
            if((isc.result == HIT || isc.result == INPRIM) && isc.distance < dis){
                dis = isc.distance;
                final_isc = isc;
                final_isc.primitive = primitive;
            }
        }
        return final_isc;
    }

    Engine::~Engine(){}
    void Engine::GenerateImage(const string& filaname){
        LCamera -> image -> save(filaname.c_str());
    }
    void Engine::ReadAll(ifstream& fin){
        string op;
        while(fin >> op){
            if(op == "end") break;
            if(op == "Scene"){
                LScene = new Scene;
                LScene -> ReadScene(fin);
            }
            if(op == "Camera"){
                LCamera = new Camera;
                LCamera -> ReadCamera(fin);
            }
        }
    }
}
