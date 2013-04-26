#include "scene.h"

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
}

void Scene::setCamera(const Camera* camera) {
    assert(camera);
    _camera = camera;
}

const Camera* Scene::getCamera() const {
    return _camera;
}

const Primitive* Scene::intersectScene(const Ray& ray, float& tOut) const {
    const Primitive* intersectedPrimitive = NULL;
    float minDepth = INF;

    //find primitives the ray intersects
    for(int i(0); i < _primitives.size(); ++i) {
        const Primitive* primitive = _primitives[i];

        //if we have an interection, do a depth test
        if(primitive->intersect(ray, tOut)) {
            assert(tOut > EPSILON);
            XMFLOAT3 intersection;
            ray.getPointAlongRay(tOut, intersection);

            float distance = math_distance(ray.position, intersection);
            assert(distance > EPSILON);

            if(distance < minDepth) {
                intersectedPrimitive = primitive;
            }
        }
    }

    return intersectedPrimitive;
}