#include "Transformable.h"

void Transformable::setPosition(XMFLOAT3 position) {
    XMMATRIX matrix = XMMatrixTranslationFromVector(XMLoadFloat3(&position));
    XMStoreFloat4x4(&_translation, matrix);
    _position = position;
}

XMFLOAT3 Transformable::getPosition() const {
    return _position;
}

void Transformable::setRotation(XMFLOAT3 axis, float degrees) {
    XMMATRIX matrix = XMMatrixRotationAxis(XMLoadFloat3(&axis), degrees);
    XMStoreFloat4x4(&_rotation, matrix);
}

void Transformable::setScale(XMFLOAT3 scalingFactors) {
    XMMATRIX matrix = XMMatrixScalingFromVector(XMLoadFloat3(&scalingFactors));
    XMStoreFloat4x4(&_scale, matrix);
}

void Transformable::lookAt(XMFLOAT3 position, XMFLOAT3 target, XMFLOAT3 up) {
    XMMATRIX matrix = XMMatrixLookAtRH( XMLoadFloat3(&position),
                                        XMLoadFloat3(&target),
                                        XMLoadFloat3(&up));
    XMStoreFloat4x4(&_transform, matrix); 
}

XMFLOAT4X4 Transformable::getTransformation() const {
    XMMATRIX matrix;
    XMFLOAT4X4 transform;
    XMMATRIX currentTransform = XMLoadFloat4x4(&_transform);
    XMMATRIX translation = XMLoadFloat4x4(&_translation);
    XMMATRIX rotation = XMLoadFloat4x4(&_rotation);
    XMMATRIX scale = XMLoadFloat4x4(&_scale);

    //will this work?
    XMStoreFloat4x4(&transform, currentTransform * translation * rotation * scale);

    return transform;
}