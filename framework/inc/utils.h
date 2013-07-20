#ifndef DUOKAN_UTILS_H_
#define DUOKAN_UTILS_H_

#include <QtGui/QtGui>
#include <QtSql/QtSql>

namespace framework
{

// Return the screen geometry when transform.
QRect screenGeometry();

bool dockWidget(QWidget *target, QWidget * container, Qt::Alignment align);

int statusBarHeight();

int defaultFontPointSize();

QPoint globalTopLeft(QWidget *);

QPoint globalCenter(QWidget *);

int distance(QWidget * first, QWidget *second);

int distance(QPoint first, QPoint second);

int keyboardKeyHeight();

// the width of the check-box in CheckBoxView
int checkBoxViewWidth();

// If the parent_widget is null, return the desktop widget.
QWidget * safeParentWidget(QWidget *parent_widget);

// calculates the best size for dialog
QSize bestDialogSize();

QString sizeString(int size);

bool isLandscapeVolumeMapping();


/// Page turning direction configuration. The settings are defined
/// as following:
/// >0 From left to right or bottom to top.
/// == 0 Don't distance is too small.
/// <0 From right to left or top to bottom.
class PageTurningConfig
{
public:
    PageTurningConfig();
    ~PageTurningConfig();

public:
    static bool makeSureTableExist(QSqlDatabase& db);
    static bool load(QSqlDatabase& db);
    static bool save(QSqlDatabase& db);

    static int direction(const QPoint & old_position, const QPoint & new_position);
    static void setDirection(int);
    static int distance();
    static int whichArea(const QPoint & old_position, const QPoint & new_position);

private:
    static int direction_;
    static int THRESHOLD;
};

};

#endif
