#include "m2mpropertyinstance.h"

M2MPropertyInstance::M2MPropertyInstance()
{

}


M2MPropertyInstance M2MPropertyInstance::createInstance(M2MPropertyClass *ofClass) {
    Q_ASSERT( ofClass != nullptr );

    M2MPropertyInstance inst;
    inst.mClassPtr = ofClass;

    // Set the instance's parameters.
    M2MPropertyClass *current = ofClass;
    while (current != nullptr) {
        QMap<QString, double> paramToVal;

        QVector<QString> paramNames = current->getParameterNames();
        QVector<double> paramVals = current->getDefaultParameters();

        for (int i = 0; i < paramNames.size(); ++i)
            paramToVal.insert(paramNames[i], paramVals[i]);

        inst.mParameters[current] = paramToVal;

        current = ofClass->getParent();
    }

    return inst;
}

M2MPropertyInstance M2MPropertyInstance::createInstance(M2MPropertyClass *ofClass, QVector<ParamTriplet> params) {
    // Create an instance with default parameters.
    M2MPropertyInstance inst = M2MPropertyInstance::createInstance(ofClass);

    // Set the specified parameters.
    for (ParamTriplet t : params) {
        // This assert ensures that t.first is a parent class of the instance.
        Q_ASSERT(inst.mParameters.contains(t.first));

        // This assert ensures that t.second is a parameter defined in the class t.first.
        Q_ASSERT(inst.mParameters[t.first].contains(t.second));


        inst.mParameters[t.first][t.second] = t.third;
    }

    return inst;
}


double M2MPropertyInstance::getParameter(M2MPropertyClass *owningClass, QString param) const {

    // If this assert is triggered, either owningClass is not a superclass of this instance,
    // or the instance was not created correctly.
    Q_ASSERT( mParameters.contains(owningClass) );

    // If this assert is triggered, then either:
    //  - param isn't defined in owningClass
    //  - param wasn't defined in owningClass when this instance was created (shouldn't happen!)
    //  - this instance was not created correctly
    Q_ASSERT( mParameters[owningClass].contains(param) );

    return mParameters[owningClass][param];
}

M2MPropertyClass *M2MPropertyInstance::getClass() const {
    return mClassPtr;
}



bool M2MPropertyInstance::operator==(const M2MPropertyInstance &other) const {
    return mClassPtr == other.mClassPtr && mParameters == other.mParameters;
}

bool M2MPropertyInstance::operator!=(const M2MPropertyInstance &other) const {
    return mClassPtr != other.mClassPtr || mParameters != other.mParameters;
}
