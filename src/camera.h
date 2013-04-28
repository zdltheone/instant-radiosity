#ifndef _camera_h_
#define _camera_h_

#include "util.h"
#include "Transformable.h"

class Camera : public Transformable {
public:
    Camera(float xFoV = 60.f, float yFoV = 60.f, float nearClip = 0.1f, float farClip = 100.f);
   

    float getXFoV() const;
    float getYFoV() const;

    float getNearClip() const;
    float getFarClip() const;
private:
    float _xFoV;
    float _yFoV;
    float _nearClip;
    float _farClip;
};

#endif