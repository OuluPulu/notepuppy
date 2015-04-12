#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include "hillstreeteditor.h"
#include "internalnotepuppy.h"
#include "npsettings.h"
#include "directorytreelister.h"
#include "topandtail.h"
#include "findreplace.h"

#include <QMainWindow>
#include <QMap>
#include <QListWidgetItem>
#include <QDir>
#include <QTreeWidgetItem>
#include <QContextMenuEvent>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void contextMenuEvent(QContextMenuEvent *event);

public slots:
    void showHideButtons(bool show);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;

    InternalNotePuppy *inp;
    bool translucent;

public slots:
    // Relay slots
    void openFile(NPFile *file);
    void openFile();
    void closeFile(QString filename);
    void importFile();
    void exportFile();
    void exportThisFile(QString file);
    void renameFile();
    void newFile();
    void quickFile();
    void deleteFile();
    void buildList();
    void clearSelection();
    void setEditorActive(HillStreetEditor *active);

    // do something, says the inp
    void openAnEditor(HillStreetEditor *editor);

    // The PowerTools
    void openTopAndTail();
    void openFindAndReplace();
    void topAndTailClosed();
    void findAndReplaceClosed();

    // Locally relevant slots
    void buttonsOnOff(NPFile *item);
    void setTranslucent(bool translucent);

};

#endif // MAINWINDOW_H
