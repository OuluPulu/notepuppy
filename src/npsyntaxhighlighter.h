#ifndef NPSYNTAXHIGHLIGHTER_H
#define NPSYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>

class NPSyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit NPSyntaxHighlighter(QObject *parent = 0);

    void matchPhrase(QString phrase, quint32 selected, bool caseSensitive);
protected:
    void highlightBlock(const QString &text);
signals:

private:
    QTextCharFormat lowlight;
    QTextCharFormat highlight;
    QString phrase;
    quint32 selected;
    Qt::CaseSensitivity caseSensitive;
public slots:

};

#endif // NPSYNTAXHIGHLIGHTER_H
