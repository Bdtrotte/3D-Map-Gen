#include "scene.h"
#include "drawableaxes.h"

Scene::Scene() {
    drawables.append(QSharedPointer<DrawableAxes>::create());
}

void Scene::addObject(RenderableObjectP object) {
    objects.append(object);
}

void Scene::addObject(DrawableObjectP object) {
    drawables.append(object);
}


const QVector<Scene::RenderableObjectP>& Scene::getAllObjects() const {
    return objects;
}

const QVector<Scene::DrawableObjectP>& Scene::getAllDrawables() const {
    return drawables;
}


size_t Scene::getNumVertices() const {
    size_t total = 0;

    for (auto obj : objects)
        total += obj->getNumVertices();

    return total;
}


void Scene::initializeGL() {
    for (DrawableObjectP drawable : drawables)
        drawable->initializeGL();
}
