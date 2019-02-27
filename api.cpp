#include "api.h"
#include "camera.h"
#include "engine.h"
#include "image.h"
#include "intersection.h"
#include "vector3.h"
#include "ray.h"
#include "scene.h"
#include "model.h"
#include "primitive.h"
#include "material.h"
#include "bezier3.h"
#include "light.h"
#include "ppmrender.h"

namespace Raytracer{
    void run(){
        srand(unsigned(time(NULL)));
        Engine* engine = new Engine;
        ifstream in;
        in.open("../Resources/scene.txt",ios::in);
        string op;
        while(in >> op){
            if(op == "All") engine -> ReadAll(in);
        }
        PPMRender* ppm = new PPMRender();
        ppm -> Init(engine);
        ppm -> Render();
    }
}
