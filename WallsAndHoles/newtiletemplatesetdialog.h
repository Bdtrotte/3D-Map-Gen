#ifndef NEWTILETEMPLATESETDIALOG_H
#define NEWTILETEMPLATESETDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

struct NewTileTemplateSetData
{
    QString name;
    QString fileLocation;

    NewTileTemplateSetData(QString n = "", QString fl = "") : name(n), fileLocation(fl) {}
};

class NewTileTemplateSetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewTileTemplateSetDialog(QWidget *parent = 0);

    NewTileTemplateSetData result;

private slots:
    void ok();
    void cancel();
    void findFile();

private:
    QPushButton *mOk;
    QPushButton *mCancel;
    QPushButton *mFindFile;

    QLineEdit *mName;
    QLineEdit *mFilePath;
};

#endif // NEWTILETEMPLATESETDIALOG_H
