#include "languages.h"

namespace ui
{

static const QString TRANSLATION_ROOT = "/Duokan";

bool loadTranslator(const QString &locale)
{
    // Not sure it will cause memory leak.
    static QTranslator *pre_translator;
    QTranslator *translator = new QTranslator;
    QString file = "duokan_" + locale + ".qm";

    // Double check.
    QDir dir(QDir::home());
    QString path = TRANSLATION_ROOT;
    if (!path.isEmpty())
    {
        dir.cd(path);
    }

    if (!dir.cd("translations"))
    {
        return false;
    }

    qDebug("Locale file %s", qPrintable(dir.absoluteFilePath(file)));
    translator->load(dir.absoluteFilePath(file));

    if (pre_translator)
    {
        QApplication::removeTranslator(pre_translator);
    }
    if (locale == "iw_IL")
    {
        qApp->setLayoutDirection(Qt::RightToLeft);
    } else {
        qApp->setLayoutDirection(Qt::LeftToRight);
    }

    QApplication::installTranslator(translator);
    pre_translator = translator;
    return true;
}

}

