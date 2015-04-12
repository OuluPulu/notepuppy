#include "npsettings.h"
#include "npfile.h"
#include <QDir>

NPSettings::NPSettings(QObject *parent):
        QObject(parent)
{
    settings = new QSettings(SETTING_COMPANY_INTERNAL, SETTING_APP_NAME, parent);

#ifdef ANDROID
// first look for the sdcard
    QDir home("/sdcard");
    if (!home.exists())
    {
        home.setPath("/mnt/sdcard");
        if (!home.exists())
        {
            home.setPath("/");
        }
    }

    QString root_folder_string = home.absolutePath();

#elif defined(Q_OS_SYMBIAN)

    QString root_folder_string = "E:\\";
    QDir e(root_folder_string);
    if (!e.exists())
    {
        root_folder_string = "C:\\";
    }
#else

    //    root_folder = QDir::home();
    QString root_folder_string = QDir::home().absolutePath();
#endif
    root_folder_string.append(FSEP);
    root_folder_string.append(SETTING_DEF_ROOT_FOLDER);
    QString rootf = this->settings->value(SETTING_DY_ROOT_FOLDER, root_folder_string).toString();
    this->root_folder = new QDir(rootf);
    if (!root_folder->exists())
    {
        root_folder->mkpath(rootf);
    }
}

QDir *NPSettings::GetRootFolder() const {
    return root_folder;
}

QString NPSettings::GetFont() const {
       return settings->value(SETTING_DY_FONT, SETTING_DEF_FONT).toString();
}

bool NPSettings::IsTabsAsSpaces() const {
    return this->settings->value(SETTING_DY_TABS_AS_SPACES, SETTING_DEF_TABS_AS_SPACES).toBool();
}

int NPSettings::GetSaveInterval() const {
    return this->settings->value(SETTING_DY_SAVE_INTERVAL, SETTING_DEF_SAVE_INTERVAL).toInt();
}

int NPSettings::GetTabSize() const {
    return this->settings->value(SETTING_DY_TAB_SIZE, SETTING_DEF_TAB_SIZE).toInt();
}

QString NPSettings::GetTrashFolder() const
{
    static bool setup = false;
    static QString trsh = "";
    if (!setup)
    {
        trsh = QDir::homePath();
        trsh += FSEP;
        trsh += this->settings->value(SETTING_DY_TRASH_DIR, SETTING_DEF_TRASH_DIR).toString();
        setup = true;
    }
    return trsh;
}

QString NPSettings::GetQuickFolder() const
{
    static bool setup = false;
    static QString quck = "";
    if (!setup)
    {
        quck.append(this->GetRootFolder()->absolutePath());
        quck.append(FSEP);
        quck.append(settings->value(SETTING_DY_QUICK_DIR, SETTING_DEF_QUICK_DIR).toString());

        QDir dr(quck);
        if (!dr.exists())
        {
            dr.mkpath(quck);
        }
        setup = true;
    }
    return quck;
}

void NPSettings::SetRootFolder(QDir *folder) {
    this->root_folder = new QDir(folder->canonicalPath());
    settings->setValue(SETTING_DY_ROOT_FOLDER, folder->canonicalPath());
}

void NPSettings::SetFont(QString font) {
    settings->setValue(SETTING_DY_FONT, font);
}

void NPSettings::SetTabsAsSpaces(bool is) {
    settings->setValue(SETTING_DY_TAB_SIZE, is);
}

void NPSettings::SetSaveInterval(int interval) {
    settings->setValue(SETTING_DY_SAVE_INTERVAL, interval);
}

void NPSettings::SetTabSize(int size) {
    settings->setValue(SETTING_DY_TAB_SIZE, size);
}

void NPSettings::SetTrashFolder(QString folder)
{
    settings->setValue(SETTING_DY_TRASH_DIR, folder);
}

void NPSettings::SetQuickFolder(QString folder)
{
    settings->setValue(SETTING_DY_QUICK_DIR, folder);
}

QString NPSettings::GetDateFormat() const
{
    return settings->value(SETTING_DY_STR_DATE_FORMAT, SETTING_DEF_STR_DATE_FORMAT).toString();
}

QString NPSettings::GetDateStampFormat() const
{
    return settings->value(SETTING_DY_STR_DATE_STAMP_FORMAT, SETTING_DEF_STR_DATE_STAMP_FORMAT).toString();
}

QString NPSettings::GetFontOne() const
{
    return settings->value(SETTING_DY_STR_FONT_ONE, SETTING_DEF_STR_FONT_ONE).toString();
}

QString NPSettings::GetFontTwo() const
{
    return settings->value(SETTING_DY_STR_FONT_TWO, SETTING_DEF_STR_FONT_TWO).toString();
}


int NPSettings::GetHardwrapLength() const
{
    return settings->value(SETTING_DY_IN_HARDWRAP_LENGTH, SETTING_DEF_IN_HARDWRAP_LENGTH).toInt();
}

int NPSettings::GetFontOneSize() const
{
    return settings->value(SETTING_DY_IN_FONT_ONE_SIZE, SETTING_DEF_IN_FONT_ONE_SIZE).toInt();
}

int NPSettings::GetFontTwoSize() const
{
    return settings->value(SETTING_DY_IN_FONT_TWO_SIZE, SETTING_DEF_IN_FONT_TWO_SIZE).toInt();
}

int NPSettings::GetTabSizePx() const
{
    return settings->value(SETTING_DY_TAB_SIZE_PX, SETTING_DEF_TAB_SIZE_PX).toInt();
}

void NPSettings::setDateFormat(QString format)
{
    settings->setValue(SETTING_DY_STR_DATE_FORMAT, format);
}

void NPSettings::SetDateStampFormat(QString format)
{
    settings->setValue(SETTING_DY_STR_DATE_STAMP_FORMAT, format);
}

void NPSettings::SetFontOne(QString fontName)
{
    settings->setValue(SETTING_DY_STR_FONT_ONE, fontName);
}

void NPSettings::SetFontTwo(QString fontName)
{
    settings->setValue(SETTING_DY_STR_FONT_TWO, fontName);
}

void NPSettings::SetHardwrapLength(int length)
{
    settings->setValue(SETTING_DY_IN_HARDWRAP_LENGTH, length);
}

void NPSettings::SetFontOneSize(int size)
{
    settings->setValue(SETTING_DY_IN_FONT_ONE_SIZE, size);
}

void NPSettings::SetFontTwoSize(int size)
{
    settings->setValue(SETTING_DY_IN_FONT_TWO_SIZE, size);
}

void NPSettings::SetTabSizePx(int size)
{
    settings->setValue(SETTING_DY_TAB_SIZE_PX, size);
}
