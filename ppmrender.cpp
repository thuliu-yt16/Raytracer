#include "ppmrender.h"

namespace Raytracer{
    void PPMRender::Init(Engine* engine){
        LEngine = engine;
    }
    PPMRender::PPMRender():LEngine(NULL){
        tree = HKDTree();
    }
    void PPMRender::rayTrace(const Ray& ray, int x, int y,int depth, Color weight, float rindex,bool inPrimitive){
        if(weight.Length() < EPSILON || depth > TRACEDEPTH) return;
        Intersection isc = LEngine -> firstIntersection(ray);
        if(isc.result == MISS) return;
        Primitive* prim = isc.primitive;
        Material* pm = prim -> GetMaterial();

        Color mcolor = pm -> GetColor(isc.parameter.x, isc.parameter.y);
        if(pm -> diff + pm -> spec > 0){
            HitPoint h(isc.intersection - EPSILON * ray.direction, isc.normal, ray.direction, weight * mcolor , ORIGINR, prim, x, y, Color());
            tree.addHitPoint(h);
        }
        if(pm -> refr > 0){
            float opIndex = inPrimitive ? AIR_RINDEX : pm -> rIndex;
            rayTrace(ray.Refracted(isc.intersection, isc.normal * isc.result, rindex / opIndex), x, y, depth + 1, weight * pm -> refr * mcolor,
            opIndex, !inPrimitive);
        }
        if(pm -> refl > 0){
            rayTrace(ray.Reflected(isc.intersection, isc.normal), x, y, depth + 1, weight * pm -> refl * mcolor , rindex, inPrimitive);
        }
    }

    void PPMRender::photonTrace(const Photon& photon, int depth, float rindex, bool inPrimitive){
        if(photon.photonColor.Length() < EPSILON || depth > TRACEDEPTH) return;
        Ray ray = photon.ray;
        Intersection isc = LEngine -> firstIntersection(ray);

        if(isc.result == MISS) return;
        Primitive* prim = isc.primitive;
        Material* pm = prim -> GetMaterial();
        Color mcolor = pm -> GetColor(isc.parameter.x, isc.parameter.y);
        if(pm -> diff + pm -> spec > 0){
            Photon newPhoton = photon;
            newPhoton.ray.origin = isc.intersection - EPSILON * newPhoton.ray.direction;
            tree.updateHitPoint(newPhoton);
        }
        Photon nextPhoton = photon;
        nextPhoton.photonColor *= (mcolor.Max() > 0.01) ? mcolor / (mcolor.Max()) : Vector3();
        float psb = RAND();
        if(psb < pm -> diff){
            Photon photon1 = nextPhoton.Diffused(isc.intersection, isc.normal);
            photon1.photonColor *= (pm -> diff + abs(pm -> spec * pow(isc.normal.Dot((photon1.ray.direction - nextPhoton.ray.direction) * 0.5),20)))* abs(nextPhoton.ray.direction.Dot(isc.normal));
            photonTrace(photon1, depth + 1, rindex, inPrimitive);
        }
        else if(psb < pm -> diff + pm -> refl){
            photonTrace(nextPhoton.Reflected(isc.intersection, isc.normal), depth + 1, rindex, inPrimitive);
        }
        else if(psb < pm -> diff + pm -> refl +  pm -> refr){
            float opIndex = inPrimitive ? AIR_RINDEX : pm -> rIndex;
            photonTrace(nextPhoton.Refracted(isc.intersection, isc.normal * isc.result, rindex / opIndex), depth + 1, opIndex , !inPrimitive);
        }
    }
    void PPMRender::Render(){
        Camera* camera = LEngine -> LCamera;
        Scene* scene = LEngine -> LScene;
        Image* image = camera -> image;
        int w = image -> width;
        int h = image -> height;
        cout << "Raytracing" << endl;
        int cnt = 0;


        for(int j = 0; j < h; j ++){
            for(int i = 0; i < w; i ++){
                for(int p = 0; p < camera -> depth_samples; p ++){
                    rayTrace(camera -> rayCast(i, j), i, j ,0 ,Color(1,1,1), AIR_RINDEX, false);
                }
                cnt ++;
            }
            cout << cnt << endl;
        }

        cout << "hitpoint : " << tree.hitpointList.size() << endl;
        cout << "begin build" << endl;
        tree.Build(ORIGINR);

        int iters = 0;
        Color** data = new Color*[w];
        for(int i = 0; i < w; i ++)
        data[i] = new Color[h];
        int pcnt = 0;
        while(1){
            cout << "Emit Photon" << endl;
            for(auto light : scene -> lights){
                for(int i = 0; i < PHOTON_NUMBERS; i ++){
                    photonTrace(light -> EmitPhoton(), 0, AIR_RINDEX, false);
                    pcnt ++;
                    if(pcnt % 500 == 0)
                    cout << pcnt << endl;
                }
            }
            iters ++;
            for(int i = 0; i < w; i ++)
                for(int j = 0; j < h; j ++){
                    data[i][j] = Color();
                }
            for(vector<HitPoint>::const_iterator iter = tree.hitpointList.begin(); iter != tree.hitpointList.end(); iter ++){
                data[iter -> pixel_x][iter -> pixel_y] += iter -> wgt * iter -> flux / iter -> R / iter -> R;
            }

            for(int j = 0; j < h; j ++){
                for(int i = 0; i < w; i ++){
                    data[i][j] /= camera -> depth_samples * PI * iters * PHOTON_NUMBERS * scene -> lights.size() ;
                    image -> SetColor(i, j, Color(min(1.0, data[i][j].x),min(1.0, data[i][j].y), min(1.0, data[i][j].z)));
                }
            }
            LEngine -> GenerateImage("3.ppm");
        }
    }
}
