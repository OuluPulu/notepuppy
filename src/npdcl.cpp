#include "npdcl.h"

NPdcl::NPdcl(QObject *parent, NPFile *directory) :
        QObject(parent)
{
    this->directory = directory;
    this->watcher = new QFileSystemWatcher(this);

    //connect up the signals
    connect(watcher,SIGNAL(directoryChanged(QString)),this,SLOT(directoryChanged(QString)));
}

void NPdcl::directoryChanged(QString directory)
{
    static QString drec = this->directory->getFileName();

    if (drec == directory)
    {
        int tagdjnk = 20;
        int ghdkavbfjka = 38;
    }
}
