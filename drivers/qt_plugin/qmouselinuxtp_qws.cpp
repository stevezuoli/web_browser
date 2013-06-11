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

#include "qmouselinuxtp_qws.h"

#ifndef QT_NO_QWS_MOUSE_LINUXTP
#include "qwindowsystem_qws.h"
#include "qsocketnotifier.h"
#include "qtimer.h"
#include "qapplication.h"
#include "qscreen_qws.h"
#include <private/qcore_unix_p.h> // overrides QT_OPEN

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

#include <linux/input.h>
#include "keycodes.h"

QT_BEGIN_NAMESPACE

class QWSLinuxTPMouseHandlerPrivate : public QObject
{
    Q_OBJECT
public:
    QWSLinuxTPMouseHandlerPrivate(QWSLinuxTPMouseHandler *h, const QString &);
    ~QWSLinuxTPMouseHandlerPrivate();

    void suspend();
    void resume();

private Q_SLOTS:
    void activity(int);
    void setKeypadMode(bool keypadMode);

private:
    QWSLinuxTPMouseHandler* _parent;
    int _fd;
    QSocketNotifier* _sn;

    bool _debug;
    bool _keypadMode;
    int _up, _down, _left, _right, _button;
};

QWSLinuxTPMouseHandler::QWSLinuxTPMouseHandler(const QString &driver, const QString &device)
    : QWSMouseHandler(driver, device)
{
    d = new QWSLinuxTPMouseHandlerPrivate(this, device);
}

QWSLinuxTPMouseHandler::~QWSLinuxTPMouseHandler()
{
    delete d;
}

void QWSLinuxTPMouseHandler::suspend()
{
    d->suspend();
}

void QWSLinuxTPMouseHandler::resume()
{
    d->resume();
}

QWSLinuxTPMouseHandlerPrivate::QWSLinuxTPMouseHandlerPrivate(QWSLinuxTPMouseHandler *h,
        const QString &device)
        : _parent(h)
{
    _debug = device.contains("debug", Qt::CaseInsensitive);
    //qDebug("Kindle five way keyboard(%s)", (const char*)device.toAscii());

    _up = 0;
    _down = 0;
    _left = 0;
    _right = 0;
    _button = 0;
    _keypadMode = false;

    if (device.contains("keypad", Qt::CaseInsensitive))
    {
        if (_debug)
            qDebug("KEYPAD mode flag IGNORED");
        setKeypadMode(true);
    }
    else if (_debug)
        qDebug("KEYPAD mode OFF (%s)", (const char*)device.toAscii());

    _fd = open("/dev/input/event1", O_RDONLY);

    if (_debug)
    {
        if (_fd == -1)
            qDebug("event1 open FAILED");
        else
            qDebug("event1 open SUCCESS");
    }

    _sn = new QSocketNotifier(_fd, QSocketNotifier::Read);

    connect(_sn, SIGNAL(activated(int)), this, SLOT(activity(int)));

    _sn->setEnabled(true);
}

QWSLinuxTPMouseHandlerPrivate::~QWSLinuxTPMouseHandlerPrivate()
{
    if (_debug) qDebug("Delete linux TP");
    delete _sn;
    close(_fd);
}

void QWSLinuxTPMouseHandlerPrivate::suspend()
{
    if (_debug) qDebug("Suspend linux TP");
    _sn->setEnabled(false);
}

void QWSLinuxTPMouseHandlerPrivate::resume()
{
    if (_debug) qDebug("Resume linux TP");
    _sn->setEnabled(true);
}

void QWSLinuxTPMouseHandlerPrivate::setKeypadMode(bool keypadMode)
{
    if (_debug) qDebug("setKeypadMode:%d", keypadMode? 1 : 0);
    _keypadMode = keypadMode ; //keypadMode;

    if (QScreenCursor::instance() != 0)
    {
        if (_keypadMode)
        {
            QScreenCursor::instance()->hide();
            QWSServer::setCursorVisible(false);
            _parent->mouseChanged(QPoint(0,1200), 0, 0);
        }
        else
        {
            QScreenCursor::instance()->show();
            QWSServer::setCursorVisible(true);
            _parent->mouseChanged(_parent->pos(), 0, 0);
        }
    }
}

typedef struct input_event input_event_t;

void QWSLinuxTPMouseHandlerPrivate::activity(int)
{
    _sn->setEnabled(false);

    input_event_t in;

    read(_fd, &in, sizeof(input_event_t));

    //qDebug("FiveWay: type %d, code %d, value %d", in.type, in.code, in.value);

    if (in.type == 1)
    {
        QPoint p = _parent->pos();

        switch(in.code)
        {
        default:
            break;
        case KDX_KEY_5WPRESS:
        case K3_KEY_5WPRESS:
            // button press
            if (_keypadMode)
            {
                QWSServer::sendKeyEvent(0, Qt::Key_F1, Qt::NoModifier, in.value != 0, in.value == 2);
            }
            else
            {
                if (in.value)
                    _button = !_button;
            }
            break;
        case KDX_KEY_LARROW:
            // left
            if (_keypadMode)
            {
                QWSServer::sendKeyEvent(0, Qt::Key_Left, Qt::NoModifier, in.value != 0, in.value == 2);
            }
            else if (in.value == 0)
            {
                _left = 0;
            }
            else if (in.value == 1)
            {
                _left = 1;
                p.setX(p.x() - 5);
            }
            else
            {
                if (_left < 6)
                    _left++;

                p.setX(p.x() - 5 * _left);
            }
            break;
        case KDX_KEY_RARROW:
            // right
            if (_keypadMode)
            {
                QWSServer::sendKeyEvent(0, Qt::Key_Right, Qt::NoModifier, in.value != 0, in.value == 2);
            }
            else if (in.value == 0)
            {
                _right = 0;
            }
            else if (in.value == 1)
            {
                _right = 1;
                p.setX(p.x() + 5);
            }
            else
            {
                if (_right < 6)
                    _right++;

                p.setX(p.x() + 5 * _right);
            }
            break;
        case KDX_KEY_UPARROW:
        case K3_KEY_UPARROW:
            // up
            if (_keypadMode)
            {
                QWSServer::sendKeyEvent(0, Qt::Key_Up, Qt::NoModifier, in.value != 0, in.value == 2);
            }
            else if (in.value == 0)
            {
                _up = 0;
            }
            else if (in.value == 1)
            {
                _up = 1;
                p.setY(p.y() - 5);
            }
            else
            {
                if (_up < 6)
                    _up++;

                p.setY(p.y() - 5 * _up);
            }
            break;
        case KDX_KEY_DNARROW:
        case K3_KEY_DNARROW:
            // down
            if (_keypadMode)
            {
                QWSServer::sendKeyEvent(0, Qt::Key_Down, Qt::NoModifier, in.value != 0, in.value == 2);
            }
            else if (in.value == 0)
            {
                _down = 0;
            }
            else if (in.value == 1)
            {
                _down = 1;
                p.setY(p.y() + 5);
            }
            else
            {
                if (_down < 6)
                    _down++;

                p.setY(p.y() + 5 * _down);
            }
            break;
        }

        _parent->limitToScreen(p);

        if (!_keypadMode)
        {
            _parent->mouseChanged(p, _button ? Qt::LeftButton : 0, 0);
        }
    }

    _sn->setEnabled(true);
}

QT_END_NAMESPACE

#include "qmouselinuxtp_qws.moc"

#endif //QT_NO_QWS_MOUSE_LINUXTP
