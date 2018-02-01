#ifndef M2MPROPERTY_H
#define M2MPROPERTY_H

#include <QString>
#include <QVector>


/**
 * @brief The M2MPropertyClass represents a property class in Tima's Map2Mesh code.
 */
class M2MPropertyClass {
public:
    /**
     * @brief Defines a class with no parameters.
     * @param name The name of the class.
     * @param parent The parent of the class (or nullptr).
     */
    M2MPropertyClass(QString name, M2MPropertyClass *parent = nullptr);

    /**
     * @brief Defines a class with parameters.
     * @param name The name of the class.
     * @param paramNames The parameter names.
     * @param paramDefaults The parameter default values.
     * @param parent The parent of the class (or nullptr).
     */
    M2MPropertyClass(QString name, QVector<QString> paramNames, QVector<double> paramDefaults, M2MPropertyClass *parent = nullptr);

    bool hasParent() const;
    M2MPropertyClass *getParent() const;

    QVector<QString> getParameterNames() const;
    QVector<double> getDefaultParameters() const;

protected:
    /**
     * @brief The parent class, or nullptr.
     */
    M2MPropertyClass *mParent;

    /**
     * @brief A unique class name.
     */
    QString mName;

    /**
     * @brief The list of parameters that belong specifically to this class (not including the inherited parameters).
     */
    QVector<QString> mParameterNames;

    /**
     * @brief Parallel to mParameterNames. Defines default values.
     */
    QVector<double> mParameterDefaults;
};

#endif // M2MPROPERTY_H
