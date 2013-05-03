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

    XMFLOAT3 cameraOrigin = XMFLOAT3(0.f, 0.f, 0.f);
    //XMStoreFloat3(&cameraOrigin, XMVector3Transform(XMLoadFloat3(&XMFLOAT3(0.f, 0.f, 0.f)), 
    //    XMLoadFloat4x4(&camera->getView())));

    float right  = camera->getNearClip() * tan(toRadians(camera->getXFoV() / 2.f)); 
    float left   = -1.f * right;
    float bottom = camera->getNearClip() * tan(toRadians(camera->getYFoV() / 2.f));
    float top    = -1.f * bottom;
    float dx = (right - left) / imageBuffer->getWidth();
    float dy = (top - bottom) / imageBuffer->getHeight();


    _instantRadiosity->SetReflectionNum( 5 );
    _instantRadiosity->SetSampleNum( 10 );
	_instantRadiosity->EmitVPLs( 0.5f, scene );


    for(unsigned int row(0); row < imageBuffer->getHeight(); ++row) {
        for(unsigned int col(0); col < imageBuffer->getWidth(); ++col) {
            XMFLOAT3 dir(left + col*(dx), 
                         bottom + row*(dy), 
                         cameraOrigin.z + camera->getNearClip());

            math_normalize(dir);

            XMFLOAT3 color = traceRay(Ray(cameraOrigin, dir), 0);
            
            imageBuffer->setPixel(row, col, XMFLOAT4(color.x, color.y, color.z, 1.f));
        }
    }
}

XMFLOAT3 RayTracer::traceRay(const Ray& ray, unsigned int depth) {
    float t = 0.f;
    XMFLOAT3 normal;

    const Primitive* primitive = _scene->intersectScene(ray, normal, t);
    if(primitive == NULL) {
        return XMFLOAT3(0.f, 0.f, 0.f);
    }
    XMFLOAT3 intersectPoint = ray.getPointAlongRay(t);
    XMFLOAT3 color = primitive->color;

    if(depth > _maxDepth) {
        return XMFLOAT3(); //fix this 
    }


    /*XMFLOAT3 L(1,-1,-1);
    math_normalize(L);
    XMFLOAT3 contrib;
 
    XMStoreFloat3(&contrib, XMVector3Dot(XMLoadFloat3(&normal), XMLoadFloat3(&L)) *  XMLoadFloat3(&color) + XMLoadFloat3(&XMFLOAT3(0.1f, 0.1f, 0.1f)) * XMLoadFloat3(&color));
    return contrib;*/

	XMFLOAT3 radiance = _instantRadiosity->GetRadiance( intersectPoint, normal, _scene );
    //XMStoreFloat3(&color, XMLoadFloat3(&radiance) * XMLoadFloat3(&color));
    return color;
    //check if object is diffuse or specular
    // if diffuse -> accumulate radiance from vpls and light sources
   

 }

void RayTracer::traceRayOnce( const Ray& ray, double& t )
{
    XMFLOAT3 normal;
	float tmpT = -INF;
	const Primitive* primitive = _scene->intersectScene( ray, normal, tmpT );
	if( primitive == NULL )
	{
		t = -INF;
	}

	t = tmpT;
}