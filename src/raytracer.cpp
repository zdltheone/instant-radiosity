#include "raytracer.h"

static int countNum = 0;

RayTracer::RayTracer(unsigned int maxDepth) {
    _scene = NULL;
    _maxDepth = maxDepth;
    _instantRadiosity = new InstantRadiosity();
    _showVPLs = false;
}

void RayTracer::raytrace( Scene* scene, Image* imageBuffer, int samples, int reflect, bool showVPLs) {
    assert(scene);
    assert(imageBuffer);
    _showVPLs = showVPLs;
    _scene = scene;
    
    const Camera* camera = scene->getCamera();
    assert(camera);

    XMFLOAT3 cameraOrigin = camera->getPosition();
    XMFLOAT3 cameraTarget = XMFLOAT3(0.f, 0.f, 0.f);
    //XMStoreFloat3(&cameraOrigin, XMVector3Transform(XMLoadFloat3(&XMFLOAT3(0.f, 0.f, 0.f)), 
    //    XMLoadFloat4x4(&camera->getView())));

    float right  = camera->getNearClip() * tan(toRadians(camera->getXFoV() / 2.f)); 
    float left   = -1.f * right;
    float bottom = camera->getNearClip() * tan(toRadians(camera->getYFoV() / 2.f));
    float top    = -1.f * bottom;
    float dx = (right - left) / imageBuffer->getWidth();
    float dy = (top - bottom) / imageBuffer->getHeight();
  
    if(_showVPLs) {
        _instantRadiosity->showVPLs();
    } else {
        _instantRadiosity->hideVPLs();
    }
    _instantRadiosity->SetReflectionNum( reflect );
    _instantRadiosity->SetSampleNum( samples );
	_instantRadiosity->EmitVPLs( 0.5f, scene );

    std::cout << "begin\n";
    for(unsigned int row(0); row < imageBuffer->getHeight(); ++row) {
         if(row == imageBuffer->getHeight() / 4) {
                std::cout << "25%\n";
        }
        if(row == imageBuffer->getHeight() / 2) {
            std::cout << "50%\n";
        }
        if(row == 3*imageBuffer->getHeight() / 4) {
            std::cout << "75%\n";
        }
        for(unsigned int col(0); col < imageBuffer->getWidth(); ++col) {
          

            XMFLOAT3 dir(left + col*(dx), 
                         bottom + row*(dy), 
                         cameraTarget.z + camera->getNearClip());

            math_normalize(dir);
        

            XMFLOAT3 color = traceRay(Ray(cameraOrigin, dir), 0);
            
            imageBuffer->setPixel(row, col, XMFLOAT4(color.x, color.y, color.z, 1.f));
        }
    }
      std::cout << "done\n" << endl;
}

XMFLOAT3 RayTracer::traceRay(const Ray& ray, unsigned int depth) {
    float t = 0.f;
    static XMFLOAT3 saveNormal;
    XMFLOAT3 normal;
    XMFLOAT3 intersectPoint;

    const Primitive* primitive = _scene->intersectScene(ray, normal, t);
    if(primitive == NULL) {
        return XMFLOAT3(0.f, 0.f, 0.f);
    }
    Ray r(XMFLOAT3(0.f, 0.f, 0.f), ray.direction);
    
    ray.getPointAlongRay(t, intersectPoint);
    XMFLOAT3 color = primitive->color;
   
    if(depth > _maxDepth) {
        return XMFLOAT3(); //fix this 
    }

    if(_showVPLs == false) {
	    XMFLOAT3 radiance = _instantRadiosity->GetRadiance( intersectPoint, normal, _scene );
        XMStoreFloat3(&color, XMLoadFloat3(&XMFLOAT3(1, 1, 1)) * XMLoadFloat3(&radiance) * XMLoadFloat3(&color));
    } else {
		XMFLOAT3 fakeLight(-0.25, 0.25, -1);
		XMStoreFloat3(&color, XMVector3Dot(XMVector3Normalize(XMLoadFloat3(&fakeLight)), XMLoadFloat3(&normal)) * XMLoadFloat3(&color) + XMLoadFloat3(&XMFLOAT3(0.5, 0.5, 0.5)) * XMLoadFloat3(&color));
	}

    return color;
  
 }

void RayTracer::showVPLs() {
    _showVPLs = true;
}

void RayTracer::hideVPLs() {
    _showVPLs = false;
}
