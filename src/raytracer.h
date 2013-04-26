#ifndef _raytracer_h_
#define _raytracer_h_

#include "scene.h"
#include "image.h"

class RayTracer {
public:
    RayTracer(unsigned int maxDepth);

    void raytrace(const Scene* scene, Image* imageBuffer);
private:
    XMFLOAT3 traceRay(const Ray& ray);
private:
    const Scene* _scene;
    unsigned int _maxDepth;
};

#endif