#ifndef SMALLDEVICEWINDOW_H
#define SMALLDEVICEWINDOW_H

#include <QMainWindow>

class HillStreetEditor;
class NPFile;

class SmallDeviceWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit SmallDeviceWindow(QWidget *parent = 0);
    
    virtual void createAndOpenFile(QString fname) = 0;
    virtual void closeWidget(QWidget *widge) = 0;

signals:
    
public slots:
    virtual void openFile(NPFile *file);
    virtual void openFile();
    virtual void closeFile(QString filename);
    virtual void importFile();
    virtual void exportFile();
    virtual void exportThisFile(QString file);
    virtual void renameFile();
    virtual void newFile();
    virtual void quickFile();
    virtual void deleteFile();
    virtual void buildList();
    virtual void clearSelection();
    virtual void setEditorActive(HillStreetEditor *active);
    virtual void openAnEditor(HillStreetEditor *editor);

    // And these are the "local slots"
    // TODO - the naming is odd, should change
    virtual void plusButtonPressed() = 0;
    virtual void minusButtonPressed() = 0;
    virtual void undoButtonPressed() = 0;
    virtual void redoButtonPressed() = 0;
    virtual void closeButtonPressed() = 0;
};

#endif // SMALLDEVICEWINDOW_H
