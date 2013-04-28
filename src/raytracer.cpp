#include "raytracer.h"


RayTracer::RayTracer(unsigned int maxDepth) {
    _scene = NULL;
    _maxDepth = maxDepth;
    _instantRadiosity = new InstantRadiosity();
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


    for(unsigned int row(0); row < imageBuffer->getHeight(); ++row) {
        for(unsigned int col(0); col < imageBuffer->getWidth(); ++col) {
            XMFLOAT3 dir(left + row*(dx + xOffset), 
                         bottom + col*(dy + yOffset), 
                         origin.z + camera->getNearClip());

            math_normalize(dir);

            XMFLOAT3 color = traceRay(Ray(origin, dir), 0);
        
            imageBuffer->setPixel(row, col, XMFLOAT4(color.x, color.y, color.z, 1.f));
        }
    }
}

XMFLOAT3 RayTracer::traceRay(const Ray& ray, unsigned int depth) {
    float t = 0.f;

    const Primitive* primitive = _scene->intersectScene(ray, t);
    if(primitive == NULL) {
        return XMFLOAT3(0.f, 0.f, 0.f);
    }

    XMFLOAT3 intersectPoint = ray.getPointAlongRay(t);
    XMFLOAT3 normal = primitive->getNormal(intersectPoint);

    if(depth > _maxDepth) {
        return XMFLOAT3(); //fix this 
    }

    //check if object is diffuse or specular
    // if diffuse -> accumulate radiance from vpls and light sources
   

 }

void RayTracer::traceRayOnce( const Ray& ray, double& t )
{
	float tmpT = -INF;
	const Primitive* primitive = _scene->intersectScene( ray, tmpT );
	if( primitive == NULL )
	{
		t = -INF;
	}

	t = tmpT;
}