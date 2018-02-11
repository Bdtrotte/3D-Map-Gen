#include "tiletemplatesetsview.h"

#include "newtiletemplatesetdialog.h"
#include "tiletemplateeditor.h"

#include <QAction>
#include <QDebug>
#include <QVBoxLayout>
#include <QToolBar>

TileTemplateSetsView::TileTemplateSetsView(TileTemplateSetsManager *tileTemplateSetsManager,
                                           QWidget *parent)
    : QWidget(parent)
    , mTileTemplateSetsManager(tileTemplateSetsManager)
    , mTabs(new QTabWidget(this))
{
    //Must have a valid tileTemplateSetsManager.
    Q_ASSERT(tileTemplateSetsManager != nullptr);

    connect(mTileTemplateSetsManager, &TileTemplateSetsManager::tileTemplateSetAdded,
            this, &TileTemplateSetsView::tileTemplateSetAdded);
    connect(mTileTemplateSetsManager, &TileTemplateSetsManager::tileTemplateSetAboutToBeRemoved,
            this, &TileTemplateSetsView::tileTemplateSetAboutToBeRemoved);

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

void TileTemplateSetsView::tileTemplateSetAdded(SavableTileTemplateSet *tileTemplateSet)
{
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

void TileTemplateSetsView::tileTemplateSetAboutToBeRemoved(SavableTileTemplateSet *tileTemplateSet)
{
    int tab = mTileTemplateSetsManager->tileTemplateSets().indexOf(tileTemplateSet);

    mTabs->setCurrentIndex(tab);

    QWidget *w = mTabs->currentWidget();
    mTabs->removeTab(tab);
    delete w;

    mListViews.removeAt(tab);
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
        } else {
            templateId = -1;
        }
    }

    if (templateId != -1)
        emit tileTemplateChanged(mTileTemplateSetsManager->tileTemplateSetAt(curTab)->tileTemplateAt(templateId));
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
                                                 QVector2D(0.5, 0.5));

    mTileTemplateSetsManager->tileTemplateSetAt(curTab)->addTileTemplate(newTemplate);
    mListViews[curTab]->selectionModel()->setCurrentIndex(mTileTemplateSetsManager->tileTemplateSetAt(curTab)->index(mTileTemplateSetsManager->tileTemplateSetAt(curTab)->size() - 1, 0),
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

    mTileTemplateSetsManager->tileTemplateSetAt(curTab)->removeTileTemplate(row);

    selectedTileTemplateChanged();
}

void TileTemplateSetsView::addTemplateSet()
{
    mTileTemplateSetsManager->newTileTemplateSet();
}

void TileTemplateSetsView::removeTemplateSet()
{
    int curTab = mTabs->currentIndex();
    Q_ASSERT(curTab >= 0);

    mTileTemplateSetsManager->removeTileTemplateSet(curTab);
}

void TileTemplateSetsView::saveTemplateSet()
{
    int curTab = mTabs->currentIndex();
    Q_ASSERT(curTab >= 0);

    mTileTemplateSetsManager->tileTemplateSetAt(curTab)->save();
}

void TileTemplateSetsView::loadTemplateSet()
{
    mTileTemplateSetsManager->loadTileTemplateSet();
}

void TileTemplateSetsView::tileTemplateSetSaveStatusChanged(SavableTileTemplateSet *tileTemplateSet, bool status)
{
    int tab = mTileTemplateSetsManager->tileTemplateSets().indexOf(tileTemplateSet);

    QString tabText = mTileTemplateSetsManager->tileTemplateSetAt(tab)->name();
    if (!status)
        tabText += "*";
    mTabs->setTabText(tab, tabText);
}
