#include "directorytreelister.h"
#include <QDateTime>
#include <QHeaderView>
#include <QMutex>

DirectoryTreeLister::DirectoryTreeLister(QTreeWidget *parent,  ScreenType type) :
    QObject(parent)
{
    root_dir = 0x0;
    this->treeWidget = parent;
    this->treeWidget->setSortingEnabled(true);
    this->treeWidget->sortItems(0,Qt::AscendingOrder);
    QStringList labels;
    labels << tr("File:") << tr("Size:") << tr("Date Modified:");
    treeWidget->setHeaderLabels(labels);
    this->tem = 0x0;
    watcher = new QFileSystemWatcher(this);

    if (type == MainWindow)
    {
        QHeaderView *qhv = treeWidget->header();
        qhv->resizeSection(0, 498);
        qhv->resizeSection(1, 98);
        qhv->resizeSection(2, 198);
    }
    else if (type == NewFile)
    {
        QHeaderView *qhv = treeWidget->header();
        qhv->resizeSection(0, 398);
        qhv->resizeSection(1, 98);
        qhv->resizeSection(2, 198);
    }

    connect(parent, SIGNAL(itemActivated(QTreeWidgetItem*,int)), this, SLOT(itemActivated(QTreeWidgetItem*,int)));
    connect(parent, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(itemClicked(QTreeWidgetItem*,int)));
    connect(watcher, SIGNAL(directoryChanged(QString)), this, SLOT(directoryChanged(QString)));
}

NPFile *DirectoryTreeLister::getCurrentSelection()
{
    return this->tem;
}

void DirectoryTreeLister::clearSelected()
{
    this->treeWidget->setCurrentItem(0x0);
    this->tem = 0x0;
}

void DirectoryTreeLister::directoryChanged(QString directory)
{
    // let's only allow one thread at a time through here.
    QMutex tex;

    tex.lock();
    //if (tex.tryLock())
    {
        QDir dir(directory);
        if (!dir.exists()) return; // is that the problem?
        // list directory children
        QStringList l_contents = dir.entryList(QDir::AllEntries); //just file names, no path.
        // list the listed children
        NPFile *changee = this->addToTree(directory);

        // can't list children, but can count them and then iterate.
        int kidz = changee->childCount();
        // compare
        QString stem = directory;
        stem += FSEP;
        QStringList hitlist;
        for (int i = 0; i < kidz; ++i)
        {
            NPFile *chile = (NPFile*)changee->child(i);
            if (0x0 != chile)
            {
                QString entity = chile->getShortFileName(); // returns the whole path of the file
                if (l_contents.removeAll(entity) != 0)
                { // != 0, ie the file is in both lists
                       // do nothing, it's fine.
                }
                else
                { //there were none in the list - none in actual, but is in NP list. Remove from NP list.
                    //add to the list to kill
                    hitlist.append(stem+entity);
                }
            }
        }
        //iterate the list which I have to kill
        QStringListIterator killemall(hitlist);
        while (killemall.hasNext())
        {
            this->killChildrenThenKillParent(killemall.next());
            // no, really, all this gore - I did not have fun writing this.
        }
        // what is left in l_contents has to be added
        QStringListIterator it(l_contents);
        while (it.hasNext())
        {
            QString next = it.next();
            if (!next.startsWith("."))
            {
                QString thingtoadd = directory;
                thingtoadd+=FSEP;
                thingtoadd+=next;

                QDir d(thingtoadd);
                if (d.exists())
                {
                    this->dropDirectoryIn(thingtoadd);
                }
                else
                {
                    QFile f(thingtoadd);
                    if (f.exists())
                    {
                        this->addToTree(thingtoadd);
                    }
                    else
                    {
                        // oho. it shouldn't get here, if it did then I guess do nothing. oopsista
                    }
                }
            }
        }
    }
     // unlock
#ifdef Q_WS_MAC
    watcher->addPath(directory);
#endif /* Q_WS_MAC */
    tex.unlock();
}

DirectoryTreeLister::~DirectoryTreeLister()
{
    delete root_dir;
}

void DirectoryTreeLister::killChildrenThenKillParent(QString parent)
{
    NPFile *npfile = this->getNPFileIfExists(parent);
    if (npfile != 0x0)
    {
        if (npfile->isDirectory())
        {
            int npfileChildCount = npfile->childCount();
            if (npfileChildCount > 0)
            {
                for (int i = 0; i < npfileChildCount; ++i)
                {
                    NPFile *childfile = (NPFile*)npfile->child(i);
                    if (childfile != 0x0)
                    {
                        this->killChildrenThenKillParent(childfile->getFileName());
                    }
                }
            }
        }
        // all children dead
        QTreeWidgetItem *pare = npfile->parent();
        if (0x0 != pare)
        {
            pare->removeChild(npfile);
        }
        delete(npfile);
    }
}

NPFile *DirectoryTreeLister::getChildIfExists(NPFile *papa, QString nicole)
{
    int count = papa->childCount();
    for (int i = 0; i < count; ++i)
    {
        NPFile *chid = (NPFile*)papa->child(i);
        if (chid->getShortFileName() == nicole)
        {
            return chid;
        }
    }
    return 0x0;
}

NPFile *DirectoryTreeLister::getChild(NPFile *papa, QString nicole)
{
    int count = papa->childCount();
    for (int i = 0; i < count; ++i)
    {
        NPFile *chid = (NPFile*)papa->child(i);
        if (chid->getShortFileName() == nicole)
        {
            return chid;
        }
    }
    QString nom = papa->getFileName();
    nom.append(FSEP);
    nom.append(nicole);
    QFileInfo into(nom);
    NPFile *chid = new NPFile(papa, nom, into.isDir());
    if (into.isDir())
    {
        QDir diir(nom);
        watcher->addPath(diir.absolutePath());
    }
    return chid;
}

NPFile *DirectoryTreeLister::getChild(QTreeWidgetItem *papa, QString nicole)
{

    int count = papa->childCount();
    for (int i = 0; i < count; ++i)
    {
        NPFile *chid = (NPFile*)papa->child(i);
        if (chid->getShortFileName() == nicole)
        {
            return chid;
        }
    }
    QString nom = root_dir->absolutePath();
    nom.append(FSEP);
    nom.append(nicole);
    QFileInfo into(nom);
    NPFile *chid = new NPFile(nom, into.isDir());
    if (into.isDir())
    {
        QDir diir(nom);
        watcher->addPath(diir.absolutePath());
    }
    papa->addChild(chid);
    return chid;
}

NPFile *DirectoryTreeLister::getNPFileIfExists(QString filename)
{
    NPFile *ticket = NULL;

    QFileInfo info(filename);
    QString nicename = info.filePath();//  .path();//  .canonicalFilePath();
    QString root = root_dir->path();//  ->absolutePath();

    if (nicename.startsWith(root))
    {
        //remove the first part of the name
        int count = root.count(FSEP);
        count++;
        QString restOfName = nicename.section(FSEP, count);
        QStringList ronTokens = restOfName.split(FSEP);

        if (ronTokens.length() > 0)
        {
            QTreeWidgetItem *item =  this->getRoot();
            QStringListIterator it(ronTokens);

            //do the first one
            ticket = this->getChild(item, it.next());

            while (it.hasNext())
            {
                QString nex = it.next();
                ticket = this->getChildIfExists(ticket, nex);
            }
        }
    }
    return ticket;
}

NPFile *DirectoryTreeLister::addToTree(QString filename)
{
    NPFile *ticket = NULL;

    QFileInfo info(filename);
    QString nicename = info.filePath();//  .path();//  .canonicalFilePath();
    QString root = root_dir->path();//  ->absolutePath();

    if (nicename == root)
    {
        //scandalous hack, asking for trouble later. deal with it.
        ticket = (NPFile*)this->getRoot();
    }
    else if (nicename.startsWith(root))
    {
        //remove the first part of the name
        int count = root.count(FSEP);
        count++;
        QString restOfName = nicename.section(FSEP, count);
        QStringList ronTokens = restOfName.split(FSEP);

        if (ronTokens.length() > 0)
        {
            QTreeWidgetItem *item =  this->getRoot();
            QStringListIterator it(ronTokens);

            //do the first one
            ticket = this->getChild(item, it.next());

            while (it.hasNext())
            {
                QString nex = it.next();
                ticket = this->getChild(ticket, nex);
            }
        }
    }
    return ticket;
}

QTreeWidgetItem * DirectoryTreeLister::getRoot()
{
    return treeWidget->invisibleRootItem();
}

void DirectoryTreeLister::addToRoot(NPFile *nipper)
{
    treeWidget->addTopLevelItem(nipper);
}

void DirectoryTreeLister::buildList(QDir *home_dir)
{
    if (NULL != root_dir)
    {
        delete root_dir;
    }
    QString root_biir = home_dir->absolutePath();
    root_dir = new QDir(root_biir);
    watcher->addPath(root_biir);
    treeWidget->clear();
    QStringList directories = home_dir->entryList(QDir::AllDirs);
    QStringList files = home_dir->entryList(QDir::Files);
    QStringListIterator d_it(directories);
    QStringListIterator f_it(files);
    QList<QTreeWidgetItem*> list;
    list+=getRoot();

    while (d_it.hasNext())
    {
        QString name = d_it.next();
        if (!name.startsWith("."))
        {
            home_dir->cd(name);
            QString actualName = home_dir->path();//  ->absolutePath();
            NPFile *item = new NPFile( actualName, true);
            watcher->addPath(actualName);//home_dir->canonicalPath());
            list+=item;
            buildListOpenDirectory(home_dir,item);
            home_dir->cdUp();
        }
    }
    QString basePath = home_dir->path();//  ->canonicalPath();
    while (f_it.hasNext())
    {
        QString name = basePath;
        name.append(FSEP);
        QString shortName =  f_it.next();
        name.append(shortName);
        NPFile *item = new NPFile(name);
        list+=item;
    }
    treeWidget->addTopLevelItems(list);
}

void DirectoryTreeLister::itemClicked(QTreeWidgetItem *item, int column)
{
    NPFile* puppyItem = (NPFile*)item;
    column*=1;
    tem = puppyItem;
    emit itemClicked(puppyItem);
}

void DirectoryTreeLister::itemActivated(QTreeWidgetItem *item, int column)
{
    NPFile* puppyItem = (NPFile*)item;
    column*=1;
    tem = puppyItem;
    emit itemActivated(puppyItem);
}

void DirectoryTreeLister::dropDirectoryIn(QString direc)
{
    //list the files
    QDir directory(direc);
    QStringList directories = directory.entryList(QDir::AllDirs,QDir::Name|QDir::Reversed);
    QStringList files = directory.entryList(QDir::Files, QDir::Name|QDir::Reversed);
    QStringListIterator d_it(directories);
    QStringListIterator f_it(files);

    //iterate. each directory gets sent back here, each file gets added. simples.
    while (d_it.hasNext())
    {
        QString nex = d_it.next();
        if (!nex.startsWith("."))
        {
            QString parf = direc;
            parf.append(FSEP);
            parf.append(nex);
            dropDirectoryIn(parf);
        }
    }

    while (f_it.hasNext())
    {
        QString nex = f_it.next();
        if (!nex.startsWith("."))
        {
            QString fiel = direc;
            fiel.append(FSEP);
            fiel.append(nex);
            this->addToTree(fiel);
        }
    }
    this->addToTree(direc);
}


void DirectoryTreeLister::buildListOpenDirectory(QDir *directory, NPFile *parent)
{
    QStringList directories = directory->entryList(QDir::AllDirs);
    QStringList files = directory->entryList(QDir::Files);
    QStringListIterator d_it(directories);
    QStringListIterator f_it(files);

    while (d_it.hasNext())
    {
        QString name = d_it.next();
        if (!name.startsWith("."))
        {
            directory->cd(name);
            QString actualName = directory->path();
            NPFile *item = new NPFile(parent, actualName, true);
            watcher->addPath(actualName);//directory->canonicalPath());
            QStringList liiiiist = watcher->directories();
            item->setText(0,name);
            buildListOpenDirectory(directory, item);
            directory->cdUp();
        }
    }
    QString basePath = directory->path();//  ->canonicalPath();
    while (f_it.hasNext())
    {
        QString name = basePath;
        name.append(FSEP);
        QString shortName = f_it.next();
        name.append(shortName);
        QFile file(name);
        NPFile *item = new NPFile(parent, name);
        item->setText(0,shortName);
        item->setText(1, QString::number(file.size()));
        item->setText(2, this->getModifiedDate(&file));
    }
}

QString DirectoryTreeLister::getModifiedDate(QFile *file)
{
    QFileInfo fi(file->fileName());
    QDateTime qdt = fi.lastModified();
    return qdt.toString();
}
