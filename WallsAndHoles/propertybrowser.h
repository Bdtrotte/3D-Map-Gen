#ifndef PROPERTYBROWSER_H
#define PROPERTYBROWSER_H

#include "abstractpropertymanager.h"

#include <QMap>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>

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
    void makeLine(QString propertyName, QVariant value);
    void setLine(QString propertyName, QVariant value);
    QVariant getLineValue(QString propertyName);
    void propertyEdited(QString propertyName, QVariant newValue);

    QMap<QString, QHBoxLayout *> mLines;
    QMap<QString, QMetaType::Type> mLineType;

    QVBoxLayout *mMainLayout;
};

#endif // PROPERTYBROWSER_H
