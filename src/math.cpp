#include "math.h"

float math_length(const XMFLOAT3& vector) {
    XMVECTOR vec = XMLoadFloat3(&vector);
    XMVECTOR lengthVec = XMVector3Length(vec);

    float length = 0.f;
    XMStoreFloat(&length, lengthVec);
    return length;
}

float math_distance(const XMFLOAT3& point1, const XMFLOAT3& point2) {
    XMVECTOR p1 = XMLoadFloat3(&point1);
    XMVECTOR p2 = XMLoadFloat3(&point2);
    XMVECTOR distanceVec = XMVectorSubtract(p1, p2);
    XMVECTOR length = XMVector3Length(distanceVec);

    float distance = 0.0f;
    XMStoreFloat(&distance,length);
    return distance;
}

float toRadians(float degrees) {
    return degrees * TO_RADIANS;
}

void math_normalize(XMFLOAT3& vec) {
    XMVECTOR v = XMLoadFloat3(&vec);
    v = XMVector3Normalize(v);

    XMStoreFloat3(&vec, v);
}