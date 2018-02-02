#ifndef M2MPROPERTYINSTANCE_H
#define M2MPROPERTYINSTANCE_H

#include <QVector>
#include <QString>
#include <QMap>
#include <QPair>

#include "m2mpropertyclass.h"


template< typename X, typename Y, typename Z > struct Triplet;

/**
 * @brief The M2MPropertyInstance class represents an instance of a property class in Tima's map2mesh code.
 */
class M2MPropertyInstance {
public:

    using ParamTriplet = Triplet<M2MPropertyClass *, QString, float>;

    /**
     * @brief Creates an instance of the class with default parameter values.
     *
     * The reason this method is in M2MPropertyInstance instead of M2MPropertyClass
     * is so that nonpublic attributes can be set.
     *
     * @param ofClass The parent class of the instance.
     * @return An instance of the given class.
     */
    static M2MPropertyInstance createInstance(M2MPropertyClass *ofClass);


    /**
     * @brief Creates an instance of the class with the given parameter values.
     * @param ofClass The parent class of the instance.
     * @param params The parameters to supply. Format for each entry is {{class, param name}, param value}.
     * Unspecified parameters remain at their default values.
     * @return An instance of the given class.
     */
    static M2MPropertyInstance createInstance(M2MPropertyClass *ofClass, QVector<ParamTriplet> params);


    /**
     * @brief Gets the value of the parameter.
     * @param owningClass The class that defines the parameter.
     * @param param The name of the parameter.
     * @return The value of the parameter for this instance.
     */
    double getParameter(M2MPropertyClass *owningClass, QString param) const;


    /**
     * @brief Get the class of the instance.
     * @return The class.
     */
    M2MPropertyClass *getClass() const;



    bool operator==(const M2MPropertyInstance &other) const;
    bool operator!=(const M2MPropertyInstance &other) const;


protected:
    M2MPropertyInstance();

    /**
     * @brief A pointer to the class that this is directly an instance of.
     */
    M2MPropertyClass *mClassPtr;

    /**
     * @brief The parameters for this object. All parameters are double values.
     *
     * mParameters maps classes to parameter dictionaries. Every superclass of this
     * instance must be accounted for.
     */
    QMap<M2MPropertyClass *, QMap<QString, double>> mParameters;
};


template< typename X, typename Y, typename Z >
struct Triplet {
    X first;
    Y second;
    Z third;
};


#endif // M2MPROPERTYINSTANCE_H
