#include "Transformable.h"

Transformable::Transformable() {
    _position = XMFLOAT3(0.f, 0.f, 0.f);
    XMStoreFloat4x4(&_transform, XMMatrixIdentity());
}

void Transformable::setPosition(XMFLOAT3 position) {
    _position = position;
    XMMATRIX matrix = XMMatrixTranslationFromVector(XMLoadFloat3(&_position));
    XMStoreFloat4x4(&_translation, matrix);
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

XMFLOAT4X4 Transformable::getTransformation() const {
    XMFLOAT4X4 transform;
    XMMATRIX translation = XMLoadFloat4x4(&_translation);
    XMMATRIX rotation = XMLoadFloat4x4(&_rotation);
    XMMATRIX scale = XMLoadFloat4x4(&_scale);

    XMStoreFloat4x4(&transform, (scale * rotation * translation));

    return transform;
}