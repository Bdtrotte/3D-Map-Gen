#ifndef TILEMATERIALSELECTIONBAR_H
#define TILEMATERIALSELECTIONBAR_H

#include "tilematerialset.h"

#include <QWidget>
#include <QLineEdit>

class TileMaterialSelectionBar : public QWidget
{
    Q_OBJECT

public:
    explicit TileMaterialSelectionBar(TileMaterial *tileMaterial, QWidget *parent = nullptr);

signals:
    void materialChanged(TileMaterial *material);

private slots:
    void selectMaterial();

    void materialsNameChanged(QString newName);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    void setMaterial(TileMaterial *material);

    QLineEdit *mLine;

    TileMaterial *mMaterial;
};

#endif // TILEMATERIALSELECTIONBAR_H
