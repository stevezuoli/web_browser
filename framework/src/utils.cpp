#include "utils.h"
#include "System/inc/system_manager.h"

namespace framework
{

QRect screenGeometry()
{
    QRect rc = QApplication::desktop()->geometry();
    int def_rotation = 0;//sys::defaultRotation();
    int r1 = (def_rotation + 90) % 360;
    int r2 = (def_rotation + 270) % 360;
    if (SystemManager::instance()->screenTransformation() == r1 ||
        SystemManager::instance()->screenTransformation() == r2)
    {
        int w = rc.width();
        rc.setWidth(rc.height());
        rc.setHeight(w);
    }
    return rc;
}

bool dockWidget(QWidget *target, QWidget * container, Qt::Alignment align)
{
    return true;
}

int statusBarHeight()
{
    return 60;
}

int defaultFontPointSize()
{
    return 20;
}

QPoint globalTopLeft(QWidget *wnd)
{
    return wnd->mapToGlobal(QPoint());
}

QPoint globalCenter(QWidget *wnd)
{
    return QRect(wnd->mapToGlobal(QPoint()), wnd->size()).center();
}

/// Return the distance between first and second widget.
int distance(QWidget * first, QWidget *second)
{
    QPoint first_pt = globalCenter(first);
    QPoint second_pt = globalCenter(second);
    return distance(first_pt, second_pt);
}

int distance(QPoint first_pt, QPoint second_pt)
{
    int x = (second_pt.x() - first_pt.x()) * (second_pt.x() - first_pt.x());
    int y = (second_pt.y() - first_pt.y()) * (second_pt.y() - first_pt.y());
    return static_cast<int>(sqrt(static_cast<float>(x) + static_cast<float>(y)));
}

int keyboardKeyHeight()
{
    return 50;
}

int checkBoxViewWidth()
{
    return 18;
}

QWidget * safeParentWidget(QWidget *parent_widget)
{
    QWidget * safe_ptr = parent_widget;
    if (0 == safe_ptr)
    {
        safe_ptr = qApp->desktop();
    }
    return safe_ptr;
}

QSize bestDialogSize()
{
    int width = QApplication::desktop()->screenGeometry().width() * 2 / 3;
    int height = QApplication::desktop()->screenGeometry().height() * 4 / 5;
    return QSize(width, height);
}

QString sizeString(int size)
{
    QString string;
    if (size > 1024 * 1024 * 1024)
    {
        QString tmp("%1.%2 %3");
        int gb = size >> 30;
        int left = ((size - (gb << 30)) * 10) >> 30;
        string = tmp.arg(gb).arg(left).arg(QApplication::tr("GB"));
    }
    else if (size > 1024 * 1024)
    {
        QString tmp("%1.%2 %3");
        int mb = size >> 20;
        int left = ((size - (mb << 20)) * 10) >> 20;
        string = tmp.arg(mb).arg(left).arg(QApplication::tr("MB"));
    }
    else if (size > 1024)
    {
        QString tmp("%1 %2");
        string = tmp.arg(size >> 10).arg(QApplication::tr("KB"));
    }
    else
    {
        QString tmp("%1 %2");
        string = tmp.arg(size).arg(QApplication::tr("Bytes"));
    }
    return string;
}

bool isLandscapeVolumeMapping()
{
    return qgetenv("LANDSCAPE_VOLUME_MAPPING").toInt() > 0;
}


int PageTurningConfig::direction_ = 1;
int PageTurningConfig::THRESHOLD = -1;

/// Page turning configuration. By default, from right to left or
/// from bottom to top is interpreted as next.
PageTurningConfig::PageTurningConfig()
{
}

PageTurningConfig::~PageTurningConfig()
{
}

bool PageTurningConfig::makeSureTableExist(QSqlDatabase& database)
{
    QSqlQuery query(database);
    return query.exec("create table if not exists page_turning_conf ("
                      "key text primary key, "
                      "value text "
                      ")");
}

bool PageTurningConfig::load(QSqlDatabase& db)
{
    // TODO.
    return true;
}

bool PageTurningConfig::save(QSqlDatabase& db)
{
    // TODO.
    return true;
}

int PageTurningConfig::direction(const QPoint & old_position, const QPoint & new_position)
{
    int delta_x = new_position.x() - old_position.x();
    int delta_y = new_position.y() - old_position.y();

    int dist = std::max(abs(delta_x), abs(delta_y));
    if (dist < distance())
    {
        return 0;
    }

    int delta = 0;
    if (abs(delta_x) > abs(delta_y))
    {
        delta = delta_x;
    }
    else
    {
        delta = delta_y;
    }

    if (delta < -distance())
    {
        return direction_;
    }
    else if (delta > distance())
    {
        return -direction_;
    }
    return 0;
}

void PageTurningConfig::setDirection(int conf)
{
    if (conf > 0)
    {
        direction_ = 1;
    }
    else if (conf < 0)
    {
        direction_ = -1;
    }
}

/// Return the distance threshold.
int PageTurningConfig::distance()
{
    if (THRESHOLD <= 0)
    {
        THRESHOLD = qgetenv("SWIPE_DISTANCE").toInt();
    }
    if (THRESHOLD <= 0)
    {
        THRESHOLD = 10;
    }
    return THRESHOLD;
}

int PageTurningConfig::whichArea(const QPoint & old_position, const QPoint & new_position)
{
    QRect screen = QApplication::desktop()->screenGeometry();
    int degree = 0;
#ifdef BUILD_FOR_ARM
    degree = QScreen::instance()->transformOrientation() * 90;
#endif
    if (degree == 90 || degree == 270)
    {
        screen.setSize(QSize(screen.height(), screen.width()));
    }

    if (new_position.x() < screen.width() / 3)
    {
        return -1;
    }
    else if (new_position.x() > screen.width() * 2 / 3)
    {
        return 1;
    }
    return 0;
}

}


