#ifndef PROPERTYBROWSER_H
#define PROPERTYBROWSER_H

#include "abstractpropertymanager.h"

#include <QMap>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

class PropertyBrowser : public QWidget
{
    Q_OBJECT

public:
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
    void makeLine(QString propertyName, QVariant value, bool editable, QVector<QVariant> extra);
    void setLine(QString propertyName, QVariant value);

    AbstractPropertyManager *mPropertyManager;

    //A line will have two components, a Label and a widget
    QMap<QString, QHBoxLayout *> mLines;
    QMap<QString, QMetaType::Type> mLineType;
    QMap<QString, QWidget *> mLineWidget;
    QMap<QString, QLabel *> mLineLabel;

    QVBoxLayout *mMainLayout;
};

#endif // PROPERTYBROWSER_H
