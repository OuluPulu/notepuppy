#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "newfiledialog.h"
#include <QFile>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QListIterator>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
  , ui(new Ui::MainWindow)
  , translucent(false)
{
    //build the basic UI
    ui->setupUi(this);


    // These buttons represent the glorious Maemo heritage of NotePuppy. They will remain,
    // but as NP is all Mac these days (yes it is, from 0,6,2 onwards, don't try to deny
    // it) they are hidden. Gone but not forgotten.
    ui->buttonWdiget->setVisible(false);

    inp = new InternalNotePuppy(ui->treeWidget, this);

    connect(inp, SIGNAL(editorReadyToOpen(HillStreetEditor*)), this, SLOT(openAnEditor(HillStreetEditor*)));

    inp->buildList();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::buttonsOnOff(NPFile *item)
{
    bool selected = (0x0 != item);
    ui->buttonDelete->setEnabled(selected);
    ui->buttonRename->setEnabled(selected);

    selected = (selected && !(item->isDirectory()));
    ui->buttonExport->setEnabled(selected);
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void MainWindow::showHideButtons(bool show)
{
    ui->buttonWdiget->setVisible(show);
}

void MainWindow::setTranslucent(bool translucent)
{
    qreal level = 1.0;
    if (translucent) level = 0.60;
    this->setWindowOpacity(level);
}



void MainWindow::openFile(NPFile *file)
{
    inp->openFile(file);
}

void MainWindow::openFile()
{
    inp->openFile();
}

void MainWindow::openAnEditor(HillStreetEditor *editor)
{
    editor->show();
}

void MainWindow::closeFile(QString filename)
{
    inp->closeFile(filename);
}

void MainWindow::importFile()
{
    inp->importFile();
}

void MainWindow::exportFile()
{
    inp->exportFile();
}

void MainWindow::exportThisFile(QString file)
{
    inp->exportThisFile(file);
}

void MainWindow::renameFile()
{
    inp->renameFile();
}

void MainWindow::newFile()
{
    inp->newFile();
}

void MainWindow::quickFile()
{
    inp->quickFile();
}

void MainWindow::deleteFile()
{
    inp->deleteFile();
}

void MainWindow::buildList()
{
    inp->buildList();
}

void MainWindow::clearSelection()
{
    inp->clearSelection();
}

void MainWindow::setEditorActive(HillStreetEditor *active)
{
    inp->setEditorActive(active);
}

// The PowerTools
void MainWindow::openTopAndTail()
{
    inp->openTopAndTail();
}

void MainWindow::openFindAndReplace()
{
    inp->openFindAndReplace();
}

void MainWindow::topAndTailClosed()
{
    inp->topAndTailClosed();
}

void MainWindow::findAndReplaceClosed()
{
    inp->findAndReplaceClosed();
}


 void MainWindow::contextMenuEvent(QContextMenuEvent *event)
 {
     event->accept();

     QMenu men(this);
    men.addAction(tr("New file"), this, SLOT(newFile()));
    men.addAction(tr("Import File"), this, SLOT(importFile()));

    NPFile *selected = inp->dtl()->getCurrentSelection();
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

