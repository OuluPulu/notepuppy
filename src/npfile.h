#ifndef NOTEPUPPYTREEWIDGETITEM_H
#define NOTEPUPPYTREEWIDGETITEM_H

#include <QDir>
#include <QFile>
#include <QFileSystemWatcher>
#include <QTreeWidgetItem>

// confusingly, the Windows and the Unix builds all use this.
#define FSEP   "/"




class NPFile : public QTreeWidgetItem
{
public:
    NPFile(NPFile *parent, QString filename, bool isDirectory = false);
    NPFile(QString filename, bool isDirectory = false);
    ~NPFile();
    QString getFileName();
    bool isFile();
    bool isDirectory();
    bool izDirectory;
    QString getShortFileName();
    QString getFilePath();
    QDir *getDirectory();
    void save(QString data);
/* TODO */    void renameFile(QString newName);
    void incrementFileCount();
    void decrementFileCount();
    int getFileCount();
    void addLabels();

private:
    QString actualFile;
    QFile *actualActualFile;
    QDir *actualActualDir;
    int fileCount;
    void configureFile();
    void configureDir();
    void init(QString filename, bool isDirectory);

    //should refactor, cba. Here are directory only settings
    QFileSystemWatcher *watcher;
    NPFile* parentDir;
};



#endif // NOTEPUPPYTREEWIDGETITEM_H
