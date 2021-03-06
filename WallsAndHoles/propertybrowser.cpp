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

    connect(mPropertyManager, &AbstractPropertyManager::blankProperty,
            this, [this](QString property)
    {
        blankLine(property);
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
    QBoxLayout *line = new QHBoxLayout();
    QMetaType::Type type = (QMetaType::Type)value.type();
    mLineType[propertyName] = type;

    QLabel *label = new QLabel(propertyName, mMainWidget);
    line->addWidget(label);

    mLineType[propertyName] = type;
    mLineLabel[propertyName] = label;

    switch(type) {
    case QMetaType::Int: {
        QSpinBox *w = new QSpinBox(mMainWidget);
        if (!extra.isEmpty()) {
            w->setMinimum(extra.takeFirst().toInt());
            if (!extra.isEmpty()) {
                w->setMaximum(extra.takeFirst().toInt());
                if (!extra.isEmpty() && extra.takeFirst().toBool()) {
                    w->setMinimum(w->minimum() - 1);
                    w->setValue(w->minimum());
                    w->setSpecialValueText("--");
                }
            }
        }

        w->setEnabled(editable);

        if (w->specialValueText().isEmpty())
            w->setValue(value.toInt());
        line->addWidget(w);
        connect(w, &QSpinBox::editingFinished,
                this, [this, propertyName, w]() {
            if (!w->specialValueText().isEmpty()) {
                w->setMinimum(w->minimum() + 1);
                w->setSpecialValueText("");
            }

            mPropertyManager->propertyEdited(propertyName, w->value());
        });

        mLineWidget[propertyName] = w;
        break; }
    case QMetaType::Float:
    case QMetaType::Double: {
        QDoubleSpinBox *w = new QDoubleSpinBox(mMainWidget);
        bool blank = false;
        if (!extra.isEmpty()) {
            w->setMinimum(extra.takeFirst().toDouble());
            if (!extra.isEmpty()) {
                w->setMaximum(extra.takeFirst().toDouble());
                if (!extra.isEmpty() && extra.takeFirst().toBool()) {
                    w->setMinimum(w->minimum() - 1);
                    w->setValue(w->minimum());
                    w->setSpecialValueText("--");

                    blank = true;
                }
            }
        }

        w->setEnabled(editable);

        if (!blank)
            w->setValue(value.toDouble());
        line->addWidget(w);
        connect(w, &QDoubleSpinBox::editingFinished,
                this, [this, propertyName, w]() {
            if (!w->specialValueText().isEmpty()) {
                w->setMinimum(w->minimum() + 1);
                w->setSpecialValueText("");
            }

            mPropertyManager->propertyEdited(propertyName, w->value());
        });

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
        int customType = extra.takeFirst().toInt();
        switch (customType) {
        case LineWidget: {
            QWidget *w = value.value<QWidget *>();

            line->addWidget(w);

            mLineWidget[propertyName] = w;
        break; }
        case BlockWidget: {
            QHBoxLayout *top = static_cast<QHBoxLayout *>(line);
            line = new QVBoxLayout;
            line->addLayout(top);

            QWidget *w = value.value<QWidget *>();

            line->addWidget(w);

            mLineWidget[propertyName] = w;
        break; }
        case PropertyManager: {
            QHBoxLayout *top = static_cast<QHBoxLayout *>(line);
            line = new QVBoxLayout;
            line->addLayout(top);

            PropertyBrowser *w = new PropertyBrowser(this);
            w->setPropertyManager(value.value<AbstractPropertyManager *>());

            line->addWidget(w);

            mLineWidget[propertyName] = w;
        break; }
        }

        break;
    }
    default:
        //Must have a delt with type!
        Q_ASSERT(false);
    }

    mLines[propertyName] = line;
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
    default:
        //Must have a delt with type!
        Q_ASSERT(false);
    }

    mPropertyManager->blockSignals(false);
}

void PropertyBrowser::blankLine(QString propertyName)
{
    //Prevents a signal loop
    mPropertyManager->blockSignals(true);

    QMetaType::Type type = mLineType[propertyName];

    QWidget *widget = mLineWidget[propertyName];

    switch(type) {
    case QMetaType::Int: {
        QSpinBox *w = static_cast<QSpinBox *>(widget);
        w->setMinimum(w->minimum() - 1);
        w->setValue(w->minimum());
        w->setSpecialValueText("--");
        break; }
    case QMetaType::Float:
    case QMetaType::Double: {
        QDoubleSpinBox *w = static_cast<QDoubleSpinBox *>(widget);
        w->setMinimum(w->minimum() - 1);
        w->setValue(w->minimum());
        w->setSpecialValueText("--");
        break; }
    case QMetaType::QString:
        static_cast<QLineEdit *>(widget)->setText("");
        break;
    default:
        //Must have a delt with type!
        Q_ASSERT(false);
    }

    mPropertyManager->blockSignals(false);
}
