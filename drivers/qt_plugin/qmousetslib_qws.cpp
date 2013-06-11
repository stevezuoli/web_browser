/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qmousetslib_qws.h"

#if !defined(QT_NO_QWS_MOUSE_TSLIB) || defined(QT_PLUGIN)
#include "qwindowsystem_qws.h"
#include <QtCore/qregexp.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qprocess.h>
#include "qsocketnotifier.h"
#include "qscreen_qws.h"

#include <errno.h>
#include <fcntl.h>
#include <linux/input.h>

QT_BEGIN_NAMESPACE

#ifdef TSLIBMOUSEHANDLER_DEBUG
#  include <QtCore/QDebug>
#endif

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
#define POS_TH 10

class Device
{
public:
    class Properties {
    public:
        int width;
        int height;
        int dpi;
        bool hasKeyboard;
        bool hasFiveWay;
    };

    static Device& instance();

    typedef enum MODEL {
        UNKNOWN = 0,
        EMULATOR, // qvfb qws environment
        K2,       // Kindle 2
        KDX,      // Kindle DX(G)
        K3,       // Kindle Keyboard
        K4NT,     // Kindle Non-Touch Silver 2011
        K4NTB,    // Kindle Non-Touch Black 2012
        KT,       // Kindle Touch
        KPW       // Kindle PaperWhite
    } Model;

    static Model getModel() {
        return m_model;
    }

    static int getWidth() {
        return PROPS[m_model].width;
    }

    static int getHeight() {
        return PROPS[m_model].height;
    }

    static int getDpi() {
        return PROPS[m_model].dpi;
    }

    static bool hasKeyboard() {
        return PROPS[m_model].hasKeyboard;
    }

    static bool hasFiveWay() {
        return PROPS[m_model].hasFiveWay;
    }

    static bool isTouch() {
        return !hasKeyboard() && !hasFiveWay();
    }

    static bool hasLight() {
        return m_model == KPW;
    }

    static bool isEmulator() { return m_model == EMULATOR; }

private:
    static const Properties PROPS[];
    static MODEL m_model;

    Device();
    Device(Device const&);
    void operator=(Device const&);
};

const Device::Properties Device::PROPS[] = {
//    x    y    DPI, KBD    JOY
    {600, 800,  167, false, false},    // UNKNOWN
    {600, 800,  167, true,  true },    // EMULATOR
    {600, 800,  167, true,  true },    // K2
    {824, 1200, 152, true,  true },    // KDX
    {600, 800,  167, true,  true },    // K3
    {600, 800,  167, false, true },    // K4NT
    {600, 800,  167, false, true },    // K4NTB
    {600, 800,  167, false, false},    // KT
    {758, 1024, 212, false, false},    // KPW
};

Device::Model Device::m_model = UNKNOWN;

Device::Device()
{
#ifdef i386
    m_model = EMULATOR;
    return;
#endif
    QStringList list;
    QProcess *myProcess = new QProcess();

    list << "-c" << "grep erial /proc/cpuinfo|cut -c12-15";
    myProcess->start("/bin/sh", list);
    if (myProcess->waitForReadyRead(10000)) {
        QByteArray array = myProcess->readAll();
        array.truncate(array.indexOf("\n"));

        bool ok;
        int sn = QString(array).toInt(&ok, 16);

        if (ok) {
            qDebug("serial: %X", sn);
        } else {
            qDebug("unexpected output");
            return;
        }

        switch(sn) {
        case 0xB002:
        case 0xB003:
            m_model = K2; // may not work as K2 doesn't print SN in cpuinfo
            break;
        case 0xB004:
        case 0xB005:
        case 0xB009:
            m_model = KDX;
            break;
        case 0xB006:
        case 0xB008:
        case 0xB00A:
            m_model = K3;
            break;
        case 0xB00E:
            m_model = K4NT;
            break;
        case 0xB00F:
        case 0xB010:
        case 0xB011:
        case 0xB012: // ???
            m_model = KT;
            break;
        case 0x9023:
        case 0xB023:
            m_model = K4NTB;
            break;
        case 0xB01B:
        case 0xB01C:
        case 0xB01D:
        case 0xB01F:
        case 0xB024:
            m_model = KPW;
            break;
        default:
            qDebug("Unknown model: %X", sn);
        }
    }
    myProcess->close();
}

Device &Device::instance()
{
    static Device instance;
    // qDebug("DEVICE: %d (%d x %d @ %d) [%d|%d]", getModel(), getWidth(), getHeight(), getDpi(), hasKeyboard(), hasFiveWay());
    return instance;
}


/*!
    \internal

    \class QWSTslibMouseHandler
    \ingroup qws

    \brief The QWSTslibMouseHandler class implements a mouse driver
    for the Universal Touch Screen Library, tslib.

    QWSTslibMouseHandler inherits the QWSCalibratedMouseHandler class,
    providing calibration and noise reduction functionality in
    addition to generating mouse events, for devices using the
    Universal Touch Screen Library.

    To be able to compile this mouse handler, \l{Qt for Embedded Linux}
    must be configured with the \c -qt-mouse-tslib option, see the
    \l{Pointer Handling} documentation for details. In addition, the tslib
    headers and library must be present in the build environment.  The
    tslib sources can be downloaded from \l
    {http://tslib.berlios.de/}.  Use the \c -L and \c -I options
    with \c configure to explicitly specify the location of the
    library and its headers:

    \snippet doc/src/snippets/code/src_gui_embedded_qmousetslib_qws.cpp 0

    In order to use this mouse handler, tslib must also be correctly
    installed on the target machine. This includes providing a \c
    ts.conf configuration file and setting the necessary environment
    variables, see the README file provided with tslib for details.

    The ts.conf file will usually contain the following two lines

    \snippet doc/src/snippets/code/src_gui_embedded_qmousetslib_qws.cpp 1

    To make \l{Qt for Embedded Linux} explicitly choose the tslib mouse
    handler, set the QWS_MOUSE_PROTO environment variable.

    \sa {Pointer Handling}, {Qt for Embedded Linux}
*/

class QWSTslibMouseHandlerPrivate : public QObject
{
    Q_OBJECT
public:
    QWSTslibMouseHandlerPrivate(QWSTslibMouseHandler *h,
                                const QString &device);
    ~QWSTslibMouseHandlerPrivate();

    void suspend();
    void resume();

private slots:
    void activity(int);
    void kt_activity(int);

private:
    QWSTslibMouseHandler *handler;

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

    QPoint p, oldP ;

    bool input_captured ;

    void capture_input(void) ;
    void release_input(void) ;

    bool isKT;
};

QWSTslibMouseHandlerPrivate::QWSTslibMouseHandlerPrivate(QWSTslibMouseHandler *h,
                                                         const QString &device)
    : handler(h)

{
    _debug = device.contains("debug", Qt::CaseInsensitive);

    Device::instance();
    isKT = Device::getModel() == Device::KT;

    width = Device::getWidth();
    height = Device::getHeight();

    if (isKT) {
        _fd = open("/dev/input/event3", O_RDONLY);
        // Kindle Touch Home button
        kt_fd = open("/dev/input/event2", O_RDONLY);
        kt_sn = new QSocketNotifier(kt_fd, QSocketNotifier::Read);
        connect(kt_sn, SIGNAL(activated(int)), this, SLOT(kt_activity(int)));
        kt_sn->setEnabled(true);
    } else {
        _fd = open("/dev/input/event0", O_RDONLY);
    }

    if (_fd == -1)
        qDebug("Open touch failed!");
    else
        qDebug("Open touch succeeded!");

    if (kt_fd == -1)
        qDebug("Open Keyboard failed!");
    else
        qDebug("Open Keyboard succeeded!");

    _sn = new QSocketNotifier(_fd, QSocketNotifier::Read);

    connect(_sn, SIGNAL(activated(int)), this, SLOT(activity(int)));

    _sn->setEnabled(true);

    p.setX(0); p.setY(0);
    oldP.setX(0); oldP.setY(0);
    touch = newtouch = false ;
    doubletap = newdoubletap = false ;

    oldX=0; oldY=0;

    capture_input() ;
}

QWSTslibMouseHandlerPrivate::~QWSTslibMouseHandlerPrivate()
{
    release_input() ;
    delete _sn;
    close(_fd);
    if (isKT) {
        delete kt_sn;
        close(kt_fd);
    }
}

void QWSTslibMouseHandlerPrivate::suspend()
{
    _sn->setEnabled(false);
    if (isKT) kt_sn->setEnabled(false);
}

void QWSTslibMouseHandlerPrivate::resume()
{
    _sn->setEnabled(true);
    if (isKT) kt_sn->setEnabled(true);
}

typedef struct input_event input_event_t;

void QWSTslibMouseHandlerPrivate::kt_activity(int)
{
    kt_sn->setEnabled(false);

    input_event_t in;

    read(kt_fd, &in, sizeof(input_event_t));

    if (in.type == 1)
    {
        if (_debug)
        {
            QString debugText = QString("Keyboard: type %1, code %2, value %3").arg(in.type).arg(in.code).arg(in.value) ;
            qDebug("%s", (const char*)debugText.toAscii());
        }

        if (in.code == KT_HOME) {
            QWSServer::sendKeyEvent(0, Qt::Key_Home, Qt::NoModifier, in.value != 0, in.value != 2);
        }
    }

    kt_sn->setEnabled(true);
}

void QWSTslibMouseHandlerPrivate::activity(int)
{
    _sn->setEnabled(false);
    int pos;
    input_event_t in; ssize_t size ;
    size = read(_fd, &in, sizeof(input_event_t));
    if (_debug) qDebug("TS data: type %X, code %X, value %d", in.type, in.code, in.value);

    switch(in.type)
    {
    case EV_ABS:
        pos = in.value;
        if (isKT) {
            pos = (in.code == ABS_MT_POSITION_X ? width : height) * (in.value) / 0x1000;
        }

        if (in.code == ABS_MT_POSITION_X && abs(oldX - pos) > POS_TH) {
            p.setX(pos);
            oldX = pos;
        }  else if (in.code == ABS_MT_POSITION_Y && abs(oldY - pos) > POS_TH) {
            p.setY(pos);
            oldY = pos;
        }
        break ;
    case EV_KEY:
        if (in.code == BTN_TOUCH)
        {
            newtouch = (in.value == 0) ? false : true ;
            handler->mouseChanged(p, (newtouch) ? Qt::LeftButton : 0, 0);
        }
        else if (in.code == BTN_TOOL_DOUBLETAP)
        {
            newdoubletap = (in.value == 0) ? false : true ;
            handler->mouseChanged(p, (newdoubletap) ? Qt::RightButton : 0, 0);
        }
        break ;
    case EV_SYN:
        // swipe
        if (!doubletap && !newdoubletap && touch && newtouch && p != oldP)
            handler->mouseChanged(p, Qt::LeftButton, 0);
        oldP.setX(p.x());
        oldP.setY(p.y());
        touch = newtouch ;
        doubletap = newdoubletap ;
        break ;
    default:
        break ;
    }

    if (_debug)
    {
        qDebug("x=%d, y=%d, touch=%d %d, doubletap=%d %d", p.x(), p.y(), touch, newtouch, doubletap, newdoubletap);
        if (in.type == EV_SYN)
            qDebug("________________");
    }

    _sn->setEnabled(true);
}

void QWSTslibMouseHandlerPrivate::capture_input(void)
{
    int on = 1 ;

    if (!input_captured )
    {
        if (_debug) qDebug("attempting to capture input...");

        if (_fd != -1)
        {
            if (ioctl(_fd, EVIOCGRAB, on)) {
                qDebug("Capture touch input: error");
            }
        }

        if (isKT && kt_fd != -1)
        {
            if (ioctl(kt_fd, EVIOCGRAB, on)) {
                qDebug("Capture touch input: error");
            }
        }

        input_captured = true ;
    }
}

void QWSTslibMouseHandlerPrivate::release_input(void)
{
    int off = 0 ;

    if (input_captured )
    {
        if (_debug) qDebug("attempting to release input...");
        if (_fd != -1)
        {
            if (ioctl(_fd, EVIOCGRAB, off)) {
                qDebug("Release touch input: error");
            }
        }

        if (isKT && kt_fd != -1)
        {
            if (ioctl(kt_fd, EVIOCGRAB, off)) {
                    qDebug("Release kbd input: error");
            }
        }

        input_captured = false ;
    }
}

/*!
    \internal
*/
QWSTslibMouseHandler::QWSTslibMouseHandler(const QString &driver,
                                           const QString &device)
    : QWSMouseHandler(driver, device)
{
    d = new QWSTslibMouseHandlerPrivate(this, device);
}

/*!
    \internal
*/
QWSTslibMouseHandler::~QWSTslibMouseHandler()
{
    delete d;
}

/*!
    \reimp
*/
void QWSTslibMouseHandler::suspend()
{
    d->suspend();
}

/*!
    \reimp
*/
void QWSTslibMouseHandler::resume()
{
    d->resume();
}

QT_END_NAMESPACE

#include "qmousetslib_qws.moc"

#endif //QT_NO_QWS_MOUSE_TSLIB
