#include "hillstreeteditor.h"
#include "ui_hillstreeteditor.h"

#include <QTextStream>
#include <QDir>
#include <QMessageBox>
#include <QMutex>
#include <QPushButton>

// these numbers have no relevance at all.
#define EW_KEEP_THIS_FILE  4375
#define EW_RELOAD_THE_FILE 5427

HillStreetEditor::HillStreetEditor(NPFile *file, NPSettings *settings, QWidget *parent) :
        QMainWindow(parent)
    , ui(new Ui::HillStreetEditor)
    , m_isFullScreen(false)
{
    this->setFocusPolicy(Qt::StrongFocus);
    ui->setupUi(this);
    ui->plainTextEdit->setProperty("FingerScrollable", true);
    ui->plainTextEdit->setProperty("FingerScrollBars", true);
    this->file = file;
    this->settings = settings;
    QString shortFileName = file->getShortFileName();
    this->setWindowTitle(shortFileName);
    filename = file->getFileName();
    QFile *fill = new QFile(filename, this);

    ui->plainTextEdit->setTabStopWidth(settings->GetTabSizePx());
    // create the fonts
    usingFontOne = true;
    fontOne = QFont("American Typewriter");
    int chumpadoo = fontOne.pointSize();
    chumpadoo+=2;
    fontOne.setPointSize(chumpadoo);
    fontTwo = QFont("Helvetica Neue", fontOne.pointSize());  //ui->plainTextEdit->document()->defaultFont();

    ui->plainTextEdit->document()->setDefaultFont(fontOne);

    //check if the file is a real one
    if (!fill->exists())
    {
        //if not real then oh well

    }
    else
    {
        //if real then read it
        if (fill->open(QIODevice::ReadOnly))
        {
            QTextStream *str = new QTextStream(fill);
            QString all = str->readAll();
            ui->plainTextEdit->setPlainText(all);
            QFileInfo info(fill->fileName());
            lastModified = info.lastModified();
            outer = new TTOuter(this);
            jesus = new QAction(this);
            jesus->setShortcut(QKeySequence::Save);
            this->addAction(jesus);
            connect(outer, SIGNAL(saveNow()), this, SLOT(save()));
            connect(ui->plainTextEdit, SIGNAL(textChanged()), outer, SLOT(keyPressed()));
            connect(jesus, SIGNAL(triggered()), this, SLOT(save()));
            this->hookUpFileListener();
        }
        else
        {
            ui->plainTextEdit->setPlainText(tr("Can't open the file."));
        }
    }
    delete(fill);
}

HillStreetEditor::~HillStreetEditor()
{
}

bool HillStreetEditor::event(QEvent *event)
{
    QEvent::Type t = event->type();
    if (QEvent::WindowActivate == t)
    {
        event->accept();
        emit editorActivated(this);
        return true;
    }
    else if (QEvent::Close == t)
    {
        event->accept();
        outer->closeDown();
        emit closed(filename);
        return true;
    }
    return QMainWindow::event(event);
}


void HillStreetEditor::save()
{
    bool oksave = false;
    QFileInfo info(filename);//file->getFileName());

    if (lastModified == info.lastModified())
    {
        oksave = true;
    }
    else
    {
        //shite, different times. File has been changed.
        QFile fill(filename);//file->getFileName());
        if (fill.open(QIODevice::ReadOnly))
        {
            QTextStream str(&fill);
            QString file_content = str.readAll();
            QString editor_content = ui->plainTextEdit->toPlainText();
            //oh, this could be huge....
            if (file_content == editor_content)
            {
                //jei, nfp!!
                oksave = true;
            }
            else
            {
                //prompt user that the files has changed
                int result = promptFileChangedAction(file->getShortFileName());
                if (EW_KEEP_THIS_FILE == result)
                {
                    oksave = true;
                }
                else
                {
                    //reload the file.
                    ui->plainTextEdit->setPlainText(file_content);
                }
            }
        }
    }

    if (oksave)
    {
        QString data = ui->plainTextEdit->toPlainText();
        file->save(data);
        QFileInfo infoo(filename);//(file->getFileName());
        this->lastModified = infoo.lastModified();
    }
}

/**
  TODO - this method should be in the NPFile, not in the editor window.
  In fact, when it comes to refactoring NP, I think that this editor
  windows are going to have to be invisible to the MainWindow.
  */
void HillStreetEditor::fileChangedExternally()
{
    //oho!

    static QMutex mutex;

    if (mutex.tryLock())
    {

        if (!QFile::exists(filename))
        {
            QString msg = tr("File deleted!!! Recreate or close editor?");
            QMessageBox ques(this);
            QPushButton *buttonRecreate = ques.addButton(tr("Recreate"), QMessageBox::ActionRole);
            ques.addButton(tr("Close"), QMessageBox::ActionRole);
            ques.setWindowTitle(filename);
            ques.setText(msg);

            ques.exec();

            if (ques.clickedButton() == buttonRecreate)
            {
                QFile newFile(filename);
                newFile.open(QIODevice::ReadWrite);
                QString data = ui->plainTextEdit->toPlainText();
                newFile.write(data.toUtf8());
                newFile.close();
                this->hookUpFileListener();
                this->file->addLabels();
            }
            else
            {
                this->close();
            }

        }
        else
        {

            QFileInfo info(filename);//file->getFileName());

            if (lastModified != info.lastModified())
            {
                QFile *fill = new QFile(filename);//file->getFileName());
                fill->open(QIODevice::ReadOnly);
                QTextStream str(fill);
                QString file_content = str.readAll();
                QString editor_content = ui->plainTextEdit->toPlainText();
                //oh, this could be huge....
                if (file_content != editor_content)
                {
                    //eep, the files are different
                    int result = promptFileChangedAction(file->getShortFileName());
                    if (EW_RELOAD_THE_FILE == result)
                    {
                        //reload the file.
                        /* what's with all the 2s? Sometimes, the file reads before it is saved, and then file_content is blank. This makes the
                        file blank. Irritating? Fuck yeah. And don't even get me started on Vim incompatibilities :( */
                        QFile *fill2 = new QFile(filename);
                        fill2->open(QIODevice::ReadOnly);
                        QTextStream str2(fill2);
                        QString file_content2 = str2.readAll();
                        ui->plainTextEdit->setPlainText(file_content2);
                        this->file->addLabels();
                    }
                    else
                    {
                        //TODO - refactor to single method.
                        QString data = ui->plainTextEdit->toPlainText();
                        file->save(data);
                        QFileInfo infoo(filename);
                        this->lastModified = infoo.lastModified();
                    }
                }
                delete (fill);
            }
        }
        mutex.unlock();
    }
}

/**
  * ;P
  */
void HillStreetEditor::hookUpFileListener()
{
    QStringList paths;
    paths.append(filename);//file->getFileName());
    watcher = new QFileSystemWatcher(paths, this);
    connect(watcher,SIGNAL(fileChanged(QString)),this, SLOT(fileChangedExternally()));
}



int HillStreetEditor::promptFileChangedAction(QString filename)
{
    QString msg = ("Woah, ");
    msg += (filename);
    msg += (" changed externally! Use the external one?");

    if (QMessageBox::question(this,file->getShortFileName(),msg,QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
    {
        return EW_RELOAD_THE_FILE;
    }
    else
    {
        return EW_KEEP_THIS_FILE;
    }
}

#include <QDebug>
void HillStreetEditor::keyPressEvent(QKeyEvent *kev)
{
    // I don't know if there is anything else the escape key could do?
    if(kev->key() == 16777216)
    {
        this->setFullScreen(false);
    }
}

void HillStreetEditor::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void HillStreetEditor::showFileInfo()
{
    // get the file name and the file info
    QFileInfo info(filename);
    QString fileinfo("File Information:\n\n");
    fileinfo.append("Filename: ").append(file->getShortFileName()).append("\n");
    fileinfo.append("Location: ").append(info.absoluteFilePath()).append("\n");
    fileinfo.append("Canonical Location: ").append(info.canonicalFilePath()).append("\n");
    QVariant var(info.size());
    fileinfo.append("Size: ").append(var.toString()).append(" bytes\n");
    QString frmt = settings->GetDateFormat();
    fileinfo.append("Created: ").append(info.created().toString(frmt)).append("\n");
    fileinfo.append("Last modified: ").append(info.lastModified().toString(frmt)).append("\n");
    fileinfo.append("Last read: ").append(info.lastRead().toString(frmt)).append("\n");
    fileinfo.append("Owner: ").append(info.owner()).append("\n");

    QMessageBox::information(this, file->getShortFileName(), fileinfo, QMessageBox::Ok);
}

void HillStreetEditor::dateStamp()
{
    QDateTime now = QDateTime::currentDateTime();
    QString stamp = now.toString("ddd dd/MM/yyyy hh:mm:ss");
    ui->plainTextEdit->insertPlainText(stamp);
}

void HillStreetEditor::newFile()
{
    emit(newFileSignal());
}

void HillStreetEditor::hardWrap()
{
    hardWordWrap(false);
}

void HillStreetEditor::hardUnwrap()
{
    hardWordWrap(true);
}

void HillStreetEditor::hardWordWrap(bool unwrap)
{
    // check to see if there is anything selected.
    QTextCursor cursor = ui->plainTextEdit->textCursor();
    QString slected = cursor.selectedText();
    bool doWholePage = false;

    if (slected.length() == 0)
    {
        // ohoh have to do everything
        slected = ui->plainTextEdit->toPlainText();
        doWholePage = true;
    }

    QString newString = this->startBlockify(slected, unwrap);

    if (doWholePage) {
        cursor.beginEditBlock();
        cursor.select(QTextCursor::Document);
        cursor.insertText(newString);
        cursor.endEditBlock();
    }
    else {
        cursor.beginEditBlock();
        cursor.endEditBlock();
        cursor.insertText(newString);
    }

}

/**
  Logically, this method would be two separate methods. Originally it was
  meant to work out if you are blocking or unblocking, but that didn't
  work very well, so I broke it in two. I could have untangled this bit
  as well, but couldn't be bothered. :P
  */
QString HillStreetEditor::startBlockify(QString string, bool unblockify)
{
    wchar_t split = CARRAIGE_RETUN;
    QString processedLine;
    QStringList components = string.split(split);
    QStringListIterator iterator(components);

    int blockedLineLength = settings->GetHardwrapLength();


    int longest = 0;
    while (iterator.hasNext())
    {
        QString line = iterator.next();
        if (line.length() > longest)
        {
            longest = line.length();
        }
    }
    if (!unblockify)
    {
        // blockify

        // who would have thought that the blockification process could possibly be so fiddley??
        // I'm sure that there is a simpler way to do it, I just don't know it yet.

        if (!(longest <= blockedLineLength) )
        {

            QStringList paragraphs = string.split(split);
            QStringListIterator paragraph_iterator(paragraphs);
            bool needToAddCR = false;

            while (paragraph_iterator.hasNext())
            {
                int currentLineLength = 0;
                QString a_paragraph = paragraph_iterator.next();
                QStringList words = a_paragraph.split(" ");
                QStringListIterator words_iterator(words);
                QString line = "";
                bool blank_paragraph = true;

                while (words_iterator.hasNext())
                {
                    QString word = words_iterator.next();

                    if (word != "") {
                        if (needToAddCR)
                        {
                            needToAddCR = false;
                            processedLine.append(split);
                        }
                        blank_paragraph = false;
                        if (word.length() > 0)
                        {
                            if ((word.length() + currentLineLength + 1) < blockedLineLength)
                            {
                                line.append(word);
                                line.append(" ");
                                currentLineLength += word.length();
                                currentLineLength += 1;
                            }
                            else
                            {
                                processedLine.append(line);
                                processedLine.append(split);
                                line.clear();
                                line.append(word);
                                line.append(" ");
                                currentLineLength = line.length();
                                currentLineLength += 1;
                            }
                        }
                    }
                }

                processedLine.append(line);
                if (!blank_paragraph) {
                    if (paragraph_iterator.hasNext()) {
                        needToAddCR = true;
                        processedLine.append(split);
                    }
                }
            }
        }
        else
        {
            processedLine = string;
        }
        // OK, that may be blockified?
    }
    else
    {
        // reverse blockify
        if (longest < blockedLineLength)
        {
            bool needToAddCR = false;
            QVariant CR(split);
            QString CRCR;
            CRCR.append(CR.toChar());
            CRCR.append(CR.toChar());

            QStringList paragraphs = string.split(CRCR);
            QStringListIterator paragraphs_iterator(paragraphs);

            while (paragraphs_iterator.hasNext())
            {
                QString paragraph = paragraphs_iterator.next();
                QStringList lines = paragraph.split(split);
                QStringListIterator lines_iterator(lines);

                while (lines_iterator.hasNext())
                {
                    QString line = lines_iterator.next();

                    if (line.length() > 0)
                    {
                        if (needToAddCR)
                        {
                            needToAddCR = false;
                            processedLine.append(split);
                        }

                        processedLine.append(line);
                        if (!(processedLine.endsWith(" ")))
                        {
                            processedLine.append(" ");
                        }
                    }
                }
                processedLine.append(split);

                if (paragraphs_iterator.hasNext())
                {
                    needToAddCR = true;
                }
            }
        }
        else
        {
            processedLine = string;
        }
        /* end of the reverse blockify move. */
    }
    return processedLine;
}

void HillStreetEditor::exportFile()
{
    emit(exportThisFilePlease(filename));
}

QString HillStreetEditor::getString()
{
    return ui->plainTextEdit->toPlainText();
}

void HillStreetEditor::openFindReplaceDialog()
{
    emit openFindAndReplace();
}

void HillStreetEditor::openTrimLinesDialog()
{
    emit openTopAndTailPlease();
}

void HillStreetEditor::wordWrap(bool wrap)
{
    if (wrap)
    {
        ui->plainTextEdit->setWordWrapMode(QTextOption::WordWrap);
    }
    else
    {
        ui->plainTextEdit->setWordWrapMode(QTextOption::NoWrap);
    }
}

void HillStreetEditor::setOverwriteMode(bool insert)
{
    ui->plainTextEdit->setOverwriteMode(insert);
}

void HillStreetEditor::setTranslucent(bool translucent)
{
    qreal level = 1.0;
    if (translucent) level = 0.60;
    this->setWindowOpacity(level);
}

void HillStreetEditor::setFullScreen(bool yesFullScreen)
{
    static int backUpWidth = 0;
    static int maxScreenSize = 0;
    if (yesFullScreen && !m_isFullScreen)
    {
        if (maxScreenSize)
        {
            this->setFixedWidth(maxScreenSize);
            this->showFullScreen();
        }
        else
        {
            backUpWidth = this->ui->plainTextEdit->width();
            this->showFullScreen();
            maxScreenSize = this->ui->plainTextEdit->width();
        }
        int wdth = maxScreenSize;
        if (wdth > 950) {
            wdth /= 8;
            wdth *= 5;
        }
        else { /* small screens have a smaller text area. of course. */
            wdth *= 95;
            wdth /= 100;
        }
        this->ui->plainTextEdit->setFixedWidth(wdth);
        this->ui->centralwidget->layout()->setContentsMargins(0,50,0,40);
        m_isFullScreen = true;
    }
    else if (m_isFullScreen)
    {
        this->ui->plainTextEdit->setFixedWidth(QWIDGETSIZE_MAX);
        this->ui->centralwidget->layout()->setContentsMargins(0,0,0,0);
        this->setFixedWidth(backUpWidth);
        this->showNormal();
        m_isFullScreen = false;
    }
}

void HillStreetEditor::setSecondFont(bool secondFont)
{
    if (secondFont)
    {
        ui->plainTextEdit->document()->setDefaultFont(fontTwo);
        usingFontOne = false;
    }
    else
    {
        ui->plainTextEdit->document()->setDefaultFont(fontOne);
        usingFontOne = true;
    }
}

void HillStreetEditor::incrementFontSize()
{
    int size = fontOne.pointSize();
    if (size < 150)
    {
        size++;
        fontOne.setPointSize(size);
        fontTwo.setPointSize(size);
        if (usingFontOne)
        {
            ui->plainTextEdit->document()->setDefaultFont(fontOne);
        }
        else
        {
            ui->plainTextEdit->document()->setDefaultFont(fontTwo);
        }
    }
}

void HillStreetEditor::decrementFontSize()
{
    int size = fontOne.pointSize();
    if (size > 3)
    {
        size--;
        fontOne.setPointSize(size);
        fontTwo.setPointSize(size);
        if (usingFontOne)
        {
            ui->plainTextEdit->document()->setDefaultFont(fontOne);
        }
        else
        {
            ui->plainTextEdit->document()->setDefaultFont(fontTwo);
        }

    }
    this->closeWindow();
}

void HillStreetEditor::quickFile()
{
    emit createQuickFile();
}

QPlainTextEdit *HillStreetEditor::getPlainText()
{
    return ui->plainTextEdit;
}

void HillStreetEditor::closeWindow()
{
    this->close();
    // notification that the file is closed is sent back to the
    // file manager by the Qt parts.
}

void HillStreetEditor::renameFile(QString newFileName)
{
    Q_UNUSED(newFileName)
}
