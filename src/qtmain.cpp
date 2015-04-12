/*
 * Created: 05/13/10-20:35:58
 * Author: rachel
 */


#ifdef Q_WS_MAEMO_5
#define DO_DBUS
#endif /* Q_WS_MAEMO_5 */

// Deprecated - I don't think I need DBUS any more.
//#ifdef Q_WS_UNIX
//#define DO_DBUS
//#endif /* Q_WS_UNIX */

//#define DEV

#ifndef DEV
#ifdef DO_DBUS
#include <QDBusConnection>
#include <QDBusError>
#endif /* DO_DBUS */

#endif /* DEV */

#ifdef DEV

#ifdef DO_DBUS
#undef DO_DBUS
#endif /* DON'T_DO_DBUS */

#define ONE_SCREEN

#endif /* DEV */

//#include <QCoreApplication>
#include <QApplication>
//#include <QPushButton>
#ifdef ONE_SCREEN
#include "onewindow.h"
#else
#include "mainwindow.h"
#endif /* ONESCREEN */



int main(int argc, char *argv[])
{
        QApplication app(argc, argv);

#ifdef DO_DBUS
        QString serviceName = "com.loralora.notepuppy";
        QString objectName = "/usr/bin/notepuppy";

        QDBusConnection bus = QDBusConnection::sessionBus();

        if (!bus.isConnected())
        {
            qWarning("No session bus :-(");
        }

        if (!bus.registerService(serviceName))
        {
            qWarning("Can not register service - NotePuppy is probably already open.");
            return 0;
        }

        if (!bus.registerObject(objectName, &app))
        {
            qWarning("Can not register object :( %s", "perkele");
            return 0;
        }
#endif /* DO_DBUS */

#ifdef Q_WS_WINDOWS
        // Because there just aren't enough ifdefs here yet.
        QIcon mutt(":/woof/cute.png");
        app.setWindowIcon(mutt);
#endif // Q_WS_WINDOWS


#ifdef ONE_SCREEN
        OneWindow main;
  #ifdef Q_WS_MAC
        main.show();
  #else /* ndef Q_WS_MAC */
        main.showFullScreen();
  #endif /* Q_WS_MAC */
#else /* ndef ONE_SCREEN */
        MainWindow main;
        main.show();
#endif /* ONE_SCREEN */

        int appex = app.exec();

#ifdef DO_DBUS
        bus.unregisterService(serviceName);
        bus.unregisterObject(objectName);
#endif /* DO_DBUS */

#ifdef DO_DBUS
#undef DO_DBUS
#endif /* DO_DBUS */

        return appex;
}
