#include "topandtail.h"
#include "ui_topandtail.h"

TopAndTail::TopAndTail(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TopAndTail)
{
    ui->setupUi(this);
}

TopAndTail::~TopAndTail()
{
    delete ui;
}

void TopAndTail::closeEvent(QCloseEvent *)
{
    emit closed();
}

void TopAndTail::setEditor(HillStreetEditor *editor)
{
    this->editor = editor;
}
