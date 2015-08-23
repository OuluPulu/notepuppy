#ifndef NEWFILEDIALOG_H
#define NEWFILEDIALOG_H

#include <QDialog>
#include "directorytreelister.h"

#define DUB_QUOTES          "\""
#define DOT_TXT             ".txt"
#define REP_CHAR            "$"

namespace Ui {
    class NewFileDialog;
}

class NewFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewFileDialog(QDir *rootDirectory, QWidget *parent, QString caption, QString filename = "", bool dir = false);
    ~NewFileDialog();
    QString getFile();
    QString getDirectory();
    NPFile *getSelected();
    bool isDirectory();
public slots:
    void itemClicked(NPFile *item);
    void insertDate();
private:
    Ui::NewFileDialog *ui;
    DirectoryTreeLister *dtl;
    NPFile *item;
    QDir *rootDir;
    bool hasRecognisedEnding(QString *file);
    void makeLegalName(QString *file);
};

#endif // NEWFILEDIALOG_H
