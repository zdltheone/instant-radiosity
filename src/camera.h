#ifndef _camera_h_
#define _camera_h_

#include "util.h"

class Camera {
public:
    Camera(float width = 800.f, float height = 600.f, float xFoV = 60.f, float yFoV = 60.f, float nearClip = 0.1f, float farClip = 100.f);
    
    void setPosition(XMFLOAT3 position);
    XMFLOAT3 getPosition() const;

    void setTarget(XMFLOAT3 target, XMFLOAT3 up);

    float getXFoV() const;
    float getYFoV() const;

    float getNearClip() const;
    float getFarClip() const;

    XMFLOAT4X4 getView() const;
    XMFLOAT4X4 getProjection() const;
    XMFLOAT3 getTarget() const;
private:
    void updateView();
    void updateProjection(float width, float height);
private:
    XMFLOAT3 _position;
    XMFLOAT3 _target;
    XMFLOAT3 _up;

    XMFLOAT4X4 _view;
    XMFLOAT4X4 _proj;

    float _aspectHbyW;
    float _xFoV;
    float _yFoV;
    float _nearClip;
    float _farClip;
};

#endif