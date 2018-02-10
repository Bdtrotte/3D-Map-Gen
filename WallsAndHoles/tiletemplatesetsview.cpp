#include "tiletemplatesetsview.h"

#include "newtiletemplatesetdialog.h"
#include "tiletemplateeditor.h"
#include "xmltool.h"

#include <QAction>
#include <QDebug>
#include <QVBoxLayout>
#include <QToolBar>
#include <QFileDialog>
#include <QMessageBox>

TileTemplateSetsView::TileTemplateSetsView(QWidget *parent)
    : QWidget(parent)
    , mTabs(new QTabWidget(this))
{
    mTabs->setTabPosition(QTabWidget::North);

    connect(mTabs, &QTabWidget::currentChanged,
            this, &TileTemplateSetsView::selectedTileTemplateChanged);

    TileTemplateEditor *templateEditor = new TileTemplateEditor(this);
    connect(this, &TileTemplateSetsView::tileTemplateChanged,
            templateEditor, &TileTemplateEditor::tileTemplateChanged);

    QToolBar *actionBar = new QToolBar(this);
    actionBar->setFloatable(false);
    actionBar->setMovable(false);
    actionBar->addAction("Add Template Set", this, &TileTemplateSetsView::addTemplateSet);
    actionBar->addAction("Remove Template Set", this, &TileTemplateSetsView::removeTemplateSet);
    actionBar->addAction("Save Template Set", this, &TileTemplateSetsView::saveTemplateSet);
    actionBar->addAction("Load Template Set", this, &TileTemplateSetsView::loadTemplateSet);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(mTabs);
    layout->addWidget(templateEditor);
    layout->addWidget(actionBar);
    setLayout(layout);
}

void TileTemplateSetsView::addTileTemplateSet(SavableTileTemplateSet *tileTemplateSet)
{
    mTileTemplateSets.append(tileTemplateSet);

    connect(tileTemplateSet, &SavableTileTemplateSet::saveStateChanged,
            this, [this, tileTemplateSet](bool status){
        tileTemplateSetSaveStatusChanged(tileTemplateSet, status);
    });

    QWidget *templateWidget = new QWidget(this);

    QListView *templateList = new QListView(templateWidget);
    templateList->setModel(tileTemplateSet);
    connect(templateList->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &TileTemplateSetsView::selectedTileTemplateChanged);
    mListViews.append(templateList);

    QToolBar *actionBar = new QToolBar(templateWidget);
    actionBar->setFloatable(false);
    actionBar->setMovable(false);
    actionBar->addAction("Add Template", this, &TileTemplateSetsView::addTemplate);
    actionBar->addAction("Remove Template", this, &TileTemplateSetsView::removeTemplate);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(templateList);
    layout->addWidget(actionBar);

    templateWidget->setLayout(layout);

    QString name = tileTemplateSet->name();
    if (!tileTemplateSet->isSaved())
        name += "*";
    mTabs->setCurrentIndex(mTabs->addTab(templateWidget, name));
}

void TileTemplateSetsView::removeCurrentTileTemplateSet()
{
    int curI = mTabs->currentIndex();
    if (curI == -1) return;

    QWidget *w = mTabs->currentWidget();
    mTabs->removeTab(curI);
    delete w;

    mTileTemplateSets.removeAt(curI);
    mListViews.removeAt(curI);
}

void TileTemplateSetsView::selectedTileTemplateChanged()
{
    int curTab = mTabs->currentIndex();
    int templateId;

    if (curTab == -1) {
        templateId = -1;
    } else {
        const QModelIndex &index = mListViews[curTab]->selectionModel()->currentIndex();
        if (index.isValid()) {
            templateId = index.row();
            if (templateId >= mTileTemplateSets[curTab]->size())
                templateId = mTileTemplateSets[curTab]->size() - 1;
        } else {
            templateId = -1;
        }
    }

    if (templateId != -1)
        emit tileTemplateChanged(mTileTemplateSets[curTab]->tileTemplateAt(templateId));
    else
        emit tileTemplateChanged(nullptr);
}

void TileTemplateSetsView::addTemplate()
{
    int curTab = mTabs->currentIndex();
    Q_ASSERT(curTab >= 0);

    TileTemplate *newTemplate = new TileTemplate(QColor::fromHsv(qrand()%255, 255, 255),
                                                 "New Tile Template",
                                                 0,
                                                 1,
                                                 QVector2D(0.5, 0.5),
                                                 mTileTemplateSets[curTab]);

    mTileTemplateSets[curTab]->addTileTemplate(newTemplate);
    mListViews[curTab]->selectionModel()->setCurrentIndex(mTileTemplateSets[curTab]->index(mTileTemplateSets[curTab]->size() - 1, 0),
                                                          QItemSelectionModel::ClearAndSelect);
}

void TileTemplateSetsView::removeTemplate()
{
    int curTab = mTabs->currentIndex();
    Q_ASSERT(curTab >= 0);

    QItemSelectionModel *selectionModel = mListViews[curTab]->selectionModel();
    const QModelIndex &curIndex = selectionModel->currentIndex();
    Q_ASSERT(curIndex.isValid());

    int row = curIndex.row();

    mTileTemplateSets[curTab]->removeTileTemplate(row);

    selectedTileTemplateChanged();
}

void TileTemplateSetsView::addTemplateSet()
{
    NewTileTemplateSetDialog dia;
    if (dia.exec()) {
        for (SavableTileTemplateSet *ts : mTileTemplateSets) {
            if (dia.result.fileLocation == ts->savePath()) {
                QMessageBox mb;
                mb.setText(tr("Tile Template Set already open at requested location."));
                mb.exec();
                return;
            }
        }

        SavableTileTemplateSet *newTTS = new SavableTileTemplateSet(dia.result.fileLocation, dia.result.name);
        addTileTemplateSet(newTTS);
    }
}

void TileTemplateSetsView::removeTemplateSet()
{
    int curTab = mTabs->currentIndex();
    Q_ASSERT(curTab >= 0);

    // TODO add ability to abort remove

    removeCurrentTileTemplateSet();
}

void TileTemplateSetsView::saveTemplateSet()
{
    int curTab = mTabs->currentIndex();
    Q_ASSERT(curTab >= 0);

    mTileTemplateSets[curTab]->save();
}

void TileTemplateSetsView::loadTemplateSet()
{
    QString path = QFileDialog::getOpenFileName(this,
                                                tr("Load Tile Template Set"),
                                                "/home/",
                                                tr("XML files (*.xml)"));

    for (SavableTileTemplateSet *ts : mTileTemplateSets) {
        if (path == ts->savePath()) {
            QMessageBox mb;
            mb.setText(tr("The requested file is already open."));
            mb.exec();
            return;
        }
    }

    if (!path.isNull()) {
        SavableTileTemplateSet *newSet = XMLTool::openTileTemplateSet(path);
        if (newSet != nullptr) {
            addTileTemplateSet(newSet);
        }
    }
}

void TileTemplateSetsView::tileTemplateSetSaveStatusChanged(SavableTileTemplateSet *tileTemplateSet, bool status)
{
    int tab = mTileTemplateSets.indexOf(tileTemplateSet);

    QString tabText = mTileTemplateSets[tab]->name();
    if (!status)
        tabText += "*";
    mTabs->setTabText(tab, tabText);
}
