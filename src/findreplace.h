#ifndef FINDREPLACE_H
#define FINDREPLACE_H

#include <QDialog>
#include <QTextFormat>
#include <QTextDocument>
//#include "hillstreeteditor.h"

namespace Ui {
    class FindReplace;
}

class FindReplace : public QDialog
{
    Q_OBJECT

public:
    explicit FindReplace(QWidget *parent = 0);
    ~FindReplace();

    enum Mode {
        FIND,
        EDITLIVE,
        REPLACE
    };

    void close();

protected:
    void closeEvent(QCloseEvent *);

private:
    Ui::FindReplace *ui;

    void clearHighlightedTerms();
    void highlightFound();
    void checkPositionLegal();
    void handleFocus();
    void writeMessage(QString message);
    QString intToString(int number);
    void flick();

    /* Variables */
    HillStreetEditor *editor;
    QTextDocument *document;
    Qt::CaseSensitivity caseSensitive;
    Mode mode;
    QString textString;
    QString searchTerm;
    QString replaceWith;
    quint32 position;
    quint32 count;
    QTextCharFormat lowlight;
    QTextCharFormat highlight;

public slots:
    /* from the MainWindow */
    void setEditor(HillStreetEditor *editor);
    void editorClosed(HillStreetEditor *editor);

private slots:
    void setCaseSensitive(bool selected);
    void setFindMode(bool selected);
    void setEditLiveMode(bool selected);
    void setReplaceMode(bool selected);
    void previousButtonPressed();
    void nextButtonPressed();
    void allButtonPressed();
    void findTextEntered(QString newText);
    void replaceTextEntered(QString newText);

signals:
    void closed();
};

#endif // FINDREPLACE_H
