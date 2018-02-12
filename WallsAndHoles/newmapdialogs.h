#ifndef NEWMAPDIALOGS_H
#define NEWMAPDIALOGS_H

#include <QWidget>
#include <QDialog>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QAbstractButton>
#include <QCheckBox>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>

struct NewMapDatas
{
    int width;
    int height;

    NewMapDatas(int w, int h) : width(w), height(h) {}
};

class NewMapDialogs : public QWidget
{
    Q_OBJECT
public:
    explicit NewMapDialogs(QWidget *parent = nullptr);

    NewMapDatas result;

signals:

public slots:
};

#endif // NEWMAPDIALOGS_H
