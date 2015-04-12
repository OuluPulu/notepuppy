#include "fileinfo.h"
#include "ui_fileinfo.h"

FileInfo::FileInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileInfo)
{
    ui->setupUi(this);
}

FileInfo::~FileInfo()
{
    delete ui;
}
