#ifndef CAMERA_H
#define CAMERA_H
#include "all.h"
#include "vector3.h"
#include "image.h"
#include "ray.h"
/*
       y
       ^
       |
       |
       ------- > x
      /
    /
  L
 z
*/
namespace Raytracer{
    class Camera{
    public:
        Vector3 up;
        Vector3 position;
        Vector3 direction;
        Image* image;
        float f;
        float image_w;
        float image_h;
        float Aperture_R;
        int depth_samples;

        Camera(){
            cout << "Camera created" << endl;
        }
        void ReadCamera(ifstream& fin){
            string op;
            while(fin >> op){
                if(op == "end") break;
                if(op == "up") fin >> up;
                if(op == "position") fin >> position;
                if(op == "direction") fin >> direction;
                if(op == "f") fin >> f;
                if(op == "width") fin >> image_w;
                if(op == "height") fin >> image_h;
                if(op == "aperture") fin >> Aperture_R;
                if(op == "image"){
                    image = new Image;
                    image -> ReadImage(fin);
                }
                if(op == "samples") fin >> depth_samples;
            }
        }
        void Init(){
            up = Vector3(0,1,0);
            position = Vector3(0,5,50);
            direction = Vector3(0,0,-1);
            image = new Image(800,600);
            f = 51;
            image_w = 34;
            image_h = 25.5;
            Aperture_R = 0;
        }
        Ray rayCast(float x, float y){
            int w = image -> width;
            int h = image -> height;
            Vector3 u = direction.Cross(up).Normal();
            Vector3 v = -up.Normal();
            Vector3 beginpixel = position + direction * f - 0.5 * image_w * u - 0.5 * image_h * v;
            Vector3 delta_u = image_w / w * u;
            Vector3 delta_v = image_h / h * v;
            float xx,yy;
            do{
                xx = 2 * RAND() - 1;
                yy = 2 * RAND() - 1;
            }while(xx * xx + yy * yy > 1);
            return Ray(position + Aperture_R * (xx * u + yy * v), (direction * f - 0.5 * image_w * u - 0.5 * image_h * v + x * delta_u + y * delta_v - Aperture_R * (xx * u + yy * v)).Normal(), INF);
        }
        ~Camera(){
            delete image;
        }
    };
}

#endif
