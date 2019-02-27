#ifndef IMAGE_H
#define IMAGE_H
#include "all.h"
#include "vector3.h"
namespace Raytracer{
    class Image{
    private:
        Color** image;
    public:
        int width;
        int height;

        Image();
        Image(int _w, int _h);
        void save(const char* filename);
        Color GetColor(int x,int y);
        void SetColor(int x,int y,Color color);
        void ReadImage(ifstream& fin);
        ~Image();
    };
}
#endif
