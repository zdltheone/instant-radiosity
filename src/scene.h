#ifndef _scene_h_
#define _scene_h_

#include <vector>
#include <map>
#include "camera.h"
#include "primitive.h"
#include "light.h"
#include "math.h"

class Scene {
public:
    Scene();

    void addPrimitive(const Primitive* primitive);
    void addLight(const Light* light);

    const std::vector<const Light*>& getLights() const;
    const std::vector<const Light*>& getLights(Light::LightType type) const;

    void setCamera(const Camera* camera);
    const Camera* getCamera() const;
        
    const Primitive* intersectScene(const Ray& ray, XMFLOAT3& normalOut, float& tOut) const;
private:
    std::vector<const Primitive*> _primitives;
    std::vector<const Light*> _lights;
    std::map<Light::LightType, std::vector<const Light*> > _lightsMap;
    const Camera* _camera;
};

#endif