#include "tiletemplatesetsview.h"

#include "newtiletemplatesetdialog.h"
#include "tiletemplatepropertymanager.h"

#include <QDebug>
#include <QVBoxLayout>
#include <QToolBar>
#include <QToolButton>
#include <QSettings>

TileTemplateSetsView::TileTemplateSetsView(TileTemplateSetsManager *tileTemplateSetsManager,
                                           QWidget *parent)
    : QWidget(parent)
    , mTileTemplateSetsManager(tileTemplateSetsManager)
    , mTemplatePropertyBrowser(new PropertyBrowser(this))
    , mTabs(new QTabWidget(this))
    , mNewTemplate(new QAction("Add Template", this))
    , mRemoveTemplate(new QAction("Remove Template", this))
{
    //Must have a valid tileTemplateSetsManager.
    Q_ASSERT(tileTemplateSetsManager != nullptr);

    connect(mTileTemplateSetsManager, &TileTemplateSetsManager::tileTemplateSetAdded,
            this, &TileTemplateSetsView::tileTemplateSetAdded);
    connect(mTileTemplateSetsManager, &TileTemplateSetsManager::tileTemplateSetAboutToBeRemoved,
            this, &TileTemplateSetsView::tileTemplateSetAboutToBeRemoved);

    connect(mNewTemplate, &QAction::triggered,
            this, &TileTemplateSetsView::addTemplate);
    connect(mRemoveTemplate, &QAction::triggered,
            this, &TileTemplateSetsView::removeTemplate);

    mTabs->setTabPosition(QTabWidget::North);

    connect(mTabs, &QTabWidget::currentChanged,
            this, &TileTemplateSetsView::selectedTileTemplateChanged);

    connect(this, &TileTemplateSetsView::tileTemplateChanged, &TileTemplateSetsView::tileTemplateChangedSlot);

    QToolBar *actionBar = new QToolBar(this);
    actionBar->setFloatable(false);
    actionBar->setMovable(false);
    actionBar->addAction("Add", this, &TileTemplateSetsView::addTemplateSet);
    mRemoveTemplateSet = actionBar->addAction("Remove", this, &TileTemplateSetsView::removeTemplateSet);
    mSaveTemplateSet = actionBar->addAction("Save", this, &TileTemplateSetsView::saveTemplateSet);
    actionBar->addAction("Load", this, &TileTemplateSetsView::loadTemplateSet);
    mRemoveTemplateSet->setEnabled(false);
    mSaveTemplateSet->setEnabled(false);
    mRemoveTemplate->setEnabled(false);

    actionBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    //Setting tooltips for actions
    actionBar->actions().at(0)->setToolTip("Add Template Set");
    actionBar->actions().at(1)->setToolTip("Remove Template Set");
    actionBar->actions().at(2)->setToolTip("Save Template Set");
    actionBar->actions().at(3)->setToolTip("Load Template Set");

    //Sets icons for the action bar
    actionBar->actions().at(0)->setIcon(QIcon("://images/icons/22x22/add.png"));
    actionBar->actions().at(1)->setIcon(QIcon("://images/icons/22x22/remove.png"));
    actionBar->actions().at(2)->setIcon(QIcon("://images/icons/22x22/save.png"));
    actionBar->actions().at(3)->setIcon(QIcon("://images/icons/22x22/load.png"));



    //Set up default template set view
    mDefaultTemplateView = new QListView(this);
    mDefaultTemplateView->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum));
    mDefaultTemplateView->hide();

    mSplitter = new QSplitter(this);
    mSplitter->setOrientation(Qt::Vertical);
    mSplitter->addWidget(mDefaultTemplateView);
    mSplitter->addWidget(mTabs);
    mSplitter->addWidget(mTemplatePropertyBrowser);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(mSplitter);
    layout->addWidget(actionBar);
    setLayout(layout);
}

void TileTemplateSetsView::setDefaultTileTemplateSet(TileTemplateSet *tileTemplateSet)
{
    //No use in creating a view if the default set is empty, though there should always be at least the eraser.
    if (tileTemplateSet && tileTemplateSet->size() == 0)
        tileTemplateSet = nullptr;

    QItemSelectionModel *m = mDefaultTemplateView->selectionModel();

    mDefaultTemplateSet = tileTemplateSet;
    mDefaultTemplateView->setModel(mDefaultTemplateSet);
    mDefaultTemplateView->setVisible(tileTemplateSet);

    delete m;

    if ((m = mDefaultTemplateView->selectionModel())) {
        connect(m, &QItemSelectionModel::currentRowChanged,
                this, &TileTemplateSetsView::defaultTileTemplateSelected);
    }
}

void TileTemplateSetsView::saveState()
{
    QSettings settings;

    settings.setValue("templatesetview/splitter", mSplitter->saveState());
}

void TileTemplateSetsView::restoreState()
{
    QSettings settings;

    mSplitter->restoreState(settings.value("templatesetview/splitter").toByteArray());
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
    actionBar->addAction(mNewTemplate);
    actionBar->addAction(mRemoveTemplate);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(templateList);
    layout->addWidget(actionBar);

    templateWidget->setLayout(layout);

    QString name = tileTemplateSet->name();
    if (!tileTemplateSet->isSaved())
        name += "*";
    mTabs->setCurrentIndex(mTabs->addTab(templateWidget, name));

    mRemoveTemplateSet->setEnabled(true);
    mSaveTemplateSet->setEnabled(true);
}

void TileTemplateSetsView::tileTemplateSetAboutToBeRemoved(SavableTileTemplateSet *tileTemplateSet)
{
    int tab = mTileTemplateSetsManager->tileTemplateSets().indexOf(tileTemplateSet);

    mTabs->setCurrentIndex(tab);

    QWidget *w = mTabs->currentWidget();
    mTabs->removeTab(tab);
    delete w;

    mListViews.removeAt(tab);

    if (mTabs->count() == 0) {
        mRemoveTemplateSet->setEnabled(false);
        mSaveTemplateSet->setEnabled(false);
    }
}

void TileTemplateSetsView::selectedTileTemplateChanged()
{
    if (QItemSelectionModel *m = mDefaultTemplateView->selectionModel()) {
        m->blockSignals(true);
        m->clear();
        m->blockSignals(false);
    }

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

    if (templateId != -1) {
        emit tileTemplateChanged(mTileTemplateSetsManager->tileTemplateSetAt(curTab)->tileTemplateAt(templateId));
        mRemoveTemplate->setEnabled(true);
    } else {
        emit tileTemplateChanged(nullptr);
        mRemoveTemplate->setEnabled(false);
    }
}

void TileTemplateSetsView::defaultTileTemplateSelected(const QModelIndex &current)
{
    for (QListView *v : mListViews) {
        v->selectionModel()->blockSignals(true);
        v->selectionModel()->clear();
        v->selectionModel()->blockSignals(false);
    }

    emit tileTemplateChanged(mDefaultTemplateSet->tileTemplateAt(current.row()));

    mRemoveTemplate->setEnabled(false);
}

void TileTemplateSetsView::addTemplate()
{
    int curTab = mTabs->currentIndex();
    Q_ASSERT(curTab >= 0);

    TileTemplate *newTemplate = new TileTemplate(QColor::fromHsv(qrand()%255, 255, 255),
                                                 "New Tile Template",
                                                 0,
                                                 1,
                                                 nullptr,                // Default material.
                                                 QVector2D(0.5, 0.5));

    mTileTemplateSetsManager->addTileTemplate(curTab, newTemplate);
    mListViews[curTab]->selectionModel()->setCurrentIndex(mTileTemplateSetsManager->tileTemplateSetAt(curTab)->index(mTileTemplateSetsManager->tileTemplateSetAt(curTab)->size() - 1, 0),
                                                          QItemSelectionModel::ClearAndSelect);

    mRemoveTemplate->setEnabled(true);
}

void TileTemplateSetsView::removeTemplate()
{
    int curTab = mTabs->currentIndex();
    Q_ASSERT(curTab >= 0);

    QItemSelectionModel *selectionModel = mListViews[curTab]->selectionModel();
    const QModelIndex &curIndex = selectionModel->currentIndex();
    Q_ASSERT(curIndex.isValid());

    int row = curIndex.row();

    mTileTemplateSetsManager->removeTileTemplate(curTab, row);

    selectedTileTemplateChanged();

    if (mTileTemplateSetsManager->tileTemplateSetAt(curTab)->size() == 0)
        mRemoveTemplate->setEnabled(false);
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

void TileTemplateSetsView::tileTemplateChangedSlot(TileTemplate *tileTemplate)
{
    if (tileTemplate)
        mTemplatePropertyBrowser->setPropertyManager(new TileTemplatePropertyManager(tileTemplate));
    else
        mTemplatePropertyBrowser->clear();
}

void TileTemplateSetsView::tileTemplateSetSaveStatusChanged(SavableTileTemplateSet *tileTemplateSet, bool status)
{
    int tab = mTileTemplateSetsManager->tileTemplateSets().indexOf(tileTemplateSet);

    QString tabText = mTileTemplateSetsManager->tileTemplateSetAt(tab)->name();
    if (!status)
        tabText += "*";
    mTabs->setTabText(tab, tabText);
}
