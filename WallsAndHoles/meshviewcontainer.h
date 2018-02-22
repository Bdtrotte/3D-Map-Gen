#ifndef MESHVIEWCONTAINER_H
#define MESHVIEWCONTAINER_H

#include <QWidget>

#include "meshview.h"

namespace Ui {
class MeshViewContainer;
}

class MeshViewContainer : public QWidget
{
    Q_OBJECT

public:
    explicit MeshViewContainer(QWidget *parent = 0);
    ~MeshViewContainer();

    /**
     * @brief Calls setRenderer(renderer) on the mesh view.
     */
    void setRenderer(QSharedPointer<AbstractRenderer> renderer);

private slots:
    void on_toolSelection_currentIndexChanged(int index);


protected:

    MeshView *mMeshView;

private:
    Ui::MeshViewContainer *ui;
};

#endif // MESHVIEWCONTAINER_H
