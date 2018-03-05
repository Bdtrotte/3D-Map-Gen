#ifndef MESHVIEWCONTAINER_H
#define MESHVIEWCONTAINER_H

#include <QWidget>
#include <QToolBar>

#include "abstractrenderer.h"

class MeshView;
class AbstractMeshViewCamera;

class MeshViewContainer : public QWidget
{
    Q_OBJECT

public:
    explicit MeshViewContainer(QWidget *parent = 0);

    void saveMesh(QString path);
    void loadMesh(QString path);

    /**
     * @brief Calls setRenderer(renderer) on the mesh view.
     */
    void setRenderer(QSharedPointer<AbstractRenderer> renderer);

    QAction *addCamera(AbstractMeshViewCamera *camera,
                   QString name,
                   QIcon icon = QIcon(),
                   QKeySequence ks = QKeySequence());

private:
    MeshView *mMeshView;

    QToolBar *mToolBar;
};

#endif // MESHVIEWCONTAINER_H
