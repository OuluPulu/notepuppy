#include "onesmallwindow.h"
#include "ui_onesmallwindow.h"

#include "filezapper.h"

#include <QPlainTextEdit>

OneSmallWindow::OneSmallWindow(QWidget *parent) :
    SmallDeviceWindow(parent),
    ui(new Ui::OneSmallWindow)
{
    ui->setupUi(this);

    setWindowTitle(tr("NotePuppeh"));
    settings = new NPSettings(this);
    inp = new InternalNotePuppy(ui->treeWidget, this);
    connect(inp, SIGNAL(editorReadyToOpen(HillStreetEditor*)), this, SLOT(openAnEditor(HillStreetEditor*)));

    buildList();
}

OneSmallWindow::~OneSmallWindow()
{
    delete ui;
}

void OneSmallWindow::buildList()
{
    inp->buildList();
}

void OneSmallWindow::createAndOpenFile(QString fname)
{
    inp->createAndOpenFile(fname);
}

void OneSmallWindow::openAnEditor(HillStreetEditor *editor)
{
    // Open the editor and shove it into a tab
    ui->tabWidget->addTab(editor, editor->windowTitle());//  file->getShortFileName());
    // Set that tab to be right at the front
    int howMany = ui->tabWidget->count();
    howMany--;
    ui->tabWidget->setCurrentIndex(howMany);
}

void OneSmallWindow::closeWidget(QWidget *widge)
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

void OneSmallWindow::openFile(NPFile *file)
{
    inp->openFile(file);
}

void OneSmallWindow::openFile()
{

}

void OneSmallWindow::plusButtonPressed()
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

void OneSmallWindow::minusButtonPressed()
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

void OneSmallWindow::undoButtonPressed()
{
    HillStreetEditor *hse = getEditor();
    if (hse)
    {
        hse->getPlainText()->undo();
    }
}

void OneSmallWindow::redoButtonPressed()
{
    HillStreetEditor *hse = getEditor();
    if (hse) hse->getPlainText()->redo();
}

void OneSmallWindow::closeButtonPressed()
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

HillStreetEditor *OneSmallWindow::getEditor(int which)
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

void OneSmallWindow::clearSelection()
{
    inp->clearSelection();
}
