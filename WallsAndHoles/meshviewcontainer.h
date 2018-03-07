#ifndef MESHVIEWCONTAINER_H
#define MESHVIEWCONTAINER_H

#include <QWidget>

#include "meshview.h"

class MeshViewContainer : public QWidget
{
    Q_OBJECT

public:
    explicit MeshViewContainer(QWidget *parent = 0);

    /**
     * @brief Calls setRenderer(renderer) on the mesh view.
     */
    void setRenderer(QSharedPointer<AbstractRenderer> renderer);


protected:

    MeshView *mMeshView;
};

#endif // MESHVIEWCONTAINER_H
