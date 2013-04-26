#include "raytracer.h"


RayTracer::RayTracer(unsigned int maxDepth) {
    _scene = NULL;
    _maxDepth = maxDepth;
}

void RayTracer::raytrace(const Scene* scene, Image* imageBuffer) {
    assert(scene);
    assert(imageBuffer);
    _scene = scene;
    
    const Camera* camera = scene->getCamera();
    assert(camera);

    XMFLOAT3 origin = camera->getPosition();
    float right  = camera->getNearClip() * tan(toRadians(camera->getXFoV() / 2.f)); 
    float left   = -1.f * right;
    float bottom = camera->getNearClip() * tan(toRadians(camera->getYFoV() / 2.f));
    float top    = -1.f * bottom;
    float dx = (right - left) / imageBuffer->getWidth();
    float dy = (top - bottom) / imageBuffer->getHeight();
    float xOffset = dx / 2.f;
    float yOffset = dy / 2.f;

    for(unsigned int i(0); i < imageBuffer->getHeight(); ++i) {
        for(unsigned int j(0); j < imageBuffer->getWidth(); ++j) {
            XMFLOAT3 dir(left + i*(dx + xOffset), 
                         bottom + j*(dy + yOffset), 
                         origin.z + camera->getNearClip());

            math_normalize(dir);

            XMFLOAT3 color = traceRay(Ray(origin, dir));
        
            imageBuffer->setPixel(i, j, XMFLOAT4(color.x, color.y, color.z, 1.f));
        }
    }
}

XMFLOAT3 RayTracer::traceRay(const Ray& ray) {
    float t = 0.f;

    const Primitive* primitive = _scene->intersectScene(ray, t);
    if(primitive == NULL) {
        return XMFLOAT3(0.f, 0.f, 0.f);
    }

    XMFLOAT3 intersectPoint = ray.getPointAlongRay(t);
    XMFLOAT3 normal = primitive->getNormal(intersectPoint);
}