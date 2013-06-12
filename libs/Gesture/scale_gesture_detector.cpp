#include "scale_gesture_detector.h"
#include "KindleTS/touch_event.h"

namespace gesture
{

ScaleGestureDetector::ScaleGestureDetector()
    : prev_event_(0)
    , current_event_(0)
{
    gesture_in_progress_ = false;
    active_id0_ = active_id1_ = -1;
    active0_most_recent_ = false;
    prev_event_ = NULL;
    current_event_ = NULL;
    invalid_gesture_ = false;
    reset();
}

ScaleGestureDetector::~ScaleGestureDetector()
{
    reset();
}

ScaleGestureDetector* ScaleGestureDetector::instance()
{
    static ScaleGestureDetector detector;
    return &detector;
}

void ScaleGestureDetector::reset()
{
    if (NULL != prev_event_)
    {
        delete prev_event_;
        prev_event_ = NULL;
    }
    if (NULL != current_event_)
    {
        delete current_event_;
        current_event_ = NULL;
    }
    gesture_in_progress_ = false;
    active_id0_ = active_id1_ = -1;
    active0_most_recent_ = false;
    invalid_gesture_ = false;
}

bool ScaleGestureDetector::onTouchEvent(TouchEvent* moveEvent)
{
    int action = moveEvent->actionMasked();
    bool handled = true;

    qDebug("ScaleGestureDetector::onTouchEvent, action:%d, gesture_in_progress:%d", moveEvent->action(), gesture_in_progress_ ? 1 : 0);
    if (invalid_gesture_)
    {
        qDebug("Invalid Gesture");
        handled = false;
    }
    else if (!gesture_in_progress_)
    {
        switch (action)
        {
            case TouchEvent::ACTION_DOWN:
                active_id0_ = moveEvent->pointerId(0);
                active0_most_recent_ = true;
                break;
            case TouchEvent::ACTION_UP:
                reset();
                break;
            case TouchEvent::ACTION_POINTER_DOWN:
            case TouchEvent::ACTION_MOVE:
                {
                    if (NULL != prev_event_)
                    {
                        delete prev_event_;
                    }
                    prev_event_ = moveEvent->clone();
                    int index1 = moveEvent->actionIndex();
                    int index0 = moveEvent->findPointerIndex(active_id0_);
                    active_id1_ = moveEvent->pointerId(index1);
                    if (index0 < 0 || index0 == index1)
                    {
                        index0 = findNewActiveIndex(moveEvent, active_id1_, -1);
                        active_id0_ = moveEvent->pointerId(index0);       
                    }
                    active0_most_recent_ = false;
                    setContext(moveEvent);
                    //qDebug("scaleBegin");
                    //m_listener->OnScaleBegin(this);
                    emit scaleBegin();
                    gesture_in_progress_ = true;
                }
                break;
            default:
                break;

        }
    }
    else
    {
        switch (action)
        {
            case TouchEvent::ACTION_POINTER_UP:
                {
                    setContext(moveEvent);
                    int actionIndex = moveEvent->actionIndex();
                    int actionId = moveEvent->pointerId(actionIndex);
                    //setContext(moveEvent);
                    int activeId = actionId == active_id0_ ? active_id1_ : active_id0_;
                    int index = moveEvent->findPointerIndex(activeId);
                    //focus_x_ = moveEvent->x(index);
                    //focus_y_ = moveEvent->y(index);
                    
                    //m_listener->OnScaleEnd(this);
                    //qDebug("scaleEnd");
                    emit scaleEnd();
                    reset();
                    active_id0_ = activeId;
                    active0_most_recent_ = true;
                }
                break;
            case TouchEvent::ACTION_CANCEL:
                //m_listener->OnScaleEnd(this);
                //qDebug("scaleEnd");
                emit scaleEnd();
                reset();
                break;
            case TouchEvent::ACTION_UP:
                reset();
                break;
            case TouchEvent::ACTION_MOVE:
                setContext(moveEvent);
                //if (m_listener->OnScale(this))
                //qDebug("scaling");
                emit scaling();
                {
                    delete prev_event_;
                    prev_event_ = moveEvent->clone();
                }
                break;
            default:
                emit scaleEnd();
                reset();
                break;
        }
    }
    return handled;
}

int ScaleGestureDetector::findNewActiveIndex(TouchEvent* moveEvent, int otherActiveId, int removedPointerIndex) {
    int otherActiveIndex = moveEvent->findPointerIndex(otherActiveId);
    for (int i = 0; i< moveEvent->pointerCount(); ++i)
    {
        if (i != removedPointerIndex && i != otherActiveIndex)
        {
            return i;
        }
    }
    return -1;
}

void ScaleGestureDetector::setContext(TouchEvent* moveEvent)
{
    if (NULL != current_event_)
    {
        delete current_event_;
    }

    current_len_ = -1;
    prev_len_ = -1;
    current_event_ = moveEvent->clone();
    TouchEvent* prev = prev_event_;
    TouchEvent* curr = current_event_;
    int prevIndex0 = prev->findPointerIndex(active_id0_);
    int prevIndex1 = prev->findPointerIndex(active_id1_);
    int currIndex0 = curr->findPointerIndex(active_id0_);
    int currIndex1 = curr->findPointerIndex(active_id1_);
    if (prevIndex0 < 0 || prevIndex1 < 0|| currIndex0 < 0 || currIndex1 < 0)
    {
        invalid_gesture_ = true;
        qDebug("Inavlid TouchEvent stream detected.");
        if (gesture_in_progress_)
        {
            //m_listener->OnScaleEnd(this);
            //qDebug("scaleEnd");
            emit scaleEnd();
            reset();
        }
        return;
    }
    int px0 = prev->x(prevIndex0);
    int py0 = prev->y(prevIndex0);
    int px1 = prev->x(prevIndex1);
    int py1 = prev->y(prevIndex1);
    int cx0 = curr->x(currIndex0);
    int cy0 = curr->y(currIndex0);
    int cx1 = curr->x(currIndex1);
    int cy1 = curr->y(currIndex1);
    
    current_finger_diff_x_ = px1 - px0;
    current_finger_diff_y_ = py1 - py0;
    current_finger_diff_x_ = cx1 - cx0;
    current_finger_diff_y_ = cy1 - cy0;
    focus_x_ = (cx0 + cx1) / 2;
    focus_y_ = (cy0 + cy1) / 2;
}

int ScaleGestureDetector::getCurrentSpan()
{
    if (current_len_ == -1)
    {
        current_len_ = qSqrt(current_finger_diff_x_ * current_finger_diff_x_ + current_finger_diff_y_ * current_finger_diff_y_);
    }
    return current_len_;
}

}
