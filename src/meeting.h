#ifndef MEETING_H
#define MEETING_H

#include <QDialog>

namespace Ui {
class Meeting;
}

class Meeting : public QDialog
{
    Q_OBJECT

public:
    explicit Meeting(QWidget *parent = nullptr);
    ~Meeting();

    QString getMeetingName();

private:
    Ui::Meeting *ui;
};

#endif // MEETING_H
