#ifndef _primitive_h_
#define _primitive_h_

#include "ray.h"
#include "util.h"
#include "Transformable.h"

class Primitive : public Transformable {
public:
    virtual bool intersect(const Ray& ray, float& tOut) const {
        return false;
    };
    virtual XMFLOAT3 getNormal(const XMFLOAT3& point) const {
        return XMFLOAT3();
    };
};

#endif