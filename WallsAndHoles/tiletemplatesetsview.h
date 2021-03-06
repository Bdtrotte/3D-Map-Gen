#ifndef TILETEMPLATESETSVIEW_H
#define TILETEMPLATESETSVIEW_H

#include "savabletiletemplateset.h"
#include "tiletemplatesetsmanager.h"
#include "propertybrowser.h"

#include <QAction>
#include <QWidget>
#include <QTabWidget>
#include <QListView>
#include <QSplitter>

/**
 * @brief The TileTemplateSetsView class
 * Stores and displays all open TileTemplateSets.
 * Has a tool bar for saving/loading/creating sets,
 * and modifying existing sets
 */
class TileTemplateSetsView : public QWidget
{
    Q_OBJECT

public:
    explicit TileTemplateSetsView(TileTemplateSetsManager *tileTemplateSetsManage,
                                  QWidget *parent = nullptr);

    void setDefaultTileTemplateSet(TileTemplateSet *tileTemplateSet);

    void saveState();
    void restoreState();

signals:
    void tileTemplateChanged(TileTemplate *tileTemplate);

private slots:
    void tileTemplateSetAdded(SavableTileTemplateSet *tileTemplateSet);
    void tileTemplateSetAboutToBeRemoved(SavableTileTemplateSet *tileTemplateSet);

    void selectedTileTemplateChanged();
    void defaultTileTemplateSelected(const QModelIndex &current);

    void addTemplate();
    void removeTemplate();

    void addTemplateSet();
    void removeTemplateSet();
    void saveTemplateSet();
    void loadTemplateSet();

    void tileTemplateChangedSlot(TileTemplate *tileTemplate);

private:
    void tileTemplateSetSaveStatusChanged(SavableTileTemplateSet *tileTemplateSet, bool status);

    TileTemplateSet *mDefaultTemplateSet;
    QListView *mDefaultTemplateView;
    TileTemplateSetsManager *mTileTemplateSetsManager;
    QList<QListView *> mListViews;
    PropertyBrowser *mTemplatePropertyBrowser;

    QTabWidget *mTabs;

    QAction *mNewTemplate;
    QAction *mRemoveTemplate;

    QAction *mRemoveTemplateSet;
    QAction *mSaveTemplateSet;

    QSplitter *mSplitter;
};

#endif // TILETEMPLATESETSVIEW_H
