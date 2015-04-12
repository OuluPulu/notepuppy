#ifndef FILEINFO_H
#define FILEINFO_H

#include <QDialog>

namespace Ui {
    class FileInfo;
}

class FileInfo : public QDialog
{
    Q_OBJECT

public:
    explicit FileInfo(QWidget *parent = 0);
    ~FileInfo();

private:
    Ui::FileInfo *ui;
};

#endif // FILEINFO_H
