#ifndef _raytracer_h_
#define _raytracer_h_

#include "scene.h"
#include "image.h"

class RayTracer {
public:
    RayTracer(unsigned int maxDepth);

    void raytrace(const Scene* scene, Image* imageBuffer);

	// This function is only use for tracing rays from light sources to virtual point light
	void traceRayOnce( const Ray&ray, double& t );
private:
    XMFLOAT3 traceRay(const Ray& ray, unsigned int depth);
private:
    const Scene* _scene;
    unsigned int _maxDepth;
   // InstantRadiosity* _instantRadiosity;
};

#endif