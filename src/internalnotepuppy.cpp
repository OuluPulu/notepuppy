#include "internalnotepuppy.h"
#include "newfiledialog.h"

#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>
#include <QTextStream>

#include <QDebug>

InternalNotePuppy::InternalNotePuppy(QTreeWidget *treeWidget, QWidget *parent) :
    QObject(parent)
  , parentWidget(parent)
  , m_treeWidget(treeWidget)
//  , findAndReplace(0)
//  , topAndTail(0)
{
    settings = new NPSettings(this);
    m_dtl = new DirectoryTreeLister(m_treeWidget, DirectoryTreeLister::MainWindow);
    connect(m_dtl, SIGNAL(itemActivated(NPFile*)), this, SLOT(openFile(NPFile*)));
}

void InternalNotePuppy::buildList()
{
    static bool built = false;
    if (!built)
    {
        m_dtl->buildList(settings->GetRootFolder());
        built = true;
    }
}



void InternalNotePuppy::clearSelection()
{
    m_dtl->clearSelected();
}

/* should be called fileClosed, because it's a reaction not an instruction. */
void InternalNotePuppy::closeFile(QString filename)
{
    HillStreetEditor *ed = kids.value(filename);
    if (ed == this->activeEditor)
    {
        this->activeEditor = 0x0;
        emit editorClosed(ed);
    }
    kids.remove(filename);
    delete ed;
    ed = NULL;
}

 void InternalNotePuppy::contextMenuEvent(QContextMenuEvent *event)
 {
     event->accept();

     QMenu men(this->parentWidget);
    men.addAction(tr("New file"), this, SLOT(newFile()));
    men.addAction(tr("Import File"), this, SLOT(importFile()));

    NPFile *selected = m_dtl->getCurrentSelection();
    if (0x0 != selected)
    {
        QString filename = selected->getShortFileName();
        if (selected->isFile())
        {
            men.addSeparator();
            QString text = tr("Open \"");
            text.append(filename);
            text.append("\"");
            men.addAction(text, this, SLOT(openFile()));
            QString moreText = tr("Export \"");
            moreText.append(selected->getShortFileName());
            moreText.append("\"");
            men.addAction(moreText, this, SLOT(exportFile()));
        }

        men.addSeparator();

        QString text = tr("Rename \"");
         QString moreText = tr("Delete \"");
         text.append(filename);
         moreText.append(filename);
         text.append("\"");
         moreText.append("\"");
        men.addAction(text, this, SLOT(renameFile()));
        men.addAction(moreText, this, SLOT(deleteFile()));
    }

    men.exec(event->globalPos());
 }

bool InternalNotePuppy::copyDirectory(QDir original, QString destination, bool overwrite)
{
    bool journey = true;

    //first step, copy the directory

    if (!settings->GetRootFolder()->exists(destination))
    {
        if (!original.mkpath(destination)) return false;
    }

    QStringList directories = original.entryList(QDir::AllDirs);
    QStringList files = original.entryList(QDir::Files);
    QStringListIterator d_it(directories);
    QStringListIterator f_it(files);


    //second step, copy the children files

    while (f_it.hasNext())
    {
        QString next = f_it.next();
        if (!next.startsWith("."))
        {
            QString f_orig = original.absolutePath();
            f_orig.append(FSEP);
            f_orig.append(next);
            QString f_new(destination);
            f_new.append(FSEP);
            f_new.append(next);
            QFile *f = new QFile(f_orig);
            if (!copyFile(f,f_new, overwrite))
            {
                journey = false;
            }
            delete f;
        }
    }

    //third step, copy the children directories

    while (d_it.hasNext())
    {
        QString next = d_it.next();
        if (!next.startsWith("."))
        {
            QString d_orig = original.absolutePath();
            d_orig.append(FSEP);
            d_orig.append(next);
            QString d_new(destination);
            d_new.append(FSEP);
            d_new.append(next);
            QDir d(d_orig);
            if (!copyDirectory(d, d_new, overwrite))
            {
                journey = false;
            }
        }
    }
    return journey;
}

bool InternalNotePuppy::copyFile(QFile *original, QString destination, bool force)
{
    if (QFile::exists(destination) && force)
    {
        QFile::remove(destination);
    }
    return original->copy(destination);
}

void InternalNotePuppy::deleteFile()
{
    NPFile *delFile = m_dtl->getCurrentSelection();
    if (NULL == delFile) return;
    QString fileToGo = delFile->getFileName();
    if (!(QFile::exists(fileToGo))) return;
    bool doDelete = true;
    if (delFile->isDirectory())
    {
        //check for children.
        if (this->directoryHasChildren(fileToGo))
        {
            QString msg(fileToGo);
            msg.append(tr(" - really delete this directory??? It has children!"));
            int result = QMessageBox::question(this->parentWidget, NOTEYFICATION, msg, QMessageBox::Ok|QMessageBox::Cancel);
            if (QMessageBox::Ok == result)
            {
                QStringList open = openChildren(fileToGo);
                if (open.length() > 0)
                {
                    QString message(tr("Can't be done I'm afraid. You have to close:"));
                    QStringListIterator it(open);
                    while (it.hasNext())
                    {
                        message.append(it.next());
                        message.append("\n");
                    }
                    message.append(tr("Come back when you have done that."));
                    QMessageBox::information(this->parentWidget, NOTEYFICATION, message);
                    doDelete = false;
                }
            }
            else doDelete = false;
        }
    }
    else if (kids.contains(fileToGo))
    {
        QString msg("Can't delete ");
        msg.append(fileToGo);
        msg.append(", it's open.");
        QMessageBox::information(this->parentWidget, NOTEYFICATION, msg);
        doDelete = false;
    }
    else
    {
        QString msg("Delete ");
        msg.append(fileToGo);
        msg.append("?");
        if (!(QMessageBox::Ok == QMessageBox::question(this->parentWidget, NOTEYFICATION, msg, QMessageBox::Ok|QMessageBox::Cancel)))
        {
            doDelete = false;
        }
    }

    if (doDelete)
    {
        bool worked = false;
        QDir *root = settings->GetRootFolder();
        QString newName = settings->GetTrashFolder();

//        newName += fileToGo;
        if (delFile->isDirectory())
        {
            QString rootdir = root->absolutePath();
            QString delFull = delFile->getFilePath();
            QString folderaddress = delFull.section(rootdir,1);
            newName+=folderaddress;

            QDir drec(fileToGo);
            QString nnchk = newName;
            int nnnum = 0;
            while (root->exists(nnchk))
            {
                nnnum++;
                nnchk = newName;
                nnchk.append(" (");
                nnchk.append(QString::number(nnnum));
                nnchk.append(")");
            }
            newName = nnchk;
            if (this->copyDirectory(drec, newName, true))
            {
                worked = this->deleteFolder(drec);
            }

        }
        else
        {
            newName+=FSEP;
            newName+=delFile->getShortFileName();
            QFile file(fileToGo);

            QString nnchk = newName;
            int nnnum = 0;
            while (QFile::exists(nnchk))
            {
                nnnum++;
                nnchk = newName;
                nnchk.append(" (");
                nnchk.append(QString::number(nnnum));
                nnchk.append(")");
            }
            newName = nnchk;

// This is the delete section. In Windows, the delete process is different. I have
// nfi how to do it properly in the doze, so for that it just moves to a different
// address.
            // FIXME Preprocessor ridiculousness

#ifdef Q_WS_WINDOWS
            QString path = newName.section(FSEP, 0, -2);
            bool deld = root->mkpath(path);
            worked = file.copy(fileToGo, newName);
            if (worked) worked = file.remove(fileToGo);
#elif defined(Q_OS_SYMBIAN)
            QString path = newName.section(FSEP, 0, -2);
            bool deld = root->mkpath(path);
            worked = file.copy(fileToGo, newName);
            if (worked) worked = file.remove(fileToGo);
#elif defined(Q_OS_ANDROID)
            QString path = newName.section(FSEP, 0, -2);
            bool deld = root->mkpath(path);
            worked = file.copy(fileToGo, newName);
            if (worked) worked = file.remove(fileToGo);
#else
            worked = file.rename(newName);
#endif /* Q_WS_WINDOWS */
        }
        if (!worked)
        {
            QString msg = tr("Oho. Unable to delete ");
            msg.append(fileToGo);
            msg.append(tr(". :("));
            QMessageBox::information(this->parentWidget, NOTEYFICATION, msg, QMessageBox::Ok);
        }
    }
    clearSelection();
}

bool InternalNotePuppy::deleteFolder(QDir dir)
{
    bool delled = true;

    QFileInfoList info_l = dir.entryInfoList();
    QFileInfoList hidden_l = dir.entryInfoList(QDir::Hidden);
    info_l.append(hidden_l);
    QListIterator<QFileInfo> it(info_l);

    while (it.hasNext())
    {
        QFileInfo info = it.next();
        QString dotses = info.fileName();
        if ("." == dotses || ".." == dotses) {
            //These are the unix short cuts. Don't work on them.
        }
        else if (info.isDir())
        {
            QDir hDir(info.canonicalFilePath());
            delled = this->deleteFolder(hDir);
        }
        else if (info.isFile())
        {
            QString trailer = settings->GetTrashFolder();
            QString park = info.absolutePath();
            trailer+=park;
            QFile filer(park);
            delled = filer.rename(park, trailer);
            delled = QFile::remove(info.canonicalFilePath());
        }
    }
    delled = settings->GetRootFolder()->rmdir(dir.canonicalPath());
    return delled;
}

bool InternalNotePuppy::directoryHasChildren(QString direc)
{
    QDir dir(direc);
    QStringList children = dir.entryList();
    QStringList lapset;

    QStringListIterator it(children);
    while (it.hasNext())
    {
        QString next = it.next();
        if (!(next=="." || next==".."))
        {
            /* this may be a problem - it will complain about invisible files,
            but the end-user won't see them. Is it worth fixing? */
            lapset.append(next);
        }
    }
    return (lapset.size() != 0);
}

void InternalNotePuppy::exportFile()
{
    NPFile *theOne = m_dtl->getCurrentSelection();
    exportThisFile(theOne);
}

void InternalNotePuppy::exportThisFile(QString file)
{
    NPFile *faiel = m_dtl->addToTree(file);
    this->exportThisFile(faiel);
}


void InternalNotePuppy::exportThisFile(NPFile *theOne)
{

    if (NULL != theOne && theOne->isFile())
    {

        QString shortFileName = theOne->getShortFileName();
        QString potFileName = QDir::homePath();
        potFileName.append(FSEP);
        potFileName.append(shortFileName);

        QString newlocation = QFileDialog::getSaveFileName(this->parentWidget, tr("Where d'ya want it, love?"), potFileName);
        {
            if (QFile::exists(newlocation))
            {
                QFile::remove(newlocation);
            }
            QFile::copy(theOne->getFileName(), newlocation);
        }
    }
}

void InternalNotePuppy::findAndReplaceClosed()
{
//    delete(findAndReplace);
//    findAndReplace = 0x0;
}

void InternalNotePuppy::importFile()
{
    //open a file browse directory
    QString filename = QFileDialog::getOpenFileName(this->parentWidget, tr("Oh, OK. Which file?"), QDir::homePath());
    if (!filename.isEmpty())
    {
        QFileInfo file(filename);
        if (file.exists())
        {
            //choose where to put the file
            QString message(NOTEYFICATION);
            message.append(tr(" - where is that going?"));

            NPFile *selection = m_dtl->getCurrentSelection();
            QString rootFolder("");
            if (0x0 == selection)
            {
                rootFolder.append(settings->GetRootFolder()->absolutePath());
            }
            else
            {
                rootFolder.append(selection->getFilePath());
            }
            NewFileDialog nfd(new QDir(rootFolder), this->parentWidget, message, file.fileName());

            //open a dilaog boks, and pick a folder.
            int result = nfd.exec();

            if (QDialog::Accepted == result)
            {
                //is there a file with that name already?
                QString file_str = nfd.getFile();
                QString dir_str = nfd.getDirectory();

                NPFile *directory = nfd.getSelected();
                if (NULL != directory && directory->isFile())
                {
                    directory = (NPFile *)directory->parent();
                }

                QString target("");

                if (0 == file_str.size())
                {
                    target.append(dir_str);
                    target.append(FSEP);
                    target.append(filename.section(FSEP,-1));
                }
                else
                {
                    target.append(file_str);
                }
                QFile target_file(target);

                bool OK2COPY = true;
                if (target_file.exists())
                {
                    QString message("There is already a file there!! Overwrite it?");
                    int result = QMessageBox::question(this->parentWidget, NOTEYFICATION, message, QMessageBox::Ok|QMessageBox::Cancel);
                    OK2COPY = (QMessageBox::Ok == result);
                }
                if (OK2COPY)
                {
                    QFile foil(filename);
                    this->copyFile(&foil, target, OK2COPY);
//                    m_dtl->addToTree(target);
                }
            }
        }
        else
        {
            QString message = filename;
            message.append(" not found. Sorry dude.");
            QMessageBox::information(this->parentWidget, NOTEYFICATION, message);
        }
    }
}

void InternalNotePuppy::interOpenFile(NPFile *file)
{
    QString filename = file->getFileName();
    if (kids.contains(filename))
    {
#ifndef ONE_SCREEN
        //works well on dtop, not well on smaller screens
        HillStreetEditor *edie = kids.value(filename);
        connect(edie, SIGNAL(newFileSignal()), this, SLOT(newFile()));
        connect(edie, SIGNAL(exportThisFilePlease(QString)), this, SLOT(exportThisFile(QString)));
        edie->activateWindow();
        /* hacky way of bringing the editor window to the front, there is probaly a better way.... */
        edie->setVisible(false);
        edie->setVisible(true);
#endif /* 1_SCREEN */
    }
    else
    {
        HillStreetEditor *ed = new HillStreetEditor(file, settings);
        connect(ed, SIGNAL(closed(QString)), this, SLOT(closeFile(QString)));
        connect(ed, SIGNAL(newFileSignal()), this, SLOT(newFile()));
        connect(ed, SIGNAL(exportThisFilePlease(QString)), this, SLOT(exportFile()));
        connect(ed, SIGNAL(createQuickFile()), this, SLOT(quickFile()));
        connect(ed, SIGNAL(editorActivated(HillStreetEditor*)), this, SLOT(setEditorActive(HillStreetEditor*)));
        /* Wire up the powertools */
        connect(ed, SIGNAL(openFindAndReplace()), this, SLOT(openFindAndReplace()));
        connect(ed, SIGNAL(openTopAndTailPlease()), this, SLOT(openTopAndTail()));
        // emit the notification
        emit editorReadyToOpen(ed);
        kids.insert(filename, ed);
    }
}

void InternalNotePuppy::newFile()
{
    //open a dialog box asking for a file name
    QString dialoy = NOTEYFICATION + tr(" - name for the new file, please");
    QDir *rootDir;
    NPFile *tem = m_dtl->getCurrentSelection();

    if (0x0 != tem)
    {
        if (tem->isDirectory())
        {
            rootDir = new QDir(tem->getFileName());
        }
        else
        {
            rootDir = new QDir(tem->getFilePath());
        }
    }
    else
    {
        rootDir = settings->GetRootFolder();
    }

    NewFileDialog nfd(rootDir, this->parentWidget, dialoy);
    int result = nfd.exec();

    if (QDialog::Accepted == result)
    {
        QString fname = nfd.getFile();
        bool isDir = nfd.isDirectory();

        if (isDir)
        {
            settings->GetRootFolder()->mkpath(fname);
//            m_dtl->addToTree(fname);
        }
        else
        {
            //check if the file already exists, then
            if (QFile::exists(fname))
            {
                //open a dialog box telling them NO.
                QString msg(tr("A file called "));
                msg.append(fname);
                msg.append(tr(" already exists, innit."));
                QMessageBox::information(this->parentWidget, NOTEYFICATION, msg);
            }
            else
            {
                createAndOpenFile(fname);
                /**
                QString str = fname.section(FSEP,0,-2);
                QDir dr(str);
                if (!dr.exists())
                {
                    dr.mkpath(str);
                }

                QFile *fill = new QFile(fname);
                fill->open(QIODevice::WriteOnly);
                QTextStream stream(fill);
                QString content("");
                stream << content;
                fill->flush();
                fill->close();
                delete fill;

                NPFile *newFile = m_dtl->addToTree(fname);

                if (NULL != newFile)
                {
                    interOpenFile(newFile);
                }
                */
            }
        }
    }
}

void InternalNotePuppy::createAndOpenFile(QString fname)
{
    QString str = fname.section(FSEP,0,-2);
    QDir dr(str);
    if (!dr.exists())
    {
        dr.mkpath(str);
    }

    QFile *fill = new QFile(fname);
    fill->open(QIODevice::WriteOnly);
    QTextStream stream(fill);
    QString content("");
    stream << content;
    fill->flush();
    fill->close();
    delete fill;

    NPFile *newFile = m_dtl->addToTree(fname);

    if (NULL != newFile)
    {
        interOpenFile(newFile);
    }
}

QStringList InternalNotePuppy::openChildren(QString direc)
{
    QDir dir(direc);
    QStringList allChildren = dir.entryList();
    QStringList openChildren;

    QStringListIterator it(allChildren);

    while (it.hasNext())
    {
        QString next = dir.absolutePath();
        next.append(FSEP);
        next.append(it.next());
        if (kids.contains(next))
        {
            openChildren.append(next);
        }
    }
    return openChildren;
}

void InternalNotePuppy::openFile(NPFile *file)
{
    if (!file->isDirectory())
    {
       interOpenFile(file);
    }
}

void InternalNotePuppy::openFile()
{
    NPFile *sel = m_dtl->getCurrentSelection();
    if (0x0 != sel)
    {
        openFile(sel);
    }
}

void InternalNotePuppy::openFindAndReplace()
{
    qWarning() << "This feature doesn't work in Qt 5, was never implemented properly, and is gone. For now.";
//    if (0 == findAndReplace)
//    {
//        findAndReplace = new FindReplace();
//        connect(findAndReplace, SIGNAL(closed()), this, SLOT(findAndReplaceClosed()));
//        connect(this, SIGNAL(editorChanged(HillStreetEditor*)), findAndReplace, SLOT(setEditor(HillStreetEditor*)));
//        connect(this, SIGNAL(editorClosed(HillStreetEditor*)), findAndReplace, SLOT(editorClosed(HillStreetEditor*)));
//        findAndReplace->show();
//    }
//    else
//    {
//        findAndReplace->setVisible(false);
//        findAndReplace->setVisible(true);
//    }
//    findAndReplace->setEditor(activeEditor);
}

void InternalNotePuppy::openTopAndTail()
{
    qWarning() << "This feature doesn't work in Qt 5, was never implemented properly, and is gone. For now.";
//    if (0 == topAndTail)
//    {
//        topAndTail = new TopAndTail();
//        connect(topAndTail, SIGNAL(closed()), this, SLOT(topAndTailClosed()));
//        connect(this, SIGNAL(editorChanged(HillStreetEditor*)), topAndTail, SLOT(setEditor(HillStreetEditor*)));
//        topAndTail->show();
//    }
//    else
//    {
//        topAndTail->setVisible(false);
//        topAndTail->setVisible(true);
//    }
//    topAndTail->setEditor(activeEditor);
}


void InternalNotePuppy::quickFile()
{
    // get the "quick file" directory
    QString qkfl = settings->GetQuickFolder();
    qkfl.append(FSEP);
    // get the next file name
    QDateTime now = QDateTime::currentDateTime();
    qkfl.append(now.toString("yyyyMMdd hh-mm-ss"));

    QString cntr = ".txt";

    for (int i = 0; QFile::exists(qkfl + cntr); ++i)
    {
        /* one feels that it is unlikely this code will be executed very often. */
        cntr = " (";
        QVariant eye = i;
        cntr.append(eye.toString());
        cntr.append(").txt");
    }
    // make it sew
    qkfl.append(cntr);

    QFile *fill = new QFile(qkfl);
    fill->open(QIODevice::WriteOnly);
    QTextStream stream(fill);
    QString content("");
    stream << content;
    fill->flush();
    fill->close();
    delete fill;

    NPFile *newFile = m_dtl->addToTree(qkfl);
    if (NULL != newFile)
    {
        this->interOpenFile(newFile);
    }
}

void InternalNotePuppy::setEditorActive(HillStreetEditor *active)
{
    this->activeEditor = active;
    emit editorChanged(active);
}

void InternalNotePuppy::setTranslucent(bool translucent)
{
    qreal level = 1.0;
    if (translucent) level = 0.60;
    this->parentWidget->setWindowOpacity(level);
}

void InternalNotePuppy::topAndTailClosed()
{
//    delete(topAndTail);
//    topAndTail = 0x0;
}

void InternalNotePuppy::renameFile()
{
    //get the file to do
    NPFile *filetomove = m_dtl->getCurrentSelection();
    QString filename = filetomove->getFileName();
    QFileInfo finfo(filename);

    bool dir = filetomove->isDirectory();
    NewFileDialog nfd(settings->GetRootFolder(),this->parentWidget, tr("And what will you call it?"), finfo.fileName(), dir);
    int result = nfd.exec();
    if (QDialog::Accepted == result)
    {
        QString newname = nfd.getFile();
        bool isDir = nfd.isDirectory();

        if (!newname.isEmpty())
        {
            bool changed = false;

            if (isDir && filetomove->isDirectory())
            {
                if (this->openChildren(filename).size() > 0)
                {
                    QString msg(tr("Can't rename "));
                    msg.append(filename);
                    msg.append(tr(" it has open children."));
                    QMessageBox::information(this->parentWidget, NOTEYFICATION, msg);
                }
                else
                {
                      QDir dir(filename);

                      bool worked = dir.rename(filename, newname);
                      if (!worked)
                      {
                          QMessageBox::information(this->parentWidget, NOTEYFICATION, tr("Didn't work. Check that there isn't a folder with the same (or similar) name."));
                      }
                      else changed = true;
                }
            }
            else if (!isDir && !filetomove->isDirectory())
            {
                if (kids.contains(filename))
                {
                    QString msg(tr("Can't rename "));
                    msg.append(filename);
                    msg.append(tr(" it's open."));
                    QMessageBox::information(this->parentWidget, NOTEYFICATION, msg);
                }
                else
                {
                    QFile file(filename);

                    bool worked = file.rename(filename, newname);

                    if (!worked)
                    {
                        QMessageBox::information(this->parentWidget, NOTEYFICATION, tr("Didn't work. Sorry."));
                    }
                    else changed = true;
                }
            }
            else
            {

                QMessageBox::information(this->parentWidget, NOTEYFICATION, "Illegal move :-(", QMessageBox::Ok);
            }
            if (changed)
            {
                delete (filetomove);
                clearSelection();
            }
        }
    }
}
