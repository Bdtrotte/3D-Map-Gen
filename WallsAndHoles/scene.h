#ifndef SCENE_H
#define SCENE_H

#include "renderableobject.h"


class Scene {
    using RenderableObjectP = std::shared_ptr<RenderableObject>;

public:
    Scene();

    // Adds an object to the scene.
    void addObject(RenderableObjectP object);

    // Computes the number of vertices in the scene by adding up the number of vertices
    // in each object. This does not consider whether vertices are reused.
    size_t getNumVertices() const;

    const std::vector<RenderableObjectP>& getAllObjects() const;

protected:

    // shared_ptr is very useful here
    std::vector<RenderableObjectP> objects;
};

#endif // SCENE_H
