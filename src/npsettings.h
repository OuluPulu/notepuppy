#ifndef NPSETTINGS_H
#define NPSETTINGS_H

#include <QObject>
#include <QSettings>
#include <QDir>

/**
  Simple class to hold the settings used in NotePuppy.

*/

/**
  TODO
  Configurable Settings for later:
  The editor Fonts.
  The hard wrap line length.
  The date stamp format.
  */

// Global crape
#define NOTEYFICATION  "NoteWoof"

//dynamic settings
#define SETTING_DY_ROOT_FOLDER              "root_folder"
#define SETTING_DY_FONT                     "font"
#define SETTING_DY_TABS_AS_SPACES           "tabs_as_spaces"
#define SETTING_DY_SAVE_INTERVAL            "save_interval"
#define SETTING_DY_TAB_SIZE                 "tab_size"
#define SETTING_DY_TRASH_DIR                "trash_dir"
#define SETTING_DY_QUICK_DIR                "quick_dir"
// strings
#define SETTING_DY_STR_DATE_FORMAT              "date_format"
#define SETTING_DY_STR_DATE_STAMP_FORMAT        "date_Stamp_format"
#define SETTING_DY_STR_FONT_ONE                 "font_one"
#define SETTING_DY_STR_FONT_TWO                 "font_two"
// integers
#define SETTING_DY_IN_HARDWRAP_LENGTH          "hardwrap_length"
#define SETTING_DY_IN_FONT_ONE_SIZE            "font_size_one"
#define SETTING_DY_IN_FONT_TWO_SIZE            "font_size_two"
#define SETTING_DY_TAB_SIZE_PX                 "tab_size_pixels"

//static settings
#define SETTING_COMPANY                     "Pulu"
#define SETTING_COMPANY_INTERNAL            "loralora.com"
#define SETTING_APP_NAME                    "NotePuppy"
#define SETTING_APP_VERSION                 "0.7.5"

//default settings
#define SETTING_DEF_FONT                    "font"
#define SETTING_DEF_TABS_AS_SPACES          true
#define SETTING_DEF_SAVE_INTERVAL           30
#define SETTING_DEF_TAB_SIZE                4
#ifdef Q_WS_MAEMO_5 /* The Maemo puters have the whole MyDocs things going on. */
#define SETTING_DEF_ROOT_FOLDER             "MyDocs/NotePuppy"
#else
#ifdef Q_WS_WINDOWS /* Windows Machines have "My Documents" */
#define SETTING_DEF_ROOT_FOLDER             "NotePuppy"
#else
#define SETTING_DEF_ROOT_FOLDER             "NotePuppy"
#endif /* Q_WS_WINDOWS */
#endif /* QS_W_MAEMO_5 */

// FIXME - there must be a better way to use the preprocessor...
#ifdef Q_WS_WINDOWS
#define SETTING_DEF_TRASH_DIR                  "NotePuppy Trash"
#elif defined(Q_OS_SYMBIAN)
#define SETTING_DEF_TRASH_DIR                  "NotePuppy Trash"
#elif defined(Q_OS_ANDROID)
#define SETTING_DEF_TRASH_DIR                  "NotePuppy Trash"
#else
#define SETTING_DEF_TRASH_DIR                  ".Trash"
#endif /* Q_WS_WINDOWS / not-def Q_WS_WINDOWS*/

#define SETTING_DEF_QUICK_DIR                   "Quick"

// strings
#define SETTING_DEF_STR_DATE_FORMAT              "ddd MMM d hh:mm:ss"
#define SETTING_DEF_STR_DATE_STAMP_FORMAT        "ddd dd/MM/yyyy hh:mm:ss"
#define SETTING_DEF_STR_FONT_ONE                 "Lucida Grande"
#define SETTING_DEF_STR_FONT_TWO                 "Lucida Console"

// integers
#define SETTING_DEF_IN_HARDWRAP_LENGTH          80
#define SETTING_DEF_IN_FONT_ONE_SIZE            13
#define SETTING_DEF_IN_FONT_TWO_SIZE            13
#define SETTING_DEF_TAB_SIZE_PX                 40


class NPSettings : public QObject
{
    Q_OBJECT
public:
    NPSettings(QObject *parent);

    QDir *GetRootFolder() const;
    QString GetFont() const;
    bool IsTabsAsSpaces() const; /*is that lolcat speak?*/
    int GetSaveInterval() const;
    int GetTabSize() const;
    QString GetTrashFolder() const;
    QString GetQuickFolder() const;

    QString GetDateFormat() const;
    QString GetDateStampFormat() const;
    QString GetFontOne() const;
    QString GetFontTwo() const;

    int GetHardwrapLength() const;
    int GetFontOneSize() const;
    int GetFontTwoSize() const;
    int GetTabSizePx() const;

    void SetRootFolder(QDir *folder);
    void SetFont(QString font);
    void SetTabsAsSpaces(bool is);
    void SetSaveInterval(int interval);
    void SetTabSize(int size);
    void SetTrashFolder(QString folder);
    void SetQuickFolder(QString folder);

    void setDateFormat(QString format);
    void SetDateStampFormat(QString format);
    void SetFontOne(QString fontName);
    void SetFontTwo(QString fontName);

    void SetHardwrapLength(int length);
    void SetFontOneSize(int size);
    void SetFontTwoSize(int size);
    void SetTabSizePx(int size);

private:

    QSettings *settings;
    QDir *root_folder;

};

#endif // NPSETTINGS_H
