#ifndef NEWFILEWIDGET_H
#define NEWFILEWIDGET_H

#include "directorytreelister.h"
#include "npfile.h"

#include <QDir>
#include <QWidget>

#define DUB_QUOTES          "\""
#define DOT_TXT             ".txt"
#define REP_CHAR            "-np-"


namespace Ui {
    class NewFileWidget;
}

/**
 * This class is a simple copy of the NewFileDialog. I think it is terrible
 * bad form for a class to be copy pasted, and of course one day I will
 * refctor it. In the mean time, it remains a part of the new UI components.
 */
class NewFileWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NewFileWidget(QDir *rootDirectory, QWidget *parent, QString caption, QString filename = "", bool dir = false);
    ~NewFileWidget();
    QString getFile();
    QString getDirectory();
    NPFile *getSelected();
    bool isDirectory();
    DirectoryTreeLister *directoryTreeLister() const { return dtl; }
    void clearFileLine();
private slots:
    void itemClicked(NPFile *item);
    void okBP();
    void canBP();
signals:
    void okButtonPressed(QString fileString, bool isDirectory);
    void cancelButtonPressed();
private:
    Ui::NewFileWidget *ui;
    DirectoryTreeLister *dtl;
    NPFile *item;
    QDir *rootDir;
    bool hasRecognisedEnding(QString *file);
    void makeLegalName(QString *file);
};

#endif // NEWFILEWIDGET_H
