#ifndef MATERIAL_H
#define MATERIAL_H
#include "all.h"
#include "vector3.h"
#include "texture.h"
namespace Raytracer{
    class Material{
    public:
        Color color;
        float refl,refr,diff,spec,rIndex;
        Texture* texture;
        Material();
        Material(float _refl,float _refr,float _diff,float _spec,float _rindex,const Color& _color, Texture* t = NULL);
        const Color& GetColor(float x, float y) const;
        void ReadMaterial(ifstream& fin);
    };

}
#endif
