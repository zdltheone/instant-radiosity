#ifndef _scene_h_
#define _scene_h_

#include <vector>
#include "camera.h"
#include "primitive.h"
#include "light.h"
#include "math.h"

class Scene {
public:
    Scene();

    void addPrimitive(const Primitive* primitive);
    void addLight(const Light* light);

    void setCamera(const Camera* camera);
    const Camera* getCamera() const;
        
    const Primitive* intersectScene(const Ray& ray, float& tOut) const;
private:
    std::vector<const Primitive*> _primitives;
    std::vector<const Light*> _lights;
    const Camera* _camera;
};

#endif