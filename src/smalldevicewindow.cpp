#include "smalldevicewindow.h"

SmallDeviceWindow::SmallDeviceWindow(QWidget *parent) :
    QMainWindow(parent)
{
}

void SmallDeviceWindow::openFile(NPFile *file)
{
    Q_UNUSED(file)
}

void SmallDeviceWindow::openFile() {}

void SmallDeviceWindow::closeFile(QString filename)
{
    Q_UNUSED(filename)
}

void SmallDeviceWindow::importFile() {}

void SmallDeviceWindow::exportFile() {}

void SmallDeviceWindow::exportThisFile(QString file)
{
    Q_UNUSED(file)
}

void SmallDeviceWindow::renameFile() {}

void SmallDeviceWindow::newFile() {}

void SmallDeviceWindow::quickFile() {}

void SmallDeviceWindow::deleteFile() {}

void SmallDeviceWindow::buildList() {}

void SmallDeviceWindow::clearSelection() {}

void SmallDeviceWindow::setEditorActive(HillStreetEditor *active)
{
    Q_UNUSED(active)
}

void SmallDeviceWindow::openAnEditor(HillStreetEditor *editor)
{
    Q_UNUSED(editor)
}
