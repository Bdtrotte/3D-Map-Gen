#include "scene.h"

Scene::Scene() {

}

void Scene::addObject(RenderableObjectP object) {
    objects.push_back(object);
}


const std::vector<Scene::RenderableObjectP>& Scene::getAllObjects() const {
    return objects;
}

size_t Scene::getNumVertices() const {
    size_t total = 0;

    for (auto obj : objects)
        total += obj->getNumVertices();

    return total;
}
