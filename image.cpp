#include "image.h"

namespace Raytracer{
    Image::Image():width(0),height(0),image(NULL){}
    Image::Image(int _w,int _h):width(_w),height(_h){
        image = new Color* [width];
        for(int i = 0; i < width; i ++){
            image[i] = new Color[height];
        }
    }

    void Image::ReadImage(ifstream& fin){
        string op;
        while(fin >> op){
            if(op == "end") break;
            if(op == "width") fin >> width;
            if(op == "height") fin >> height;
        }
        if(image){
            for(int i = 0; i < width; i ++) delete[] image[i];
            delete[] image;
        }
        image = new Color* [width];
        for(int i = 0; i < width; i ++){
            image[i] = new Color[height];
        }
    }

    void Image::save(const char* filename){
        ofstream out(filename);
        out << "P3" << endl;
        out << width << ' ' << height << endl;
        out << 255 << endl;
        for(int j = 0; j < height;j++){
            for(int i = 0; i < width;i++){
                out << int(image[i][j].r * 255) << ' '  << int(image[i][j].g * 255) << ' ' << int(image[i][j].b * 255) << endl;
            }
        }
    }
    void Image::SetColor(int x,int y,Color color){
        if( 0 <= x && x < width && 0 <= y && y < height)
        image[x][y] = color;
    }
    Color Image::GetColor(int x, int y){
        return image[x][y];
    }

    Image::~Image(){
        for(int i = 0;i < width; i++){
            delete[] image[i];
        }
        delete[] image;
    }
}
