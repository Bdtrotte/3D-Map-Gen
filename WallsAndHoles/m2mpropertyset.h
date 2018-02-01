#ifndef M2MPROPERTYSET_H
#define M2MPROPERTYSET_H

#include <QMap>
#include "m2mpropertyinstance.h"


/**
 * @brief The M2MPropertySet class represents a set of M2MPropertyInstances.
 */
class M2MPropertySet {
public:
    M2MPropertySet();

    bool containsProperty(M2MPropertyClass *prop) const;

    /**
     * @brief Adds the property to the set, ASSUMING IT IS NOT ALREADY THERE.
     * @param propInst The property to be added.
     */
    void addProperty(M2MPropertyInstance propInst);

    /**
     * @brief Returns the property of the given type.
     * @param ofType The type of the property.
     */
    M2MPropertyInstance getProperty(M2MPropertyClass *ofType) const;


    bool operator==(const M2MPropertySet &other) const;
    bool operator!=(const M2MPropertySet &other) const;

protected:
    QMap<M2MPropertyClass *, M2MPropertyInstance> mProperties;
};

#endif // M2MPROPERTYSET_H
