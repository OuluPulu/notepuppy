#include "newfiledialog.h"
#include "ui_newfiledialog.h"

#include <QAction>
#include <QDate>

NewFileDialog::NewFileDialog(QDir *rootDirectory, QWidget *parent, QString caption, QString filename, bool dir) :
    QDialog(parent),
    ui(new Ui::NewFileDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(caption);
    this->dtl = new DirectoryTreeLister(ui->treeWidget, DirectoryTreeLister::NewFile);
    dtl->buildList(rootDirectory);
    connect(dtl, SIGNAL(itemClicked(NPFile*)), this, SLOT(itemClicked(NPFile*)));
    connect(ui->buttonDate, SIGNAL(clicked(bool)), this, SLOT(insertDate()));
    item = 0x0;
    this->rootDir = rootDirectory;
    this->ui->fileLine->setFocus();
    this->ui->fileLine->setText(filename);
    this->ui->fileLine->selectAll();
    if (dir) ui->checkBox->setChecked(true);

    QAction *dateMe = new QAction(this);
    dateMe->setShortcut(QKeySequence("Ctrl+D"));
    connect(dateMe, SIGNAL(triggered(bool)), this, SLOT(insertDate()));
    this->addAction(dateMe);
}

NewFileDialog::~NewFileDialog()
{
    delete ui;
    delete dtl;
}

void NewFileDialog::itemClicked(NPFile *item)
{
    this->item = item;
}

void NewFileDialog::insertDate()
{
    QDate d = QDate::currentDate();
    QString date = d.toString("yyyy-MM-dd");
    QString buffer = ui->fileLine->text();
    buffer += date;
    ui->fileLine->setText(buffer);
}

bool NewFileDialog::isDirectory()
{
    return ui->checkBox->isChecked();
}

QString NewFileDialog::getFile()
{
    //get the selected directory
    QString fname = this->getDirectory();

    //get the file and process it
    QString file = ui->fileLine->text();

    //does the file name start and end with "s?
    if (file.size() > 2)
    {
        if (file.startsWith(DUB_QUOTES) && file.endsWith(DUB_QUOTES))
        {
            QString tmp = file.remove(0,1);
            int l = tmp.size();
            l--;
            if (l > 0)
            {
                file = tmp.remove(l,1);
            }
            else file = tmp; /* There is something wrong here... */
        }
        else if (!this->hasRecognisedEnding(&file))
        {
            file.append(DOT_TXT);
        }
        makeLegalName(&file);
        fname.append(FSEP);
        fname.append(file);
    }


    return fname;
}

bool NewFileDialog::hasRecognisedEnding(QString *file)
{
    bool known = false;
    if (ui->checkBox->isChecked())
    {
        //it is a directory
        known = true;
    }
    else if (file->contains("."))
    {
        int l = file->size();
        int x = file->lastIndexOf(".");
        if (7 > l-x)
        {
            //we support up to five letters at the end of a filename.
            known = true;
        }
    }
    return known;
}

void NewFileDialog::makeLegalName(QString *file)
{
    file->replace('?',REP_CHAR);
    file->replace('"',REP_CHAR);
    file->replace('<',REP_CHAR);
    file->replace('>',REP_CHAR);
    file->replace('\\',REP_CHAR);
    file->replace(':',REP_CHAR);
    file->replace('*',REP_CHAR);
    file->replace('|',REP_CHAR);
}

NPFile *NewFileDialog::getSelected()
{
    return item;
}

QString NewFileDialog::getDirectory()
{
    QString dir = "";
    if (0x0 != item && item->isDirectory())
    {
        dir.append(item->getFileName());
    }
    else if (0x0 != item)
    {
        dir.append(item->getFilePath());
    }
    else
    {
        dir.append(this->rootDir->absolutePath());
    }
    return dir;
}
