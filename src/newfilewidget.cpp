#include "newfilewidget.h"
#include "ui_newfilewidget.h"

NewFileWidget::NewFileWidget(QDir *rootDirectory, QWidget *parent, QString caption, QString filename, bool dir) :
    QWidget(parent),
    ui(new Ui::NewFileWidget)
{
    ui->setupUi(this);
    this->setWindowTitle(caption);
    this->dtl = new DirectoryTreeLister(ui->treeWidget, DirectoryTreeLister::NewFile);
    dtl->buildList(rootDirectory);
    connect(dtl, SIGNAL(itemClicked(NPFile*)), this, SLOT(itemClicked(NPFile*)));
    item = 0x0;
    this->rootDir = rootDirectory;
    this->ui->fileLine->setFocus();
    this->ui->fileLine->setText(filename);
    this->ui->fileLine->selectAll();
    if (dir) ui->checkBox->setChecked(true);
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(okBP()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(canBP()));
}

NewFileWidget::~NewFileWidget()
{
    delete ui;
    delete dtl;
}

void NewFileWidget::itemClicked(NPFile *item)
{
    this->item = item;
}

bool NewFileWidget::isDirectory()
{
    return ui->checkBox->isChecked();
}

void NewFileWidget::okBP()
{
    QString fna = this->getFile();
    bool drc = ui->checkBox->isChecked();
    if (ui->fileLine->text().length() < 1)
    { // it is a directory.
        drc = true;
    }
    emit okButtonPressed(fna, drc);
}

void NewFileWidget::canBP()
{
    emit cancelButtonPressed();
}

QString NewFileWidget::getFile()
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

void NewFileWidget::clearFileLine()
{
    ui->fileLine->setText("");
}

bool NewFileWidget::hasRecognisedEnding(QString *file)
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

void NewFileWidget::makeLegalName(QString *file)
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

NPFile *NewFileWidget::getSelected()
{
    return item;
}

QString NewFileWidget::getDirectory()
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
