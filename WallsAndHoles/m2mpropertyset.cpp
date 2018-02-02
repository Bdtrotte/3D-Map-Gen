#include "m2mpropertyset.h"

M2MPropertySet::M2MPropertySet()
{

}


bool M2MPropertySet::containsProperty(M2MPropertyClass *prop) const {
    return mProperties.contains(prop);
}



M2MPropertyInstance M2MPropertySet::getProperty(M2MPropertyClass *ofType) const {
    // Using the find() syntax is necessary here because (at the time of writing)
    // M2MPropertyInstance does not have a public default constructor, and
    // QMap's operator[] and value() both construct a default object if
    // the key is not present.

    auto itr = mProperties.find(ofType);

    // This assert is triggered if containsProperty(ofType) is false.
    Q_ASSERT( itr != mProperties.end() );

    return itr.value();
}

void M2MPropertySet::addProperty(M2MPropertyInstance propInst) {
    // mProperties should not already contain this kind of property
    Q_ASSERT( !mProperties.contains(propInst.getClass()) );

    mProperties.insert(propInst.getClass(), propInst);
}



bool M2MPropertySet::operator==(const M2MPropertySet &other) const {
    return mProperties == other.mProperties;
}

bool M2MPropertySet::operator!=(const M2MPropertySet &other) const {
    return mProperties != other.mProperties;
}
