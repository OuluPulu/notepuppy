#include "filezapper.h"
#include "ui_filezapper.h"

#include "directorytreelister.h"
#include "hillstreeteditor.h"
#include "newfilewidget.h"

#include <QDebug>

FileZapper::FileZapper(QDir *rootDir, SmallDeviceWindow *parent) :
    QDialog(parent)
  , ui(new Ui::FileZapper)
  , m_parent(parent)
{
    ui->setupUi(this);

    QGridLayout *layout = new QGridLayout(ui->widget);
    NewFileWidget *nfi = new NewFileWidget(rootDir, ui->widget, tr("New File"));
    m_widget = nfi;
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(nfi);

    connect(nfi, SIGNAL(okButtonPressed(QString,bool)), this, SLOT(okButtonZapped(QString,bool)));
    connect(nfi, SIGNAL(cancelButtonPressed()), this, SLOT(cancelButtonZapped()));

}

FileZapper::~FileZapper()
{
    delete ui;
}

void FileZapper::okButtonZapped(QString filename, bool isDirectory)
{
    // need to create this here file
    // FIXME - duplicate behaviour, the INP does this as well.
    // This could be better to have it all in a utilities unit,
    // or put it into the DTL even?
    // perhaps a bit wanky to cast the widget for a button press
    NewFileWidget *nfi = qobject_cast<NewFileWidget*>(m_widget);
    //DirectoryTreeLister *dtl = nfi->directoryTreeLister();

    if (isDirectory)
    {
        QDir dir(filename);
        dir.mkpath(filename);
        nfi->clearFileLine();
        // as we are creating a directory, that's enough - we can just leave it.
        // this should appear straight away in the tree. :D
    }
    else
    {
        m_parent->createAndOpenFile(filename);
        m_parent->closeWidget(this);
    }
}

void FileZapper::close()
{
    // TODO this method needs to be done :)
}

NPFile *FileZapper::createNewFile(QString filename)
{
    //check if the file already exists, then
    NPFile *npf(0);
    if (QFile::exists(filename))
    {
        // open the existing file
        NewFileWidget *nfi = qobject_cast<NewFileWidget*>(m_widget);
        npf = nfi->directoryTreeLister()->addToTree(filename);
    }
    else
    {
        QString str = filename.section(FSEP,0,-2);
        QDir dr(str);
        if (!dr.exists())
        {
            dr.mkpath(str);
        }

        QFile *fill = new QFile(filename);
        fill->open(QIODevice::WriteOnly);
        QTextStream stream(fill);
        QString content("");
        stream << content;
        fill->flush();
        fill->close();
        delete fill;

        NewFileWidget *nfi = qobject_cast<NewFileWidget*>(m_widget);
        npf = nfi->directoryTreeLister()->addToTree(filename);

    }
    return npf;
}

void FileZapper::cancelButtonZapped()
{
    m_parent->closeWidget(this);
}
