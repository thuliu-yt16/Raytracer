#include "scene.h"

namespace Raytracer{
    Scene::Scene(){
        primitives.clear();
        lights.clear();
    }
    void Scene::ReadScene(ifstream& fin){
        string op;
        Primitive* prim = NULL;
        Light* l = NULL;
        while(fin >> op){
            if(op == "end") break;
            if(op == "Primitive"){
                prim = new Primitive;
                prim -> ReadPrimitive(fin);
                primitives.push_back(prim);
            }
            if(op == "Light"){
                string type;
                fin >> type;
                if(type == "SpotLight") l = new SpotLight;
                if(type == "PointLight") l = new PointLight;
                if(type == "SurfaceLight") l = new SurfaceLight;
                l -> ReadLight(fin);
                lights.push_back(l);
            }
        }
    }
    void Scene::Init(){
            // ground
        Texture* floor_ = new Texture();
        floor_ -> LoadTexture("../Resources/Texture/floor5.ttr");
        Texture* sphere = new Texture();
        sphere -> LoadTexture("../Resources/Texture/sphere.ttr");
        primitives.push_back(
            new Primitive(new Plane(Vector3(0,1,0), 5, Vector3(1,0,0), Vector3(0,0,1)),
                new Material(0.0,0.0,0.9,0.1,1.0,Color(1,1,1), floor_),//Color(07333,0692,07451)),
                0
            )
        );
            //back
        primitives.push_back(
            new Primitive(new Plane(Vector3(0,0,1), 8, Vector3(1,0,0), Vector3(0,1,0)),
                new Material(0.0,0,0.9,0.1,1,Color(1,1,1),floor_),//Color(07333,0692,07451)),
                1
            )
        );
            //right
        primitives.push_back(
            new Primitive(new Plane(Vector3(-1,0,0),10, Vector3(0,1,0), Vector3(0,0,1)),
                new Material(0.0,0.1,0.8,0.1,1,Color(0.4549,0.4275,0.72156), floor_),
            2
            )
        );
            //left
        primitives.push_back(
            new Primitive(new Plane(Vector3(1,0,0),10, Vector3(0,1,0), Vector3(0,0,-1)),
                new Material(0.0,0,0.9,0.1,1,Color(0.72156,0.4275,0.4549), floor_),
                3
            )
        );
            //up
        primitives.push_back(
            new Primitive(new Plane(Vector3(0,-1,0),15),
                new Material(0.0,0,0.9,0.1,1,Color(1,1,1)),//Color(07333,0692,07451)),
                5
            )
        );

        lights.push_back(
            new SurfaceLight(Vector3(0,14.9999,2),Vector3(1,0,0),Vector3(0,0,1),80,4,3,Color(1,1,1))
        );

        primitives.push_back(
            new Primitive(new Sphere(Vector3(5.5,-2.5,-3),2.5),
                new Material(0.0,0.0,0.9,0.1,1.3333,Color(0,1,0.2),sphere),
                4
            )
        );

        Model* cup = new Bezier2(Vector3(-3,-5,1), Vector3(0,0,1).Normal(), Vector3(1,0,0));
        dynamic_cast<Bezier2*>(cup) -> LoadModel("../Resources/bowling.bpt");
        primitives.push_back(
            new Primitive(cup,
                          new Material(0.0,0.0,0.9,0.1,1.33,Color(0.76078,0.043,0.9333)),
                          13
                      )
        );
    }
    Scene::~Scene(){
        for(auto primitive: primitives){
            delete primitive;
        }
        for(auto light: lights){
            delete light;
        }
        primitives.clear();
        lights.clear();
    }
}
