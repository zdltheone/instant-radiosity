#ifndef __transformable_h__
#define __transformable_h__

#include "util.h"

class Transformable {
public:
    void setPosition(XMFLOAT3 position);
    XMFLOAT3 getPosition() const;
    void setRotation(XMFLOAT3 axis, float degrees);
    void setScale(XMFLOAT3 scalingFactors);
    void lookAt(XMFLOAT3 position, XMFLOAT3 target, XMFLOAT3 up);

    XMFLOAT4X4 getTransformation() const;
private:
    XMFLOAT3 _position;

    XMFLOAT4X4 _translation;
    XMFLOAT4X4 _rotation;
    XMFLOAT4X4 _scale;
    XMFLOAT4X4 _lookAt;

    XMFLOAT4X4 _transform;
};

#endif