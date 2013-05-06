#ifndef _raytracer_h_
#define _raytracer_h_

#include "scene.h"
#include "image.h"
#include "instantRadiosity.h"

class RayTracer {
public:
    RayTracer(unsigned int maxDepth);

    void raytrace( Scene* scene, Image* imageBuffer, int samples, int reflect, bool showVPLS = false);
    void showVPLs();
    void hideVPLs();
private:
    XMFLOAT3 traceRay(const Ray& ray, unsigned int depth);
private:
    const Scene* _scene;
    unsigned int _maxDepth;
    InstantRadiosity* _instantRadiosity;
    bool _showVPLs;
};

#endif