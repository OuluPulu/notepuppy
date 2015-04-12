#ifndef INTERNALNOTEPUPPY_H
#define INTERNALNOTEPUPPY_H

#include "directorytreelister.h"
//#include "findreplace.h"
#include "hillstreeteditor.h"
#include "npfile.h"
#include "npsettings.h"
//#include "topandtail.h"

#include <QContextMenuEvent>
#include <QMap>
#include <QTreeWidget>
#include <QObject>

class InternalNotePuppy : public QObject
{
    Q_OBJECT
public:
    InternalNotePuppy(QTreeWidget *treeWidget, QWidget *parent = 0);

    void contextMenuEvent(QContextMenuEvent *event);
    DirectoryTreeLister *dtl() { return m_dtl; }
    void createAndOpenFile(QString filename);

private:
    NPSettings *settings;
    QMap<QString, HillStreetEditor*> kids;
    DirectoryTreeLister *m_dtl;
    /* Tracks the last activated editor - may not ACTUALLY still be active */
    HillStreetEditor *activeEditor;

    QWidget *parentWidget;
    QTreeWidget *m_treeWidget;

    /* The PowerTools */
//    FindReplace *findAndReplace;
//    TopAndTail *topAndTail;

    //functions
    void interOpenFile(NPFile *file);
    bool directoryHasChildren(QString direc);
    QStringList openChildren(QString direc);
    void exportThisFile(NPFile *theOne);

    /**
      Returns true iff all the files and directories inside of it are deleted.
      Does not stop if there is an error but carries on trashing things. Don't
      know how good this approach is yet.
      */
    bool deleteFolder(QDir dir);
    bool copyFile(QFile *original, QString destination, bool force);


    /**
      Returns true iff all the files and directories are copied across. Does
      not stop if there is an error, but carries on. Don't know how good this
      approach is yet.
      */
    bool copyDirectory(QDir original, QString destination, bool overwrite);

public slots:
    void openFile(NPFile *file);
    void openFile();
    void closeFile(QString filename);
    void importFile();
    void exportFile();
    void exportThisFile(QString file);
    void renameFile();
    void newFile();
    void quickFile();
//    void buttonsOnOff(NPFile *item);
    void deleteFile();
//    void showHideButtons(bool show);
    void buildList();
    void clearSelection();
    void setTranslucent(bool translucent);
    void setEditorActive(HillStreetEditor *active);
    /* The PowerTools */
    void openTopAndTail();
    void openFindAndReplace();
    void topAndTailClosed();
    void findAndReplaceClosed();

signals:
    void editorChanged(HillStreetEditor *newEditor);
    void editorClosed(HillStreetEditor *editor);
    void editorReadyToOpen(HillStreetEditor *editor);



};

#endif // INTERNALNOTEPUPPY_H
