#include "kindlets.h"
#include <QDateTime>

QT_BEGIN_NAMESPACE

KindleTS::KindleTS(const QString & driver, const QString & device)

#ifdef BUILD_FOR_ARM
: QWSMouseHandler(driver, device),
#else
:
#endif
  input_captured(false),
  current_slot_id(0),
  current_pointer_count(0)
{
    _debug = device.contains("debug", Qt::CaseInsensitive);

    Device::instance();
    isKT = Device::getModel() == Device::KT;

    width = Device::getWidth();
    height = Device::getHeight();

    qDebug("KindleTS(%s, %s)", (const char*)driver.toAscii(), (const char*)device.toAscii());

#ifdef BUILD_FOR_ARM
    if (isKT) {
        _fd = open("/dev/input/event3", O_RDONLY|O_NOCTTY);
        // Kindle Touch Home button
        kt_fd = open("/dev/input/event2", O_RDONLY|O_NOCTTY);
        kt_sn = new QSocketNotifier(kt_fd, QSocketNotifier::Read);
        connect(kt_sn, SIGNAL(activated(int)), this, SLOT(kt_activity(int)));
        kt_sn->setEnabled(true);
    } else {
        _fd = open("/dev/input/event0", O_RDONLY);
    }

    if (_fd == -1)
        qDebug("TOUCH open FAILED");
    else
        qDebug("TOUCH open SUCCESS");

    if (kt_fd == -1)
        qDebug("KBD open FAILED");
    else
        qDebug("KBD open SUCCESS");

    _sn = new QSocketNotifier(_fd, QSocketNotifier::Read);

    connect(_sn, SIGNAL(activated(int)), this, SLOT(activity(int)));

    _sn->setEnabled(true);

    p.setX(0); p.setY(0);
    oldP.setX(0); oldP.setY(0);
    touch = newtouch = false ;
    doubletap = newdoubletap = false ;

    oldX=0; oldY=0;

    // Do not capture input when KindleApp exsits
    // It maybe a bug of touch driver. To refine it later.
    //capture_input();
#endif
}

KindleTS::~KindleTS()
{
#ifdef BUILD_FOR_ARM
    delete _sn;

    // Do not release input when KindleApp exsits
    // It maybe a bug of touch driver. To refine it later.
    //release_input();

    close(_fd);
    if (isKT) {
        delete kt_sn;
        close(kt_fd);
    }
#endif
}

void KindleTS::suspend()
{
#ifdef BUILD_FOR_ARM
    _sn->setEnabled(false);
    if (isKT) kt_sn->setEnabled(false);
#endif
}

void KindleTS::resume()
{
#ifdef BUILD_FOR_ARM
    _sn->setEnabled(true);
    if (isKT) kt_sn->setEnabled(true);
#endif
}

void KindleTS::kt_activity(int)
{
#ifdef BUILD_FOR_ARM
    kt_sn->setEnabled(false);

    input_event_t in;

    read(kt_fd, &in, sizeof(input_event_t));

    if (in.type == 1)
    {
        QString debugText = QString("Keyboard: type %1, code %2, value %3").arg(in.type).arg(in.code).arg(in.value) ;
        qDebug("%s", (const char*)debugText.toAscii());

        if (in.code == KT_HOME) {
            QWSServer::processKeyEvent(0, Qt::Key_Home, Qt::NoModifier, in.value != 0, in.value != 2);
        }
    }
    kt_sn->setEnabled(true);
#endif
}

void KindleTS::activity(int)
{
#ifdef BUILD_FOR_ARM
    _sn->setEnabled(false);

    TouchEvent* te = readTouchEvent(_fd);

    bool is_up = te != 0 ? (te->actionMasked() == TouchEvent::ACTION_UP) : false;
    if (is_up)
    {
        qDebug("Clear pointer count");
        current_pointer_count = 0;
    }

    if (te != 0 && te->pointerCount() >= 2)
    {
        qDebug("Emit multi-touch event, pos1(%d, %d), pos2(%d, %d), action:%d", te->x(0), te->y(0), te->x(1), te->y(1), te->action());
        emit touchEvent(te);
        delete te;
        _sn->setEnabled(true);
        return;   
    }

    if (te == 0 //EV_KEY
        || te->pointerCount() < 2)
    {
        // send single mouse event from event stack
        for (int i = 0; i < event_stack_.size(); i++)
        {
            readSingleTouchEvent(event_stack_[i]);
        }
        qDebug("Send Single Event-------------------");
    }

    if (te != 0)
        delete te;
    _sn->setEnabled(true);
#endif
}

void KindleTS::capture_input(void)
{
#ifdef BUILD_FOR_ARM
    int on = 1 ;
    if (!input_captured )
    {
        qDebug("attempting to capture input...");
        if (_fd != -1)
        {
            int touch_ret = ioctl(_fd, EVIOCGRAB, &on);
            if (touch_ret != 0) {
                qDebug("Capture touch input: %d", touch_ret);
            }
            else {
                qDebug("Capture touch input succeeded");
            }
        }

        if (isKT && kt_fd != -1)
        {
            int kt_ret = ioctl(kt_fd, EVIOCGRAB, &on);
            if (kt_ret != 0) {
                qDebug("Capture key input: %d", kt_ret);
            }
            else {
                qDebug("Capture key input succeeded");
            }
        }
        input_captured = true ;
    }
#endif
}

void KindleTS::release_input(void)
{
#ifdef BUILD_FOR_ARM
    if (input_captured )
    {
        qDebug("attempting to release input...");
        if (_fd != -1)
        {
            int touch_ret = ioctl(_fd, EVIOCGRAB, 0);
            if (touch_ret != 0) {
                qDebug("Release touch input: %d", touch_ret);
            }
            else {
                qDebug("Release touch input succeeded");
            }
        }

        if (isKT && kt_fd != -1)
        {
            int fd_ret = ioctl(kt_fd, EVIOCGRAB, 0);
            if (fd_ret != 0) {
                qDebug("Release kbd input: %d", fd_ret);
            }
            else {
                qDebug("Release kbd input succeeded");
            }
        }
        input_captured = false ;
    }
#endif
}

static qint64 getCurrentTimeInMsec()
{
    QDateTime current_time = QDateTime::currentDateTimeUtc();
    return current_time.toMSecsSinceEpoch();
}


void KindleTS::readSingleTouchEvent(const input_data& in)
{
#ifdef BUILD_FOR_ARM
    int pos;
    //qDebug("TS data: type %d, code %d, value %d", in.type, in.code, in.value);
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
            mouseChanged(p, (newtouch) ? Qt::LeftButton : 0, 0);
        }
        else if (in.code == BTN_TOOL_DOUBLETAP)
        {
            newdoubletap = (in.value == 0) ? false : true ;
            mouseChanged(p, (newdoubletap) ? Qt::RightButton : 0, 0);
        }
        break ;
    case EV_SYN:
        // swipe
        if (!doubletap && !newdoubletap && touch && newtouch && p != oldP)
        {
            mouseChanged(p, Qt::LeftButton, 0);
        }
        oldP.setX(p.x());
        oldP.setY(p.y());
        touch = newtouch ;
        doubletap = newdoubletap ;
        break ;
    default:
        break ;
    }

    //qDebug("Single Event x=%d, y=%d, touch=%d %d, doubletap=%d %d", p.x(), p.y(), touch, newtouch, doubletap, newdoubletap);
    //if (in.type == EV_SYN)
    //    qDebug("________________");
#endif
}

//maybe just update one of x/y
TouchEvent* KindleTS::readTouchEvent(int _touch_device)
{
#ifdef BUILD_FOR_ARM
    struct input_event ie_buf;
    TouchEvent te;
    int touch_action = TouchEvent::ACTION_MASK;
    int current_action_index = -1;
    int current_index = -1;
    static int s_oldX[2] = {-1, -1};
    static int s_oldY[2] = {-1, -1};
    static int index_to_slots[2] ={-1, -1};
    static qint64 down_time = -1;
    int read_count = 0;

    // clear event stack
    event_stack_.clear();

    for (;;)
    {
        if (read(_touch_device, &ie_buf, sizeof(ie_buf)) >0 )
        {
            // send single touch event at first
            input_data in;
            in.type = ie_buf.type;
            in.code = ie_buf.code;
            in.value = ie_buf.value;
            event_stack_.push_back(in);

            int position_value = ie_buf.value;
            if (EV_ABS == ie_buf.type)
            {
                if (isKT)
                {
                    position_value = (ie_buf.code == ABS_MT_POSITION_X ? width : height) * (ie_buf.value) / 0x1000;
                }
                ++read_count;
            }
            
            if (ie_buf.type == EV_ABS && ie_buf.code == ABS_MT_SLOT)
            {
                current_slot_id = ie_buf.value;
                //qDebug("current_slot_id = ie_buf.value; %d", ie_buf.value);
                continue;
            }
            if (ie_buf.type == EV_ABS && ie_buf.code == ABS_MT_TRACKING_ID && ie_buf.value >=0)
            {
                qDebug("ie_buf.type == EV_ABS && ie_buf.code == ABS_MT_TRACKING_ID && ie_buf.value >=0");
                ++current_index;
                index_to_slots[current_index] = current_slot_id;
                
                ++current_pointer_count;
                if (current_pointer_count > 2)
                {
                    current_pointer_count = 2;
                }
                
                qDebug("current_pointer_count at ie_buf.value >=0:%d", current_pointer_count);
                if (1 == current_pointer_count)
                {
                    touch_action = TouchEvent::ACTION_DOWN;
                    down_time = getCurrentTimeInMsec();
                }
                else if (2 == current_pointer_count)
                {
                    touch_action = TouchEvent::ACTION_POINTER_DOWN;
                }
                current_action_index = current_index;
            }

            if (ie_buf.type == EV_ABS && ie_buf.code == ABS_MT_TRACKING_ID && ie_buf.value ==-1)
            {
                qDebug("ie_buf.type == EV_ABS && ie_buf.code == ABS_MT_TRACKING_ID && ie_buf.value ==-1");
                ++current_index;
                index_to_slots[current_index] = current_slot_id;
                --current_pointer_count;
                if (current_pointer_count < 0)
                {
                    current_pointer_count = 0;
                }

                current_action_index = current_index;
                qDebug("current_pointer_count at ie_buf.value ==-1:%d", current_pointer_count);
                if (1 == current_pointer_count)
                {
                    touch_action = TouchEvent::ACTION_POINTER_UP;
                    // sometimes we get two ABS_MT_TRACKING_ID in one loop
                    // and must break it in two
                    break;
                }
                else if (0 == current_pointer_count)
                {
                    touch_action = TouchEvent::ACTION_UP;
                }
            }

            if (ie_buf.type == EV_ABS && ie_buf.code == ABS_MT_POSITION_X)
            {
                if (-1 == current_index || current_slot_id != index_to_slots[current_index])
                {
                    ++current_index;
                    index_to_slots[current_index] = current_slot_id;
                }
                if (-1 == current_action_index)
                {
                    current_action_index = current_index;
                }
                s_oldX[current_slot_id] = position_value;

                if (TouchEvent::ACTION_MASK == touch_action)
                {
                    touch_action = TouchEvent::ACTION_MOVE;
                }
            }

            if (ie_buf.type == EV_ABS && ie_buf.code == ABS_MT_POSITION_Y)
            {
                if (-1 == current_index || current_slot_id != index_to_slots[current_index])
                {
                    ++current_index;
                    index_to_slots[current_index] = current_slot_id;
                }
                if (-1 == current_action_index)
                {
                    current_action_index = current_index;
                }
                s_oldY[current_slot_id] = position_value;

                if (TouchEvent::ACTION_MASK == touch_action)
                {
                    touch_action = TouchEvent::ACTION_MOVE;
                }
                if (TouchEvent::ACTION_DOWN == touch_action)
                {
                    // 有可能action_down后还有一个action_pointer_down
                    // 需要拆成两个move event
                    break;
                }
            }

            if (ie_buf.type == EV_SYN && ie_buf.code == SYN_REPORT && ie_buf.value ==0)
            {
                break;
            }
        }
        else
        {
            break;
        }
    }

    // previous read bread before read (0, 0, 0)
    // currently only read (0, 0, 0)
    if (0 == read_count)
    {
        return 0;
    }

    te.pointer_count_ = current_index + 1;
    te.action_ = touch_action;
    te.down_time_ = down_time;
    for (int i = 0; i < te.pointer_count_; ++i)
    {
        te.x_[i] = s_oldX[index_to_slots[i]];
        te.y_[i] = s_oldY[index_to_slots[i]];
        te.pointer_id_[i] = index_to_slots[i];
    }
    // 1. currently there're two fingers on touch screen
    //  but only one finger position information is reported in this loop
    // 2. previously there're two fingers on toush screen 
    //  but only one finger up information is reported in this loop
    // adding another's information
    ////qDebug("Pre-send touch event, current_pointer_count:%d, current_index:%d, touch_action:%d",
    ////    current_pointer_count, current_index, touch_action);
    if ((2 == current_pointer_count && 0 == current_index) ||
       (TouchEvent::ACTION_POINTER_UP == touch_action && 0 == current_index))
    {
        int another_slot = 1 - index_to_slots[0];
        te.x_[1] = s_oldX[another_slot];
        te.y_[1] = s_oldY[another_slot];
        te.pointer_id_[1] = another_slot;
        te.pointer_count_ = 2;
    }
    te.action_index_ = current_action_index;
    te.event_time_ = getCurrentTimeInMsec();
    return te.clone();
#else
    return 0;
#endif
}

QT_END_NAMESPACE
