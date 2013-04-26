#ifndef _primitive_h_
#define _primitive_h_

#include "ray.h"

class Primitive {
public:
    virtual bool intersect(const Ray& ray, float& tOut) const {
        return false;
    };
    virtual XMFLOAT3 getNormal(const XMFLOAT3& point) const {
        return XMFLOAT3();
    };
};

#endif