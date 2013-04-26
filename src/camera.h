#ifndef _camera_h_
#define _camera_h_

#include "util.h"

class Camera {
public:
    Camera(float xFoV = 60.f, float yFoV = 60.f, float nearClip = 0.1f, float farClip = 100.f);
    
    void setPosition(vec3 position);
    vec3 getPosition() const;

    float getXFoV() const;
    float getYFoV() const;

    float getNearClip() const;
    float getFarClip() const;
private:
    vec3 _position;
    float _xFoV;
    float _yFoV;
    float _nearClip;
    float _farClip;
};

#endif