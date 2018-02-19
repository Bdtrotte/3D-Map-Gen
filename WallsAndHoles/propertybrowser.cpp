#include "propertybrowser.h"

#include "colorpickerbutton.h"

#include <QVariant>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>

PropertyBrowser::PropertyBrowser(QWidget *parent)
    : QScrollArea(parent)
    , mMainWidget(new QWidget(this))
    , mPropertyManager(nullptr)
    , mMainLayout(new QVBoxLayout(mMainWidget))
{
    mMainWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    setWidgetResizable(true);
    setWidget(mMainWidget);
}

void PropertyBrowser::setPropertyManager(AbstractPropertyManager *propertyManager)
{
    if (mPropertyManager)
        clear();

    mPropertyManager = propertyManager;
    if (!mPropertyManager) return;

    mPropertyManager->setParent(mMainWidget);

    QVector<QVector<QVariant>> properties = mPropertyManager->properties();

    for (QVector<QVariant> &prop : properties)
        makeLine(prop[0].toString(), prop[1], prop[2].toBool(), prop.mid(3));

    connect(mPropertyManager, &AbstractPropertyManager::propertyChanged,
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

    while (QLayoutItem *item = mMainLayout->takeAt(0))
        delete item;

    if (mPropertyManager)
        delete mPropertyManager;

    mPropertyManager = nullptr;
}

void PropertyBrowser::makeLine(QString propertyName, QVariant value, bool editable, QVector<QVariant> extra)
{
    QHBoxLayout *line = new QHBoxLayout();
    QMetaType::Type type = (QMetaType::Type)value.type();
    mLineType[propertyName] = type;

    QLabel *label = new QLabel(propertyName, mMainWidget);
    line->addWidget(label);

    mLines[propertyName] = line;
    mLineType[propertyName] = type;
    mLineLabel[propertyName] = label;

    switch(type) {
    case QMetaType::Int: {
        QSpinBox *w = new QSpinBox(mMainWidget);
        if (!extra.isEmpty()) {
            w->setMinimum(extra.takeFirst().toInt());
            if (!extra.isEmpty()) {
                w->setMaximum(extra.takeFirst().toInt());
            }
        }

        w->setEnabled(editable);

        w->setValue(value.toInt());
        line->addWidget(w);
        connect(w, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                this, [this, propertyName](int i) { mPropertyManager->propertyEdited(propertyName, i); });

        mLineWidget[propertyName] = w;
        break; }
    case QMetaType::Float:
    case QMetaType::Double: {
        QDoubleSpinBox *w = new QDoubleSpinBox(mMainWidget);
        if (!extra.isEmpty()) {
            w->setMinimum(extra.takeFirst().toDouble());
            if (!extra.isEmpty()) {
                w->setMaximum(extra.takeFirst().toDouble());
            }
        }

        w->setEnabled(editable);

        w->setValue(value.toDouble());
        line->addWidget(w);
        connect(w, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                this, [this, propertyName](double i) { mPropertyManager->propertyEdited(propertyName, i); });

        mLineWidget[propertyName] = w;
        break; }
    case QMetaType::QString: {
        QLineEdit *w = new QLineEdit(mMainWidget);
        w->setText(value.toString());

        w->setEnabled(editable);

        line->addWidget(w);
        connect(w, &QLineEdit::textChanged,
                this, [this, propertyName](QString i) { mPropertyManager->propertyEdited(propertyName, i); });

        mLineWidget[propertyName] = w;
        break; }
    case QMetaType::QColor: {
        ColorPickerButton *w = new ColorPickerButton(value.value<QColor>(), mMainWidget);

        w->setEnabled(editable);

        line->addWidget(w);
        connect(w, &ColorPickerButton::colorPicked,
                this, [this, propertyName](QColor i) { mPropertyManager->propertyEdited(propertyName, QVariant::fromValue<QColor>(i)); });

        mLineWidget[propertyName] = w;
        break; }
    case QMetaType::Bool: {
        QCheckBox *w = new QCheckBox(mMainWidget);
        w->setChecked(value.toBool());

        w->setEnabled(editable);

        line->addWidget(w);
        connect(w, &QCheckBox::stateChanged,
                this, [this, propertyName](int i) { mPropertyManager->propertyEdited(propertyName, i); });

        mLineWidget[propertyName] = w;
        break; }
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
        break;
    case QMetaType::Bool:
        static_cast<QCheckBox *>(widget)->setChecked(value.toBool());
        break;
    case QMetaType::User:
        break;
    default:
        //Must have a delt with type!
        Q_ASSERT(false);
    }

    mPropertyManager->blockSignals(false);
}
