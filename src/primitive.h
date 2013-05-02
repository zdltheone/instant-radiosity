#ifndef _primitive_h_
#define _primitive_h_

#include "ray.h"
#include "util.h"
#include "Transformable.h"

class Primitive : public Transformable {
public:
    virtual bool intersect(const Ray& ray, XMFLOAT3& normalOut, float& tOut) const {
        return false;
    };
   
    XMFLOAT3 color;
};

#endif