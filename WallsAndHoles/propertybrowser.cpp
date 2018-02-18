#include "propertybrowser.h"

#include <QVariant>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QPushButton>

PropertyBrowser::PropertyBrowser(QWidget *parent)
    : QWidget(parent)
    , mPropertyManager(nullptr)
    , mMainLayout(new QVBoxLayout(this))
{

}

void PropertyBrowser::setPropertyManager(AbstractPropertyManager *propertyManager)
{
    if (mPropertyManager)
        clear();

    const QVector<QPair<QString, QVariant>> &properties = propertyManager->properties();

    for (const QPair<QString, QVariant> &prop : properties)
        makeLine(prop.first, prop.second);

    setLayout(mMainLayout);

    connect(propertyManager, &AbstractPropertyManager::propertyChanged,
            this, [this](QString property, QVariant value)
    {
        setLine(property, value);
    });
}

void PropertyBrowser::clear()
{
    mLines.clear();
    mLineType.clear();
    for (QWidget *w : mLineWidget) delete w;
    for (QLabel *w : mLineLabel) delete w;
    mLineWidget.clear();
    mLineLabel.clear();
    delete mMainLayout;

    mMainLayout = new QVBoxLayout(this);
}

void PropertyBrowser::makeLine(QString propertyName, QVariant value)
{
    QHBoxLayout *line = new QHBoxLayout(this);
    QMetaType::Type type = mLineType[propertyName] = (QMetaType::Type)value.type();

    QLabel *label = new QLabel(propertyName, this);
    line->addWidget(label);

    mLines[propertyName] = line;
    mLineType[propertyName] = type;
    mLineLabel[propertyName] = label;

    switch(type) {
    case QMetaType::Int: {
        QSpinBox *w = new QSpinBox(this);
        w->setValue(value.toInt());
        line->addWidget(w);
        connect(w, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                this, [this, propertyName](int i) { mPropertyManager->propertyEdited(propertyName, i); });

        mLineWidget[propertyName] = w;
        break; }
    case QMetaType::Float:
    case QMetaType::Double: {
        QDoubleSpinBox *w = new QDoubleSpinBox(this);
        w->setValue(value.toDouble());
        line->addWidget(w);
        connect(w, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                this, [this, propertyName](double i) { mPropertyManager->propertyEdited(propertyName, i); });

        mLineWidget[propertyName] = w;
        break; }
    case QMetaType::QString: {
        QLineEdit *w = new QLineEdit(this);
        w->setText(value.toString());
        line->addWidget(w);
        connect(w, &QLineEdit::textChanged,
                this, [this, propertyName](QString i) { mPropertyManager->propertyEdited(propertyName, i); });

        mLineWidget[propertyName] = w;
    }
    case QMetaType::QColor: {
        // TODO Implement color
    }
    case QMetaType::User: {
        // TODO Implement Sub property browser
        break;
    }
    default:
        //Must have a delt with type!
        Q_ASSERT(false);
    }

    mMainLayout->addLayout(line);
}

void PropertyBrowser::setLine(QString propertyName, QVariant value)
{
    //Prevents a signal loop
    mPropertyManager->blockSignals(true);

    QMetaType::Type type = mLineType[propertyName];

    QWidget *widget = mLineWidget[propertyName];

    switch(type) {
    case QMetaType::Int:
        static_cast<QSpinBox *>(widget)->setValue(value.toInt());
        break;
    case QMetaType::Float:
    case QMetaType::Double:
        static_cast<QDoubleSpinBox *>(widget)->setValue(value.toDouble());
        break;
    case QMetaType::QString:
        static_cast<QLineEdit *>(widget)->setText(value.toString());
        break;
    case QMetaType::QColor:
    case QMetaType::User:
        break;
    default:
        //Must have a delt with type!
        Q_ASSERT(false);
    }

    mPropertyManager->blockSignals(false);
}
