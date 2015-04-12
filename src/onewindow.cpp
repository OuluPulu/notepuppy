#include "onewindow.h"
#include "ui_onewindow.h"

#include "filezapper.h"

#include <QPlainTextEdit>

OneWindow::OneWindow(QWidget *parent) :
    SmallDeviceWindow(parent),
    ui(new Ui::OneWindow)
{
    ui->setupUi(this);

    setWindowTitle(tr("NotePuppeh"));
    settings = new NPSettings(this);
    inp = new InternalNotePuppy(ui->treeWidget, this);
    connect(inp, SIGNAL(editorReadyToOpen(HillStreetEditor*)), this, SLOT(openAnEditor(HillStreetEditor*)));
    connect(ui->plusButton, SIGNAL(clicked()), this, SLOT(plusButtonPressed()));
    connect(ui->minusButton, SIGNAL(clicked()), this, SLOT(minusButtonPressed()));
    connect(ui->undoButton, SIGNAL(clicked()), this, SLOT(undoButtonPressed()));
    connect(ui->redoButton, SIGNAL(clicked()), this, SLOT(redoButtonPressed()));
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(closeButtonPressed()));

    buildList();
}

OneWindow::~OneWindow()
{
    delete ui;
}

void OneWindow::buildList()
{
    inp->buildList();
}

void OneWindow::createAndOpenFile(QString fname)
{
    inp->createAndOpenFile(fname);
}

void OneWindow::openAnEditor(HillStreetEditor *editor)
{
    // Open the editor and shove it into a tab
    ui->tabWidget->addTab(editor, editor->windowTitle());//  file->getShortFileName());
    // Set that tab to be right at the front
    int howMany = ui->tabWidget->count();
    howMany--;
    ui->tabWidget->setCurrentIndex(howMany);
}

void OneWindow::closeWidget(QWidget *widge)
{
    int total = ui->tabWidget->count();
    for (int i = 0; i < total; ++i)
    {
        QWidget *woodge = ui->tabWidget->widget(i);
        if (woodge == widge)
        {
            ui->tabWidget->removeTab(i);
            woodge->close();
            delete woodge;
        }
    }
}

void OneWindow::openFile(NPFile *file)
{
    inp->openFile(file);
}


void OneWindow::plusButtonPressed()
{
    // Work out which directory to use
    // Check focus, then ask the relevant tab :)
    int whoHasFocus = ui->tabWidget->currentIndex();
    NPFile *currentSelection(0);
    if (whoHasFocus)
    {
        QWidget *widge = ui->tabWidget->widget(whoHasFocus);
        HillStreetEditor *editor = qobject_cast<HillStreetEditor*>(widge);
        if (editor)
            currentSelection = editor->getNPFile();
    }
    else
    {
        currentSelection = inp->dtl()->getCurrentSelection();
    }
    QDir *directory(0);
    if (currentSelection)
    {
        directory = currentSelection->getDirectory();
    }
    else
    {
        directory = settings->GetRootFolder();
    }
    // create a new FileZapper
    FileZapper *zp = new FileZapper(directory, this);
    ui->tabWidget->addTab(zp, tr("New file"));
    // Set that tab to be right at the front
    int howMany = ui->tabWidget->count();
    howMany--;
    ui->tabWidget->setCurrentIndex(howMany);

}

void OneWindow::minusButtonPressed()
{
    // If focus is 0, then do nothing, otherwise close the tab
    int whoHasFocus = ui->tabWidget->currentIndex();
    if (whoHasFocus)
    {
        //
        QWidget *widge = ui->tabWidget->widget(whoHasFocus);
        HillStreetEditor *editor = qobject_cast<HillStreetEditor*>(widge);
        if (editor)
        {
            editor->close();
        }
        else
        {
            FileZapper *frank = qobject_cast<FileZapper*>(widge);
            if (frank)
            {
                frank->close();
            }
            // else someone has implemented something and not added it here
        }
    }
    else
    {
        // get the current selection and delete it
        inp->deleteFile();
    }
}

/*
void OneWindow::closeFile(QString filename)
{

}

void OneWindow::importFile()
{

}

void OneWindow::exportFile()
{

}

void OneWindow::exportThisFile(QString file)
{

}

void OneWindow::renameFile()
{

}

void OneWindow::newFile()
{

}

void OneWindow::quickFile()
{

}

void OneWindow::deleteFile()
{

}

void OneWindow::clearSelection()
{

}
void OneWindow::setEditorActive(HillStreetEditor *active)
{

}

void OneWindow::openFile()
{

}
*/

void OneWindow::undoButtonPressed()
{
    HillStreetEditor *hse = getEditor();
    if (hse)
    {
        hse->getPlainText()->undo();
    }
}

void OneWindow::redoButtonPressed()
{
    HillStreetEditor *hse = getEditor();
    if (hse) hse->getPlainText()->redo();
}

void OneWindow::closeButtonPressed()
{
    int count = ui->tabWidget->count();
    for (;count > 0; --count)
    {
        HillStreetEditor *hse = getEditor(count);
        if (hse)
        {
            hse->close();
        }
    }
    QCoreApplication::exit();
}

HillStreetEditor *OneWindow::getEditor(int which)
{
    if (which < 0 || which > ui->tabWidget->count())
    {
        which = ui->tabWidget->currentIndex();
    }
    HillStreetEditor *editor = 0x0;
    if (which)
    {
        QWidget *widge = ui->tabWidget->widget(which);
        editor = qobject_cast<HillStreetEditor*>(widge);
    }
    return editor;
}
