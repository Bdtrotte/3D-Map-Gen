#include "simpletexturedscene.h"

#include "simpletexturedrenderer.h"

SimpleTexturedScene::SimpleTexturedScene()
{
    connect(this, &SimpleTexturedScene::objectAdded, this, [this] () { emit sceneUpdated(); });
    connect(this, &SimpleTexturedScene::objectRemoved, this, [this] () { emit sceneUpdated(); });
    connect(this, &SimpleTexturedScene::sceneCleared, this, [this] () { emit sceneUpdated(); });
}


SharedSimpleTexturedScene SimpleTexturedScene::makeScene()
{
    return SharedSimpleTexturedScene(new SimpleTexturedScene());
}


SimpleTexturedScene::~SimpleTexturedScene()
{

}

void SimpleTexturedScene::clear()
{
    mObjects.clear();
    emit sceneCleared();
}

void SimpleTexturedScene::addObject(QSharedPointer<SimpleTexturedObject> object)
{
    mObjects.push_back(object);
    emit objectAdded(*object);
}

void SimpleTexturedScene::removeObject(QSharedPointer<SimpleTexturedObject> object)
{
    mObjects.removeAll(object);
    emit objectRemoved(*object);
}

void SimpleTexturedScene::commitChanges()
{
    emit changesCommitted();
}


QSharedPointer<AbstractRenderer> SimpleTexturedScene::makeRenderer()
{
    QSharedPointer<SimpleTexturedRenderer> renderer = QSharedPointer<SimpleTexturedRenderer>::create(shared_from_this());

    connect(this, &SimpleTexturedScene::objectAdded, renderer.data(), &SimpleTexturedRenderer::objectAdded);
    connect(this, &SimpleTexturedScene::objectRemoved, renderer.data(), &SimpleTexturedRenderer::objectRemoved);
    connect(this, &SimpleTexturedScene::sceneCleared, renderer.data(), &SimpleTexturedRenderer::clearAll);
    connect(this, &SimpleTexturedScene::changesCommitted, renderer.data(), &SimpleTexturedRenderer::requestUpdate);

    return renderer;
}
