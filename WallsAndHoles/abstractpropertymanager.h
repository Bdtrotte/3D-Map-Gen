#ifndef ABSTRACTPROPERTYMANAGER_H
#define ABSTRACTPROPERTYMANAGER_H

#include <QObject>
#include <QVariant>
#include <QVector>

/**
 * @brief The AbstractPropertyManager class
 * Should take some source data in the constructor, and
 * implement the ability to see and modify any properties of the core object.
 */
class AbstractPropertyManager : public QObject
{
    Q_OBJECT

public:
    AbstractPropertyManager() : QObject(nullptr) {}

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
     * Returns a vector of all the properties.
     * It must be ordered as follows:
     * Must have at least three elements:
     * 0 - A QString which is the name of the property
     * 1 - The initial value of the property. Values can be of the types
     * handled in propertybrowser.h
     * 2 - A bool which says whether or not the value can be edited.
     * There can be as many more elements as needed by the given type,
     * for instance, a value of type float produces a spin box.
     * Elements 3 and 4 will be considered the min and max of the box, respectivly, if they are present.
     * Explore propertybrowser.h for more details on how different types are handled.
     * @return
     */
    virtual QVector<QVector<QVariant>> properties() = 0;

signals:
    /**
     * @brief propertyChanged
     * Should be emited whenever the source data is changed from a place other than the property browser.
     * @param propertyId
     */
    propertyChanged(QString propertyName, QVariant value);
};

#endif // ABSTRACTPROPERTYMANAGER_H
