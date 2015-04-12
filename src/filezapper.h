#ifndef FILEZAPPER_H
#define FILEZAPPER_H

#include "npfile.h"
#include "smalldevicewindow.h"

#include <QDialog>
#include <QDir>

namespace Ui {
    class FileZapper;
}

class FileZapper : public QDialog
{
    Q_OBJECT

public:
    explicit FileZapper(QDir *rootDir, SmallDeviceWindow *parent = 0);
    ~FileZapper();

    void close();

public slots:

signals:
    void cancelled();

private:
    Ui::FileZapper *ui;

    SmallDeviceWindow *m_parent;
    QWidget *m_widget;

    NPFile *createNewFile(QString filename);
private slots:
    void okButtonZapped(QString filename, bool isDirectory);
    void cancelButtonZapped();

};

#endif // FILEZAPPER_H
