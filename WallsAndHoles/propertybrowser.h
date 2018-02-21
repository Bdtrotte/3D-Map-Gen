#ifndef PROPERTYBROWSER_H
#define PROPERTYBROWSER_H

#include "abstractpropertymanager.h"

#include <QMap>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

// TODO : Provide good full documentation about how to create a propertyManager

/**
 * @brief The PropertyBrowser class
 */
class PropertyBrowser : public QScrollArea
{
    Q_OBJECT

public:
    enum CustomType {
        LineWidget,      //A one line widget
        BlockWidget,     //A multi line widget which will apear bellow the label
        PropertyManager  //Another property manager, which will create a nested propertyBrowser (most fun)
    };

    explicit PropertyBrowser(QWidget *parent = nullptr);

    /**
     * @brief setPropertyManager
     * Takes ownership of the property manager. The old property manager will be deleted.
     *
     * Set this propertyBrowser to represent the given propertyManager.
     * @param propertyManager
     */
    void setPropertyManager(AbstractPropertyManager *propertyManager);

    /**
     * @brief clear
     * Deletes the current propertyManager if there is one.
     * The PropertyBrowser will show nothing.
     */
    void clear();

private:
    QWidget *mMainWidget;

    void makeLine(QString propertyName, QVariant value, bool editable, QVector<QVariant> extra);
    void setLine(QString propertyName, QVariant value);

    AbstractPropertyManager *mPropertyManager;

    //A line will have two components, a Label and a widget
    QMap<QString, QBoxLayout *> mLines;
    QMap<QString, QMetaType::Type> mLineType;
    QMap<QString, QWidget *> mLineWidget;
    QMap<QString, QLabel *> mLineLabel;

    QVBoxLayout *mMainLayout;
};

#endif // PROPERTYBROWSER_H
