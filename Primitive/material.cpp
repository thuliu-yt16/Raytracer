#include "material.h"
namespace Raytracer{
    Material::Material():
            refl(0.0),refr(0.0),diff(0.0),spec(0.0),rIndex(1.0),color(Color(1.0,1.0,1.0)),texture(NULL){}
    Material::Material(float _refl,float _refr,float _diff,float _spec,float _rindex,const Color& _color, Texture* t):
            refl(_refl),refr(_refr),diff(_diff),spec(_spec),rIndex(_rindex),color(_color),texture(t){}
    const Color& Material::GetColor(float x, float y) const{
        if(texture)
            return texture -> GetColor(x,y);
        else
            return color;
    }
    void Material::ReadMaterial(ifstream& fin){
        string op;
        while(fin >> op){
            if(op == "end") break;
            if(op == "refl")  fin >> refl;
            if(op == "refr") fin >> refr;
            if(op == "diff") fin >> diff;
            if(op == "spec") fin >> spec;
            if(op == "rindex") fin >> rIndex;
            if(op == "color") fin >> color;
            if(op == "Texture"){
                texture = new Texture;
                texture -> ReadTexture(fin);
            }
        }
    }
}
