#include "npfile.h"
#include <QDir>
#include <QTextStream>
#include <QDateTime>

NPFile::NPFile(NPFile *parent, QString filename, bool isDirectory) :
    QTreeWidgetItem(parent)
{
    if (NULL != parent) {
        this->parentDir = parent;
        this->parentDir->incrementFileCount();
        this->parentDir->addLabels();
    }
    init(filename, isDirectory);
}

NPFile::NPFile(QString filename, bool isDirectory)
{
    init(filename, isDirectory);
    this->parentDir = NULL;
}

NPFile::~NPFile()
{
    if (NULL != this->parentDir) {
        parentDir->decrementFileCount();
        parentDir->addLabels();
    }
}

void NPFile::init(QString filename, bool isDirectory)
{
    actualFile = QString(filename);
    izDirectory = isDirectory;
    fileCount = 0;
    if (izDirectory)
    {
        actualActualFile = 0x0;
        actualActualDir = new QDir(actualFile);
    }
    else
    {
        actualActualFile = new QFile(actualFile);
        QString path = actualFile.section(FSEP, 0, -2);
        actualActualDir = new QDir(path);
    }
    addLabels();
}

void NPFile::configureDir()
{

}

void NPFile::configureFile()
{

}

void NPFile::addLabels()
{
    if (!this->izDirectory)
    {
        actualActualFile = new QFile(actualFile);
    }
    setText(0, getShortFileName());

    if (!izDirectory)
    {
        QString notActualFile(actualFile);
        QFile file(notActualFile);
        QFileInfo info(file);
        setText(1, QString::number(file.size()));
        setText(2, info.lastModified().toString());
    }

    else
    {
        QVariant midvar = this->getFileCount();
        QString midlab = midvar.toString();
        midlab.append(" file");
        if (midvar.toInt() != 1) {
            midlab.append("s");
        }
        setText(1, midlab);
    }
}

bool NPFile::isFile()
{
    return !izDirectory;
}

bool NPFile::isDirectory()
{
    return izDirectory;
}

QString NPFile::getFileName()
{
    return QString(actualFile);
}

QString NPFile::getShortFileName()
{
    QString r = actualFile.section(FSEP,-1);
    return r;
}

QDir *NPFile::getDirectory()
{
    return actualActualDir;
}

QString NPFile::getFilePath()
{
    QString r = actualActualDir->absolutePath();
    return r;
}

void NPFile::save(QString data)
{
    if (!izDirectory)
    {
        /* TODO - fix the Vim incompatibility which is right here. */
        actualActualFile->open(QIODevice::WriteOnly);
        QTextStream stream(actualActualFile);
        stream << data;
        actualActualFile->flush();
        actualActualFile->close();

        QFileInfo info(actualFile);
        this->setText(1, QString::number(actualActualFile->size()));
        setText(2, info.lastModified().toString());
    }
}

void NPFile::renameFile(QString newName)
{

}

void NPFile::incrementFileCount()
{
    this->fileCount++;
}

void NPFile::decrementFileCount()
{
    this->fileCount--;
}

int NPFile::getFileCount()
{
    return this->fileCount;
}

