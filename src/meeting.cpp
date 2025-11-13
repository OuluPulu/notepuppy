#include "meeting.h"
#include "ui_meeting.h"

#include <QDateTime>
#include <QRegularExpression>

Meeting::Meeting(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Meeting)
{
    ui->setupUi(this);
}

Meeting::~Meeting()
{
    delete ui;
}

QString Meeting::getMeetingName()
{
    QString people = ui->lineEdit_People->text();
    if (people.isEmpty()) people = "no peoople";
    people.remove(QRegularExpression(R"([\\/:*?"<>|,;])"));
    people.replace(' ', '_');
    QString subject = ui->lineEdit_Subject->text();
    if (subject.isEmpty()) subject = "no subject";
    subject.replace(' ', '_');
    return subject + "/" + people + "__at__" + QDateTime::currentDateTime().toString("yyyymmdd_hh:mm");
}
