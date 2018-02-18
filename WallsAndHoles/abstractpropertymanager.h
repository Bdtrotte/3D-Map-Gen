#ifndef ABSTRACTPROPERTYMANAGER_H
#define ABSTRACTPROPERTYMANAGER_H

#include <QObject>
#include <QVariant>

/**
 * @brief The AbstractPropertyManager class
 * Should take some source data in the constructor, and
 * implement the ability to see and modify any properties of the core object.
 */
class AbstractPropertyManager : public QObject
{
    Q_OBJECT

public:
    AbstractPropertyManager(QObject *parent = nullptr);

    /**
     * @brief propertyEdited
     * Called from PropertyBrowser when a property is edited.
     * Should be handled, and modify the source data as expected.
     * @param propertyName
     * @param value
     */
    virtual void propertyEdited(QString propertyName, QVariant value) = 0;

    /**
     * @brief properties
     * returns a vector of all the properties of this object. Each int passed along should be unique.
     * Each pair should be (propertyName, propertyValue).
     *
     * One can pass an AbstractPropertyManager as the value and it will work as expected!
     *
     * See propertybrowser.h for a list of valid types for the value to be.
     * @return
     */
    virtual QVector<QPair<QString, QVariant>> properties() = 0;

signals:
    /**
     * @brief propertyChanged
     * Should be emited whenever the source data is changed from a place other than the property browser.
     * @param propertyId
     */
    propertyChanged(QString propertyName, QVariant value);
};

#endif // ABSTRACTPROPERTYMANAGER_H
