#ifndef NPDIRECTORY_H
#define NPDIRECTORY_H

#include <QTreeWidgetItem>

class NPDirectory : public QTreeWidgetItem
{
public:
    explicit NPDirectory(QObject *parent = 0);
};

#endif // NPDIRECTORY_H
