#include "npsyntaxhighlighter.h"

#include <QColor>
#include <QDebug>

NPSyntaxHighlighter::NPSyntaxHighlighter(QObject *parent) :
    QSyntaxHighlighter(parent)
    , phrase("")
    , selected(0)
    , caseSensitive(Qt::CaseInsensitive)
{
//    QColor grey(63, 63, 63);
    QBrush greyBrush(Qt::yellow);
    lowlight.setBackground(greyBrush);
    QColor blue(0, 0, 127);
    QBrush blueBrush(blue);
    highlight.setBackground(blueBrush);
}

void NPSyntaxHighlighter::matchPhrase(QString phrase, quint32 selected, bool caseSensitive)
{
    this->phrase = phrase;
    this->selected = selected;
    this->caseSensitive = (caseSensitive?Qt::CaseSensitive:Qt::CaseInsensitive);
}

void NPSyntaxHighlighter::highlightBlock(const QString &text)
{
    int length = phrase.length();
    if (text != "" && length > 0)
    {
        int from = 0;
        int pos = text.indexOf(phrase, from, caseSensitive);

        while (pos != -1)
        {
            setFormat(pos, length, lowlight);
            from = pos + length;
            pos = text.indexOf(phrase, from, caseSensitive);
        }
    }
}
