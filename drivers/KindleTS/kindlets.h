#ifndef KINDLETS_H
#define KINDLETS_H

#include <QWSMouseHandler>
#include <QSocketNotifier>
#include <QWSServer>
#include <QtCore>

#ifdef BUILD_FOR_ARM
#include <fcntl.h>
#include <linux/input.h>
#endif

#include "Device/device.h"
#include "touch_event.h"

#define EV_SYN			0x00
#define EV_KEY			0x01
#define EV_REL			0x02
#define EV_ABS			0x03

#define ABS_MT_POSITION_X	0x35	/* Center X ellipse position */
#define ABS_MT_POSITION_Y	0x36	/* Center Y ellipse position */

#define BTN_TOOL_FINGER		0x145
#define BTN_TOUCH		0x14a
#define BTN_TOOL_DOUBLETAP	0x14d

#define KT_HOME 102

#ifndef ABS_MT_SLOT
#define ABS_MT_SLOT 0x2F
#endif

#ifndef ABS_MT_TRACKING_ID
#define ABS_MT_TRACKING_ID 0x39
#endif

#define POS_TH 10

typedef struct input_event input_event_t;

struct input_data
{
    int type;
    int code;
    int value;
};

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Gui)

class KindleTS : public QObject
#ifdef BUILD_FOR_ARM
                 , public QWSMouseHandler
#endif
{
    Q_OBJECT
public:
    explicit KindleTS(const QString & driver = QString(), const QString & device = QString());
    virtual ~KindleTS();

    virtual void suspend();
    virtual void resume();

Q_SIGNALS:
    void touchEvent(TouchEvent* te);

private slots:
    void activity(int);
    void kt_activity(int);

private:
    TouchEvent* readTouchEvent(int _touch_device);
    void readSingleTouchEvent(const input_data& in); // deprecated
    void capture_input(void);
    void release_input(void);

private:
    int width, height;

    int _fd;
    QSocketNotifier* _sn;

    int kt_fd;
    QSocketNotifier* kt_sn;

    bool _debug;

    bool touch ;
    bool doubletap ;

    bool newtouch ;
    bool newdoubletap ;

    int oldX, oldY;

    QPoint p, oldP;
    bool input_captured;
    int  current_slot_id;
    int  current_pointer_count;
    int  last_action_masked;
    bool isKT;

    QVector<input_data> event_stack_;
};

QT_END_NAMESPACE

QT_END_HEADER

#endif // KINDLETS_H
