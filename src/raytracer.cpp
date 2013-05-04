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

    //default camera location to origin
    XMFLOAT3 cameraOrigin = XMFLOAT3(0.f, 0.f, 0.f);
    //XMStoreFloat3(&cameraOrigin, XMVector3Transform(XMLoadFloat3(&XMFLOAT3(0.f, 0.f, 0.f)), 
    //XMLoadFloat4x4(&camera->getView())));

    //calculate viewport stuff
    float right  = camera->getNearClip() * tan(toRadians(camera->getXFoV() / 2.f)); 
    float left   = -1.f * right;
    float bottom = camera->getNearClip() * tan(toRadians(camera->getYFoV() / 2.f));
    float top    = -1.f * bottom;
    float dx = (right - left) / imageBuffer->getWidth();
    float dy = (top - bottom) / imageBuffer->getHeight();

    //"photon mapping" phase to create virtual point lights
    _instantRadiosity->SetReflectionNum( 5 );
    _instantRadiosity->SetSampleNum( 1 );
	//_instantRadiosity->EmitVPLs( 0.5f, scene );

    //shoot ray for each pixel in imagebuffer
    for(unsigned int row(0); row < imageBuffer->getHeight(); ++row) {
        for(unsigned int col(0); col < imageBuffer->getWidth(); ++col) {
            //get ray direction
            XMFLOAT3 dir(left + col*(dx), 
                         bottom + row*(dy), 
                         cameraOrigin.z + camera->getNearClip());

            math_normalize(dir);

            //trace ray into scene to get pixel color
            XMFLOAT3 color = traceRay(Ray(cameraOrigin, dir), 0);
   
            //set pixel in imagebuffer
            imageBuffer->setPixel(row, col, XMFLOAT4(color.x, color.y, color.z, 1.f));
        }
    }
}

XMFLOAT3 RayTracer::traceRay(const Ray& ray, unsigned int depth) {

    float t = 0.f;
    XMFLOAT3 normal;
    XMFLOAT3 intersectPoint;
    XMFLOAT3 color;
    XMFLOAT3 radiance;

    //find intersection in scene
    const Primitive* primitive = _scene->intersectScene(ray, normal, t);
    if(primitive == NULL) {
        //return black if no intersection
        return XMFLOAT3(0.f, 0.f, 0.f);
    }

    //get interscetion information
    //note: push intersection point away from primitive slightly to avoid problems
    intersectPoint = ray.getPointAlongRay(t-0.001);
    color = primitive->color;

    //check if we shouldnt go any deeper
    if(depth > _maxDepth) {
        return XMFLOAT3(0.f, 0.f, 0.f);  //placeholder
    }

    //send out shadow ray (temporary)   
    vector<const Light*> lightVec = ( _scene->getLights( Light::AreaLight ) );
	AreaLight* areaLight = (AreaLight*)lightVec[ 0 ];
    XMFLOAT3 shadowDir;
    XMStoreFloat3(&shadowDir, XMVector3Normalize(XMLoadFloat3(&areaLight->getPosition()) - XMLoadFloat3(&intersectPoint)));
    Ray shadowRay(intersectPoint, shadowDir);
    //find intersection in scene
    XMFLOAT3 tempNorm;
    float t2;
    const Primitive* primitive2 = _scene->intersectScene(shadowRay, tempNorm, t2);
    if(primitive2) {
        //return black if no intersection
        return XMFLOAT3(0.f, 0.f, 0.f);
    }
    //----

    //note: we are only handling diffuse surfaces

    //temp fake lighting
    XMFLOAT3 L = areaLight->getPosition();
    math_normalize(L);
    XMFLOAT3 contrib;
    XMStoreFloat3(&contrib, XMVector3Dot(XMLoadFloat3(&L), XMLoadFloat3(&normal)) *  XMLoadFloat3(&color) + XMLoadFloat3(&XMFLOAT3(0.1f, 0.1f, 0.1f)) * XMLoadFloat3(&color));
    return contrib;


    //get radiance from vpls and calculate final color
    //XMFLOAT3 radiance = _instantRadiosity->GetRadiance( intersectPoint, normal, _scene );
    //XMStoreFloat3(&color, XMLoadFloat3(&radiance) * XMLoadFloat3(&color));
    //return color;
 }
