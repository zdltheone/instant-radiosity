#include "scene.h"
#include <iostream>

Scene::Scene() {
    _camera = NULL;
}

void Scene::addPrimitive(const Primitive* primitive) {
    assert(primitive);
    _primitives.push_back(primitive);
}

void Scene::addLight(const Light* light) {
    assert(light);
    _lights.push_back(light);
    _lightsMap[light->getType()].push_back(light);
}

const std::vector<const Light*>& Scene::getLights() const {
    return _lights;
}

const std::vector<const Light*>& Scene::getLights(Light::LightType type) const {
    auto it = _lightsMap.find(type);
    return (it->second);
}

void Scene::setCamera(const Camera* camera) {
    assert(camera);
    _camera = camera;
}

const Camera* Scene::getCamera() const {
    return _camera;
}

void Scene::addVPL(const Primitive* vpl) {
    assert(vpl);
    _vpls.push_back(vpl);
}

void Scene::clearVPLs() {
    _vpls.clear();
}

const Primitive* Scene::intersectScene(const Ray& ray, XMFLOAT3& normalOut, float& tOut) const {
    const Primitive* intersectedPrimitive = NULL;
    float minDepth = INF;

    std::vector<const Primitive*> prims;
    prims.insert(prims.end(), _primitives.begin(), _primitives.end());
    prims.insert(prims.end(), _vpls.begin(), _vpls.end());

    //find primitives the ray intersects
    for(int i(0); i < prims.size(); ++i) {
        const Primitive* primitive = prims[i];

        //if we have an interection, do a depth test
        if(primitive->intersect(ray, normalOut, tOut)) {
            if(tOut <= EPSILON) {
                //std::cout << "intersection < EPSILON" << std::endl;
                continue;
            }
            
            XMFLOAT3 intersection;
            ray.getPointAlongRay(tOut, intersection);

            float distance = math_distance(ray.position, intersection);
            assert(distance > EPSILON);

            if(distance < minDepth) {
                minDepth = distance;
                intersectedPrimitive = primitive;
            }
        }
    }

    return intersectedPrimitive;
}