#ifndef TEXTURE_H
#define TEXTURE_H
#include "all.h"
#include "vector3.h"
namespace Raytracer{
    class Texture{
    public:
        Color** data;
        int w,h;
        Texture();
        void LoadTexture(const string& Filename);
        const Color& GetColor(float x, float y) const;
        void ReadTexture(ifstream& fin);
    };
}
#endif
