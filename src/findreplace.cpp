#include "findreplace.h"
#include "ui_findreplace.h"

#include <QDebug>

#include <QPlainTextEdit>
#include <QTextBlock>

FindReplace::FindReplace(QWidget *parent) :
    QDialog(parent)
    , ui(new Ui::FindReplace)
    , caseSensitive(Qt::CaseInsensitive)
    , mode(FIND)
    , position(1)
{
    this->document = 0x0;
    this->editor = 0x0;
    ui->setupUi(this);

    connect(ui->checkBox, SIGNAL(clicked(bool)), this, SLOT(setCaseSensitive(bool)));
    connect(ui->findButton, SIGNAL(clicked(bool)), this, SLOT(setFindMode(bool)));
    connect(ui->replaceButton, SIGNAL(clicked(bool)), this, SLOT(setReplaceMode(bool)));
    connect(ui->editLiveButton, SIGNAL(clicked(bool)), this, SLOT(setEditLiveMode(bool)));
    connect(ui->buttonPrevious, SIGNAL(clicked()), this, SLOT(previousButtonPressed()));
    connect(ui->buttonNeext, SIGNAL(clicked()), this, SLOT(nextButtonPressed()));
    connect(ui->buttonAll, SIGNAL(clicked()), this, SLOT(allButtonPressed()));
    connect(ui->le_Find, SIGNAL(textChanged(QString)), this, SLOT(findTextEntered(QString)));
    connect(ui->le_Replace, SIGNAL(textChanged(QString)), this, SLOT(replaceTextEntered(QString)));

    QBrush lowBrush(Qt::yellow); /* Yellow background - it's what Chrome uses.... */
    QColor orange(0xDD, 0x75, 0x00);
    QBrush highBrush(orange); /* Orange for highlighted. Chrome again. */
    lowlight.setBackground(lowBrush);
    highlight.setBackground(highBrush);
    ui->le_Find->setFocus();
}

FindReplace::~FindReplace()
{
    delete ui;
}

void FindReplace::allButtonPressed()
{

}

void FindReplace::checkPositionLegal()
{
    if (position > count)
    {
        position = 1;
    }
    else if (position < 1)
    {
        if (count != 0)
        {
            position = count;
        }
        else
        {
            /* this is an illegal position */
            position = 1;
        }
    }
}

void FindReplace::clearHighlightedTerms()
{

    /* current implementation is not very efficient. TODO - make good */
    if (0x0 != document)
    {
        int numBlocks = this->document->blockCount();

        for (int i = 0; i < numBlocks; ++i)
        {
            QTextBlock block = document->findBlockByNumber(i);
            block.blockFormat().clearBackground();
            QTextLayout *layout = block.layout();
            QList<QTextLayout::FormatRange> ranges = layout->additionalFormats();
            bool formatsChanged = false;

            const int preeditAreaStart = layout->preeditAreaPosition();
            const int preeditAreaLength = layout->preeditAreaText().length();

            if (preeditAreaLength != 0) {
                QList<QTextLayout::FormatRange>::Iterator it = ranges.begin();
                while (it != ranges.end()) {
                    if (it->start >= preeditAreaStart
                        && it->start + it->length <= preeditAreaStart + preeditAreaLength) {
                        ++it;
                    } else {
                        it = ranges.erase(it);
                        formatsChanged = true;
                    }
                }
            } else if (!ranges.isEmpty()) {
                ranges.clear();
                formatsChanged = true;
            }

            if (formatsChanged)
            {
                block.layout()->setAdditionalFormats(ranges);
                document->markContentsDirty(block.position(),  block.length());
            }
        }
    }
}

void FindReplace::close()
{
    this->close();
}

void FindReplace::closeEvent(QCloseEvent *)
{
    emit closed();
}

void FindReplace::editorClosed(HillStreetEditor *editor)
{
    if (this->editor == editor)
    {
        document = 0x0;
        this->editor = 0x0;
    }
}

void FindReplace::flick()
{
    findTextEntered(searchTerm);
}

void FindReplace::findTextEntered(QString newText)
{
    if (document)
    {
        searchTerm = newText;
        // Find all the text which matches the entered text. Count it.
        count = editor->getPlainText()->document()->toPlainText().count(newText, caseSensitive);

        clearHighlightedTerms();
        highlightFound();

        handleFocus();
    }
    //   highlighter->matchPhrase(newText, position, caseSensitive);
 //   highlighter->setDocument(editor->getPlainText()->document());
}

void FindReplace::handleFocus()
{
    QString message;
    if (!(searchTerm.length() < 1))
    {
        if (count == 0)
        {
            message = "Search term \"";
            message += searchTerm;
            message += "\" not found";
        }
        else
        {
            this->checkPositionLegal();
            message = "\"";
            message += searchTerm;
            message +="\", selected ";
            message += intToString(position);
            message += " of ";
            message += intToString(count);
        }
    }
    writeMessage(message);
}

void FindReplace::highlightFound()
{
    int textLength = searchTerm.length();
    if (textLength > 0)
    {
        QTextDocument *dork = editor->getPlainText()->document();
        int blockCount = dork->blockCount();
        quint32 whichTerm = 1;

        for (int i = 0; i < blockCount; ++i)
        {
            QTextBlock block = dork->findBlockByNumber(i);
            QString text = block.text();
            bool formatsChanged = false;

            QTextLayout *layout = block.layout();
            QList<QTextLayout::FormatRange> ranges = layout->additionalFormats();

            const int preeditAreaStart = layout->preeditAreaPosition();
            const int preeditAreaLength = layout->preeditAreaText().length();

            if (preeditAreaLength != 0) {
                QList<QTextLayout::FormatRange>::Iterator it = ranges.begin();
                while (it != ranges.end()) {
                    if (it->start >= preeditAreaStart
                        && it->start + it->length <= preeditAreaStart + preeditAreaLength) {
                        ++it;
                    } else {
                        it = ranges.erase(it);
                        formatsChanged = true;
                    }
                }
            } else if (!ranges.isEmpty()) {
                ranges.clear();
                formatsChanged = true;
            }

            int from = 0;
            int pos = text.indexOf(searchTerm, from, caseSensitive);

            while (pos != -1)
            {
                QTextLayout::FormatRange r;
                r.start = pos;
                r.length = textLength;
                if (whichTerm == position)
                {
                    r.format = highlight;
                    // which line?
                    int line = block.firstLineNumber();
                    // out of how many lines?
                    int howManyLines = dork->lineCount();
                    // woah! so how big is the page?
                    int pageHeight = dork->size().height();

                    // alright
                    qDebug() << "line" << line << "out of" <<howManyLines << "total  size" << pageHeight;
                    // this approach does not work :(
                }
                else
                {
                    r.format = lowlight;
                }
                whichTerm++;

                from = pos + textLength;
                pos = text.indexOf(searchTerm, from, caseSensitive);
                formatsChanged = true;
                ranges << r;
            }

            if (formatsChanged)
            {
                block.layout()->setAdditionalFormats(ranges);
                dork->markContentsDirty(block.position(), block.length());
            }
        }
//        editor->getPlainText()->ensureCursorVisible();
    }
}

QString FindReplace::intToString(int number)
{
    QVariant var(number);
    return var.toString();
}

void FindReplace::nextButtonPressed()
{
    position++;
    checkPositionLegal();
    flick();
}

void FindReplace::previousButtonPressed()
{
    position--;
    checkPositionLegal();
    flick();
}

void FindReplace::replaceTextEntered(QString newText)
{
    replaceWith = newText;
}

void FindReplace::setCaseSensitive(bool selected)
{
    if (selected)
    {
        caseSensitive = Qt::CaseSensitive;
    }
    else
    {
        caseSensitive = Qt::CaseInsensitive;
    }
    findTextEntered(this->searchTerm);
}

void FindReplace::setEditor(HillStreetEditor *editor)
{
    if (!(editor == this->editor))
    {
        // if there is another editor, then clean it first.
        if (this->editor != 0x0)
        {
            clearHighlightedTerms();
        }

        // setup the new editor
        this->editor = editor;
        this->document = editor->getPlainText()->document();
        // clear all the highlighted terms first
        clearHighlightedTerms();
        // highlight the new ones
        flick();
    }
}

void FindReplace::setEditLiveMode(bool selected)
{
    if (selected && EDITLIVE != mode)
    {
        // start transition
        ui->le_Replace->setText("");
        ui->le_Replace->setReadOnly(false);
        mode = EDITLIVE;
    }
}

void FindReplace::setFindMode(bool selected)
{
    if (selected && FIND != mode)
    {
        // start transition
        ui->le_Replace->setText("");
        ui->le_Replace->setReadOnly(true);
        mode = FIND;
    }
}

void FindReplace::setReplaceMode(bool selected)
{
    if (selected && REPLACE != mode)
    {
        // start transition
        ui->le_Replace->setText("");
        ui->le_Replace->setReadOnly(false);
        mode = REPLACE;
    }
}

void FindReplace::writeMessage(QString message)
{
    ui->outputLabel->setText(message);
}
