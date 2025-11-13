TARGET      = notepuppy
HEADERS     += \  \
    src/meeting.h \
    src/newfiledialog.h \
    src/mainwindow.h \
    src/directorytreelister.h \
    src/ttouter.h \
    src/ttinner.h \
    src/npsettings.h \
    src/npfile.h \
    src/fileinfo.h \
    src/hillstreeteditor.h \
#    src/findreplace.h \
#    src/topandtail.h \
    src/npsyntaxhighlighter.h \
    src/onewindow.h \
    src/internalnotepuppy.h \
    src/filezapper.h \
    src/newfilewidget.h \
    src/onesmallwindow.h \
    src/smalldevicewindow.h
SOURCES     +=  qtmain.cpp \
    src/meeting.cpp \
    src/newfiledialog.cpp \
    src/mainwindow.cpp \
    src/directorytreelister.cpp \
    src/ttouter.cpp \
    src/ttinner.cpp \
    src/npsettings.cpp \
    src/npfile.cpp \
    src/fileinfo.cpp \
    src/hillstreeteditor.cpp \
#    src/findreplace.cpp \
#    src/topandtail.cpp \
    src/npsyntaxhighlighter.cpp \
    src/onewindow.cpp \
    src/internalnotepuppy.cpp \
    src/filezapper.cpp \
    src/newfilewidget.cpp \
    src/onesmallwindow.cpp \
    src/smalldevicewindow.cpp
FORMS       += \
    src/meeting.ui \
    src/newfiledialog.ui \
    src/mainwindow.ui \
    src/fileinfo.ui \
    src/hillstreeteditor.ui \
    src/editorwindow.ui \
#    src/findreplace.ui \
#    src/topandtail.ui \
    src/onewindow.ui \
    src/filezapper.ui \
    src/newfilewidget.ui \
    src/onesmallwindow.ui
LEXSOURCES  += #LEXS#
YACCSOURCES += #YACCS#

INCLUDEPATH +=
LIBS        +=
DEFINES +=
#DEFINES     += Q_WS_MAEMO_5

# All generated files goes same directory
OBJECTS_DIR = build
MOC_DIR     = build
UI_DIR      = build

DESTDIR     = build
TEMPLATE    = app
DEPENDPATH  +=
VPATH       += src uis
CONFIG      -= 
CONFIG      += debug
QT=core gui widgets

INSTALLS    += target
target.path  = /usr/bin/

INSTALLS    += desktop
desktop.path  = /usr/share/applications/hildon
desktop.files  = data/notepuppy.desktop

INSTALLS    += service
service.path  = /usr/share/dbus-1/services
service.files  = data/notepuppy.service

INSTALLS    += icon64
icon64.path  = /usr/share/icons/hicolor/64x64/apps
icon64.files  = data/64x64/notepuppy.png

#
# Targets for debian source and binary package creation
#
debian-src.commands = dpkg-buildpackage -S -r -us -uc -d
debian-bin.commands = dpkg-buildpackage -b -r -uc -d
debian-all.depends = debian-src debian-bin

#
# Clean all but Makefile
#
compiler_clean.commands = -$(DEL_FILE) $(TARGET)

QMAKE_EXTRA_TARGETS += debian-all debian-src debian-bin compiler_clean

OTHER_FILES += \
    moar_win_icons.rc

symbian {
    ICON  = dawg.svg
}

unix:!macx:!symbian { #unix includes maemo5 - to block maemo5, use unix:!maemo5
#    QT += dbus
    DEFINES += Q_WS_UNIX
}

# Maemo 5 parts of the app - NotePuppy is a Maemo 5 app at heart, never forget that!
maemo5 {
    DEFINES += Q_WS_MAEMO_5
}
# All desktops (well, UMW anyway)
#unix:mac:win32:!maemo5:!symbian {
#    RESOURCES += res.qrc
#} # Actually not really used anymore.
# Lunix parts of the app
unix { #unix includes maemo5 - to block maemo5, use unix:!maemo5
}
# Mac OS X parts of the app
macx {
    ICON = NotePuppy.icns
#    DEFINES += Q_WS_MAC
}
# Windows parts of the app
win32 {
    DEFINES += Q_WS_WINDOWS
}

win32:RC_FILE = moar_win_icons.rc

RESOURCES += \
    win_icons.qrc






