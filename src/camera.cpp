#include "camera.h"


Camera::Camera(float xFoV, float yFoV, float nearClip, float farClip) {
    _xFoV = xFoV;
    _yFoV = yFoV;
    _farClip = farClip;
    _nearClip = nearClip;
}

float Camera::getXFoV() const {
    return _xFoV;
}

float Camera::getYFoV() const {
    return _yFoV;
}

float Camera::getNearClip() const {
    return _nearClip;
}

float Camera::getFarClip() const {
    return _farClip;
}