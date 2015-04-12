#ifndef TOPANDTAIL_H
#define TOPANDTAIL_H

#include <QDialog>

//#include "hillstreeteditor.h"

namespace Ui {
    class TopAndTail;
}

class TopAndTail : public QDialog
{
    Q_OBJECT

public:
    explicit TopAndTail(QWidget *parent = 0);
    ~TopAndTail();

protected:
    void closeEvent(QCloseEvent *);
private:
    Ui::TopAndTail *ui;
    HillStreetEditor *editor;

public slots:
    void setEditor(HillStreetEditor *editor);

signals:
    void closed();
};

#endif // TOPANDTAIL_H
