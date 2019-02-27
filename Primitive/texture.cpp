#include "texture.h"
namespace Raytracer{
    Texture::Texture():data(NULL){}
    void Texture::LoadTexture(const string& Filename){
        ifstream in;
        in.open(Filename,ios::in);
        in >> w >> h;
        data = new Color*[w];
        for(int i = 0; i < w; i ++){
            data[i] = new Color[h];
            for(int j = 0; j < h; j ++){
                in >> data[i][j];
                data[i][j] /= 255;
            }
        }
    }
    const Color& Texture::GetColor(float x, float y) const{
        int xx = (int(w * x));
        if(xx >= w) xx -= w;
        if(xx < 0) xx -= w;
        int yy = (int(h * y));
        if(yy >= h) yy -= h;
        if(yy < 0) yy -= h;
        return data[xx][yy];
    }
    void Texture::ReadTexture(ifstream& fin){
        string op, file;
        while(fin >> op){
            if(op == "end") break;
            if(op == "ttr") fin >> file;
        }
        LoadTexture(file);
    }
}
