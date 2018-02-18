#include "propertybrowser.h"

#include <QVariant>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QPushButton>

PropertyBrowser::PropertyBrowser(QWidget *parent)
    : QWidget(parent) {}

void PropertyBrowser::setPropertyManager(AbstractPropertyManager *propertyManager)
{
    const QVector<QPair<QString, QVariant>> &properties = propertyManager->properties();

    for (const QPair<QString, QVariant> &prop : properties)
        makeLine(prop.first, prop.second);

    connect(propertyManager, &AbstractPropertyManager::propertyChanged,
            this, [this](QString property, QVariant value)
    {
        setLine(property, value);
    });
}

void PropertyBrowser::clear()
{
    mLines.clear();
    delete mMainLayout;

    mMainLayout = new QVBoxLayout(this);
}

void PropertyBrowser::makeLine(QString propertyName, QVariant value)
{
    QHBoxLayout *line = new QHBoxLayout(this);
    mLines[propertyName] = line;
    QMetaType::Type type = mLineType[propertyName] = value.type();

    line->addWidget(QLabel(propertyName, this));

    switch(type) {
    case QMetaType::Int: {
        QSpinBox *w = new QSpinBox(this);
        w->setValue(value.toInt());
        line->addWidget(w);
        connect(w, &QSpinBox::valueChanged,
                this, [this](int i) {  });
        break; }
    case QMetaType::Double: {
        QDoubleSpinBox *w = new QDoubleSpinBox(this);
        w->setValue(value.toDouble());
        break; }
    case QMetaType::Float: {
        QDoubleSpinBox *w = new QDoubleSpinBox(this);
        w->setValue(value.toFloat());
        line->addWidget(w);
        break; }
    case QMetaType::QString: {
        QLineEdit *w = new QLineEdit(this);
        w->setText(value.toString());
        line->addWidget(w);
    }
    case QMetaType::QColor: {
        // TODO Implement color
    }
    default:
        //Must have a delt with type!
        Q_ASSERT(false);
    }

    mMainLayout->addLayout(line);
}

void PropertyBrowser::setLine(QString propertyName, QVariant value)
{

}

QVariant PropertyBrowser::getLineValue(QString propertyName)
{

}

void PropertyBrowser::propertyEdited(QString propertyName, QVariant newValue)
{

}
