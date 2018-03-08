#ifndef ABSTRACTSCENE_H
#define ABSTRACTSCENE_H

#include <memory>

#include <QObject>
#include <QWeakPointer>

#include "abstractrenderer.h"
#include "objtools.h"

class AbstractScene : public QObject
{
    Q_OBJECT

public:


    virtual ~AbstractScene() {}

    /**
     * @brief Returns an AbstractRenderer for this scene.
     *
     * This is the correct way to retreive the AbstractRenderer associated to the
     * scene. Subtypes of AbstractScene should not keep a pointer to their renderer.
     *
     * @return An instance of the appropriate subtype of AbstractRenderer.
     */
    QSharedPointer<AbstractRenderer> getRenderer()
    {
        QSharedPointer<AbstractRenderer> strongRef;

        if (mRenderer.isNull()) {
            strongRef = makeRenderer();


            // The scene should not own its renderer.
            mRenderer = strongRef.toWeakRef();
        } else
            // The returned value should be a strong reference, so that the caller
            // may take ownership of the returned object.
            strongRef = mRenderer.toStrongRef();

        return strongRef;
    }

    virtual SharedOBJModel exportOBJ()=0;


protected:

    /**
     * @brief Creates an AbstractRenderer for this scene.
     *
     * The scene should connect to the renderer's requestUpdate() signal appropriately.
     *
     * @return An instance of the appropriate subtype of AbstractRenderer.
     */
    virtual QSharedPointer<AbstractRenderer> makeRenderer() = 0;

signals:

    /**
     * @brief Emitted when the scene is changed.
     */
    void sceneUpdated();


private:


    /**
     * @brief A weak pointer to the renderer associated to this scene.
     */
    QWeakPointer<AbstractRenderer> mRenderer;

};

typedef std::shared_ptr<AbstractScene> SharedAbstractScene;


#endif // ABSTRACTSCENE_H
