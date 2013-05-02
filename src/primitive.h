#ifndef _primitive_h_
#define _primitive_h_

#include "ray.h"
#include "util.h"
#include "Transformable.h"

class Primitive : public Transformable {
public:
    enum PrimitiveType {
        Square,
        Sphere,
        Cube
    };
public:
    Primitive(PrimitiveType type) : _type(type) {};

    virtual bool intersect(const Ray& ray, XMFLOAT3& normalOut, float& tOut) const {
        return false;
    };
   
    PrimitiveType getType() const { return _type; }

    XMFLOAT3 color;

private:
    PrimitiveType _type;
};

#endif