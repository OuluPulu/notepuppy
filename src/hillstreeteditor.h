#ifndef HILLSTREETEDITOR_H
#define HILLS

#include <QMainWindow>
#include <QFile>
#include "ttouter.h"
#include "npfile.h"
#include "npsettings.h"

#define CARRAIGE_RETUN  '\n'

namespace Ui {
    class HillStreetEditor;
}

class QPlainTextEdit;

class HillStreetEditor : public QMainWindow
{
    Q_OBJECT

public:
//    EditorWindow(QString filename, QWidget *parent = 0);
    HillStreetEditor(NPFile *file, NPSettings *settings, QWidget *parent = 0);
    ~HillStreetEditor();

    QString getString();
    // Added for the OneWindow version, 18/12/11
    NPFile *getNPFile() const { return file; }

public slots:
    void save();
    void fileChangedExternally();

    // actions for the Hill Street version of NotePuppy

        //using old-skool pop-up windows.
    void newFile();  /*New File*/
    void exportFile();  /*Export File*/

    //using new-skool window areas
/* TODO */    void openFindReplaceDialog();  /*Find/Replace*/
/* TODO */    void openTrimLinesDialog();  /*Trim lines*/
    void showFileInfo(); /* tells it how it is */
    // not requiring pop-up things at all.

    // non-boolean slots.
    void hardWordWrap(bool unwrap);  /*Hard Wrap*/
    void hardWrap();
    void hardUnwrap();
    void dateStamp();  /*Date Stamp*/
    void incrementFontSize();
    void decrementFontSize();
    void quickFile();

    // these are system calls - not for front of house access =)
/* TODO */      void closeWindow();
/* TODO */      void renameFile(QString newFileName);

    // boolean ones
    void wordWrap(bool wrap); /* word wrap on or off */
    void setOverwriteMode(bool insert); /* insert mode on or off */
    void setTranslucent(bool translucent);
    /* special feature for the puppy xxx */
    void setFullScreen(bool yesFullScreen);
    void setSecondFont(bool secondFont);

    QPlainTextEdit *getPlainText();

signals:
    void closed(QString filename);
    void save(QString data);

    void exportThisFilePlease(QString filename);
    void newFileSignal();
    void createQuickFile();

    void editorActivated(HillStreetEditor *activated);

    void openTopAndTailPlease();
    void openFindAndReplace();

protected:
    void changeEvent(QEvent *e);
    bool event(QEvent *event);
    void keyPressEvent(QKeyEvent *kev);

private:
    Ui::HillStreetEditor *ui;
    QAction *jesus;
    TTOuter *outer;
    NPFile *file;
    QString filename;
    NPSettings *settings;

    //file watching equipment
    QDateTime lastModified;
    int promptFileChangedAction(QString filename);
    QFileSystemWatcher *watcher;
    void hookUpFileListener();

    // and this is all for the blockify and reverse-blockify method.
    QString startBlockify(QString string, bool unblockify);

    /* solution for font toggling and size. probably should put this into a separate object later */
    QFont fontOne;
    QFont fontTwo;
    bool usingFontOne;

    bool m_isFullScreen;
};

#endif // EDITORWINDOW_H
