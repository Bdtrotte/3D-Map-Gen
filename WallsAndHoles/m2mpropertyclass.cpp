#include "m2mpropertyclass.h"

M2MPropertyClass::M2MPropertyClass(QString name, M2MPropertyClass *parent)
    : mParent(parent),
      mName(name)
{

}


M2MPropertyClass::M2MPropertyClass(QString name, QVector<QString> paramNames, QVector<double> paramDefaults, M2MPropertyClass *parent)
    : mParent(parent),
      mName(name),
      mParameterNames(paramNames),
      mParameterDefaults(paramDefaults)
{

}


bool M2MPropertyClass::hasParent() const {
    return mParent != nullptr;
}

M2MPropertyClass *M2MPropertyClass::getParent() const {
    return mParent;
}

QVector<QString> M2MPropertyClass::getParameterNames() const {
    return mParameterNames;
}

QVector<double> M2MPropertyClass::getDefaultParameters() const {
    return mParameterDefaults;
}
