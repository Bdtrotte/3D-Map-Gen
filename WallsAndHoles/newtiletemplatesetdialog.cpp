#include "newtiletemplatesetdialog.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

NewTileTemplateSetDialog::NewTileTemplateSetDialog(QWidget *parent)
    : QDialog(parent)
    , mOk(new QPushButton("Ok", this))
    , mCancel(new QPushButton("Cancel", this))
    , mFindFile(new QPushButton("...", this))
    , mName(new QLineEdit("New Tile Template Set", this))
    , mFilePath(new QLineEdit(this))
{
    setWindowTitle("New Tile Template Set");

    QVBoxLayout *vLayout = new QVBoxLayout;
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(new QLabel("Name: ", this));
    hLayout->addWidget(mName);
    vLayout->addLayout(hLayout);
    hLayout = new QHBoxLayout;
    hLayout->addWidget(new QLabel("File Dir: ", this));
    hLayout->addWidget(mFilePath);
    hLayout->addWidget(mFindFile);
    vLayout->addLayout(hLayout);
    hLayout = new QHBoxLayout;
    hLayout->addWidget(mOk);
    hLayout->addWidget(mCancel);
    vLayout->addLayout(hLayout);

    setLayout(vLayout);

    connect(mOk, &QPushButton::clicked,
            this, &NewTileTemplateSetDialog::ok);
    connect(mCancel, &QPushButton::clicked,
            this, &NewTileTemplateSetDialog::cancel);
    connect(mFindFile, &QPushButton::clicked,
            this, &NewTileTemplateSetDialog::findFile);
}

void NewTileTemplateSetDialog::ok()
{
    if (mFilePath->text().isEmpty()) {
        QMessageBox mb;
        mb.setText("Need to set a save location!");
        mb.exec();
        return;
    }

    result = NewTileTemplateSetData(mName->text(), mFilePath->text() + "/" + mName->text() + ".xml");
    done(1);
}

void NewTileTemplateSetDialog::cancel()
{
    done(0);
}

void NewTileTemplateSetDialog::findFile()
{
    mFilePath->setText(QFileDialog::getExistingDirectory(this,
                                                         "Save Location",
                                                         "/home/"));
}
