#include "camera.h"


Camera::Camera(float xFoV, float yFoV, float nearClip, float farClip) {
    _xFoV = xFoV;
    _yFoV = yFoV;
    _farClip = farClip;
    _nearClip = nearClip;
    _position = vec3(0.f, 0.f, 0.f);
}

void Camera::setPosition(vec3 position) {
    _position = position;
}

vec3 Camera::getPosition() const {
    return _position;
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