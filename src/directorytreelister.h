#ifndef DIRECTORYTREELISTER_H
#define DIRECTORYTREELISTER_H

#include <QObject>
#include <QDir>
#include <QTreeWidget>
#include "npfile.h"

#define SEP



class DirectoryTreeLister : public QObject
{
    Q_OBJECT
public:
    enum ScreenType { MainWindow, NewFile };

    DirectoryTreeLister(QTreeWidget *parent, ScreenType type);
    ~DirectoryTreeLister();

    void buildList(QDir *home_dir);
    NPFile *getCurrentSelection();
    QTreeWidgetItem *getRoot();
    void clearSelected();
    NPFile *addToTree(QString filename);

signals:
    void itemActivated(NPFile *item);
    void itemClicked(NPFile * item);
public slots:
    void itemActivated(QTreeWidgetItem *item, int column);
    void itemClicked(QTreeWidgetItem *item, int column);
    void dropDirectoryIn(QString direc);
    void directoryChanged(QString directory);
private :
        void addToRoot(NPFile *nipper);
    void buildListOpenDirectory(QDir *directory, NPFile *item);
    QTreeWidget *treeWidget;
    QString getModifiedDate(QFile *file);
    NPFile *tem;
    QDir *root_dir;
    //not proud of the following hack, the tree should be more robust
    NPFile *getChild(QTreeWidgetItem *papa, QString nicole);
    NPFile *getChild(NPFile *papa, QString nicole);

    // to support the directory watching malarky
    QFileSystemWatcher *watcher;
    void killChildrenThenKillParent(QString parent);
    NPFile *getNPFileIfExists(QString filename);
    NPFile *getChildIfExists(NPFile *papa, QString nicole);
};

#endif // DIRECTORYTREELISTER_H
