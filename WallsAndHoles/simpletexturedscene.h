#ifndef SIMPLETEXTUREDSCENE_H
#define SIMPLETEXTUREDSCENE_H

// Using std::shared_ptr because I need std::enable_shared_from_this
#include <memory>

#include <QVector>
#include <QSharedPointer>

#include "dereferencingiterator.h"
#include "dereferencingconstiterator.h"

#include "abstractscene.h"

#include "simpletexturedobject.h"


class SimpleTexturedScene : public AbstractScene, public std::enable_shared_from_this<SimpleTexturedScene>
{
    Q_OBJECT

public:

    using SharedSimpleTexturedScene = std::shared_ptr<SimpleTexturedScene>;


    /**
     * @brief Creates an empty scene.
     * @return A shared pointer to an empty scene.
     */
    static SharedSimpleTexturedScene makeScene();


    virtual ~SimpleTexturedScene();

    /**
     * @brief Removes all objects from the scene.
     */
    void clear();

    /**
     * @brief Adds an object to the scene.
     * @param object    The object to be added.
     */
    void addObject(QSharedPointer<SimpleTexturedObject> object);

    /**
     * @brief Removes an object from the scene.
     * @param object    The object to be removed.
     */
    void removeObject(QSharedPointer<SimpleTexturedObject> object);


    /**
     * @brief Commits the changes to the scene, possibly causing it
     * to be rerendered.
     */
    void commitChanges();

    /* Iterators for accessing objects in the scene */
    auto begin()
    {
        auto itr = mObjects.begin();
        return DereferencingIterator<SimpleTexturedObject, decltype(itr)>(itr);
    }

    auto end()
    {
        auto itr = mObjects.end();
        return DereferencingIterator<SimpleTexturedObject, decltype(itr)>(itr);
    }


    auto begin() const
    {
        auto itr = mObjects.begin();
        return DereferencingConstIterator<SimpleTexturedObject, decltype(itr)>(itr);
    }

    auto end() const
    {
        auto itr = mObjects.end();
        return DereferencingConstIterator<SimpleTexturedObject, decltype(itr)>(itr);
    }

signals:
    /**
     * @brief Emitted when an object is added.
     * @param obj   The newly added object (already in the scene).
     */
    void objectAdded(const SimpleTexturedObject &obj);

    /**
     * @brief Emitted when an object is about to be removed.
     *
     * NOTE: This is NOT emitted if sceneCleared() is emitted.
     * @param obj   The object that was removed (not in the scene).
     */
    void objectRemoved(const SimpleTexturedObject &obj);

    /**
     * @brief Emitted when all objects are removed from the scene.
     */
    void sceneCleared();

    /**
     * @brief Emitted when changes to the scene are committed and should be rendered.
     */
    void changesCommitted();

protected:

    /**
     * @brief Creates an empty scene.
     */
    SimpleTexturedScene();

    /**
     * @brief Creates and returns a SimpleTexturedRenderer.
     * @return A SimpleTexturedRenderer.
     */
    QSharedPointer<AbstractRenderer> makeRenderer() override;


    QVector<QSharedPointer<SimpleTexturedObject>> mObjects;
};


typedef SimpleTexturedScene::SharedSimpleTexturedScene SharedSimpleTexturedScene;

#endif // SIMPLETEXTUREDSCENE_H
