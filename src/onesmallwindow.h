#ifndef ONEsmallWINDOW_H
#define ONEsmallWINDOW_H

#include "directorytreelister.h"
//#include "hillstreeteditor.h"
#include "internalnotepuppy.h"
#include "npsettings.h"
#include "smalldevicewindow.h"

#include <QMainWindow>


namespace Ui {
    class OneSmallWindow;
}

class OneSmallWindow : public SmallDeviceWindow
{
    Q_OBJECT

public:
    explicit OneSmallWindow(QWidget *parent = 0);
    ~OneSmallWindow();

    void createAndOpenFile(QString fname);
    void closeWidget(QWidget *widge);

private:
    Ui::OneSmallWindow *ui;
//    DirectoryTreeLister *dtl;
    NPSettings *settings;
    InternalNotePuppy *inp;

    /**
     * Reutrn NULL if there is no current editor.
     */
    HillStreetEditor *getEditor(int which = -1);

public slots:
    // These are the "API slots"
    void openFile(NPFile *file);
    void openFile();
//    void closeFile(QString filename);
//    void importFile();
//    void exportFile();
//    void exportThisFile(QString file);
//    void renameFile();
//    void newFile();
//    void quickFile();
//    void deleteFile();
    void buildList();
    void clearSelection();
//    void setEditorActive(HillStreetEditor *active);
    void openAnEditor(HillStreetEditor *editor);

    // And these are the "local slots"
    void plusButtonPressed();
    void minusButtonPressed();
    void undoButtonPressed();
    void redoButtonPressed();
    void closeButtonPressed();
};

#endif // ONEsmallWINDOW_H
