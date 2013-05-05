#include "camera.h"


Camera::Camera(float width, float height, float xFoV, float yFoV, float nearClip, float farClip) {
    _xFoV = xFoV;
    _yFoV = yFoV;
    _farClip = farClip;
    _nearClip = nearClip;


    _position = XMFLOAT3(0.f, 0.f, 0.f);
    _target   = XMFLOAT3(0.f, 0.f, 1.f);
    _up       = XMFLOAT3(0.f, 1.f, 0.f);

    updateView();
    updateProjection(width, height);
}

void Camera::setPosition(XMFLOAT3 position) {
    _position = position;
    updateView();
}

XMFLOAT3 Camera::getPosition() const {
    return _position;
}

void Camera::setTarget(XMFLOAT3 target, XMFLOAT3 up) {
    _target = target;
    _up = up;
    updateView();
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

void Camera::updateView() {
    XMVECTOR pos = XMLoadFloat3(&_position);
    XMVECTOR tar = XMLoadFloat3(&_target);
    XMVECTOR up  = XMLoadFloat3(&_up);

    XMMATRIX mat = XMMatrixLookAtRH(pos, tar, up);
    XMStoreFloat4x4(&_view, mat);
}

void Camera::updateProjection(float width, float height) {
    _aspectHbyW = height / width;
    XMMATRIX matrix = XMMatrixPerspectiveFovRH(_yFoV, _aspectHbyW, _nearClip, _farClip); 
    XMStoreFloat4x4(&_proj, matrix);
}

XMFLOAT4X4 Camera::getView() const {
    return _view;
}

XMFLOAT4X4 Camera::getProjection() const {
    return _proj;
}

XMFLOAT3 Camera::getTarget() const {
    return _target;
}