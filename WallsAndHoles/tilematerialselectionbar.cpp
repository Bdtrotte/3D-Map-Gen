#include "tilematerialselectionbar.h"

#include "tilematerialselectiondialog.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>

TileMaterialSelectionBar::TileMaterialSelectionBar(TileMaterial *tileMaterial, QWidget *parent)
    : QWidget(parent)
    , mLine(new QLineEdit(this))
    , mMaterial(tileMaterial)
{
    setAcceptDrops(true);

    mLine->setReadOnly(true);

    if (mMaterial) {
        mLine->setText(mMaterial->name());

        connect(mMaterial, &TileMaterial::nameChanged,
                this, &TileMaterialSelectionBar::materialsNameChanged);
    } else {
        mLine->setText("(none)");
    }

    QPushButton *selectMaterial = new QPushButton("...", this);

    QHBoxLayout *layout = new QHBoxLayout();

    layout->addWidget(mLine);
    layout->addWidget(selectMaterial);

    setLayout(layout);

    connect(selectMaterial, &QPushButton::clicked,
            this, &TileMaterialSelectionBar::selectMaterial);
}

void TileMaterialSelectionBar::selectMaterial()
{
    TileMaterialSelectionDialog tmsd;

    if (tmsd.exec())
        setMaterial(TileMaterialSet::getInstance()->materialAt(tmsd.result));
}

void TileMaterialSelectionBar::materialsNameChanged(QString newName)
{
    mLine->setText(newName);
}

void TileMaterialSelectionBar::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasText()
            && event->mimeData()->text().split(": ")[0] == "TileMaterial") {
        event->acceptProposedAction();
    }
}

void TileMaterialSelectionBar::dropEvent(QDropEvent *event)
{
    QStringList stringList = event->mimeData()->text().split(": ");
    int ind = stringList[1].toInt();

    TileMaterial *material = TileMaterialSet::getInstance()->materialAt(ind);
    setMaterial(material);
}

void TileMaterialSelectionBar::setMaterial(TileMaterial *material)
{
    disconnect(mMaterial);

    mMaterial = material;

    connect(mMaterial, &TileMaterial::nameChanged,
            this, &TileMaterialSelectionBar::materialsNameChanged);

    mLine->setText(material->name());

    emit materialChanged(material);
}
