#ifndef __DRIVERS_TOUCH_EVENT_H__
#define __DRIVERS_TOUCH_EVENT_H__

#include "Base/base.h"

using namespace base;

class TouchEvent
{
public:
    TouchEvent()
        : action_(0)
        , action_index_(0)
        , pointer_count_(0)
    {
        for (int i = 0; i < MAX_TOUCH_POINT; ++i)
        {
            x_[i] = -1;
            y_[i] = -1;
            pointer_id_[i] = -1;
        }

    }
    /**
     * An invalid pointer id.
     *
     * This value (-1) can be used as a placeholder to indicate that a pointer id
     * has not been assigned or is not available.  It cannot appear as
     * a pointer id inside a {@link MotionEvent}.
     */
    static const int INVALID_POINTER_ID = -1;

    static const int MAX_TOUCH_POINT = 2;

    /**
     * Bit mask of the parts of the action code that are the action itself.
     */
    static const int ACTION_MASK             = 0xff;
    
    /**
     * Constant for {@link #getActionMasked}: A pressed gesture has started, the
     * motion contains the initial starting location.
     * <p>
     * This is also a good time to check the button state to distinguish
     * secondary and tertiary button clicks and handle them appropriately.
     * Use {@link #getButtonState} to retrieve the button state.
     * </p>
     */
    static const int ACTION_DOWN             = 0;
    
    /**
     * Constant for {@link #getActionMasked}: A pressed gesture has finished, the
     * motion contains the const release location as well as any intermediate
     * points since the last down or move event.
     */
    static const int ACTION_UP               = 1;
    
    /**
     * Constant for {@link #getActionMasked}: A change has happened during a
     * press gesture (between {@link #ACTION_DOWN} and {@link #ACTION_UP}).
     * The motion contains the most recent point, as well as any intermediate
     * points since the last down or move event.
     */
    static const int ACTION_MOVE             = 2;
    
    /**
     * Constant for {@link #getActionMasked}: The current gesture has been aborted.
     * You will not receive any more points in it.  You should treat this as
     * an up event, but not perform any action that you normally would.
     */
    static const int ACTION_CANCEL           = 3;
    
    /**
     * Constant for {@link #getActionMasked}: A movement has happened outside of the
     * normal bounds of the UI element.  This does not provide a full gesture,
     * but only the initial location of the movement/touch.
     */
    static const int ACTION_OUTSIDE          = 4;

    /**
     * Constant for {@link #getActionMasked}: A non-primary pointer has gone down.
     * <p>
     * Use {@link #getActionIndex} to retrieve the index of the pointer that changed.
     * </p><p>
     * The index is encoded in the {@link #ACTION_POINTER_INDEX_MASK} bits of the
     * unmasked action returned by {@link #getAction}.
     * </p>
     */
    static const int ACTION_POINTER_DOWN     = 5;
    
    /**
     * Constant for {@link #getActionMasked}: A non-primary pointer has gone up.
     * <p>
     * Use {@link #getActionIndex} to retrieve the index of the pointer that changed.
     * </p><p>
     * The index is encoded in the {@link #ACTION_POINTER_INDEX_MASK} bits of the
     * unmasked action returned by {@link #getAction}.
     * </p>
     */
    static const int ACTION_POINTER_UP       = 6;
    static const int ACTION_LONG_TAP       = 7;

    TouchEvent* clone() const
    {
        return new TouchEvent(*this);
    }

    int action() const
    {
        return action_;
    }

    int actionMasked() const
    {
        return action() & ACTION_MASK;
    }

    int actionIndex() const
    {
        return action_index_;
    }

    void setAction(int action)
    {
        action_ = action;
    }
    
    int pointerCount() const
    {
        return pointer_count_;
    }

    // in logical screen coordinate(like 600x800, not 1500x2000)
    int x() const
    {
        return x(0);
    }
    int y() const
    {
        return y(0);
    }

    int x(int index) const
    {
        return x_[index];
    }

    int y(int index) const
    {
        return y_[index];
    }

    void offsetLocation(int offsetX, int offsetY)
    {
        x_[0] += offsetX;
        y_[0] += offsetY;
    }

    int pointerId(int index) const
    {
        return pointer_id_[index];
    }

    int findPointerIndex(int pointer_id) const
    {
        for (int i = 0; i < pointer_count_; ++i)
        {
            if (pointer_id == pointer_id_[i])
            {
                return i;
            }
        }
        return -1;
    }

    long long downTime() const
    {
        return down_time_;
    }
    long long eventTime() const
    {
        return event_time_;
    }

private:
    int x_[MAX_TOUCH_POINT];
    int y_[MAX_TOUCH_POINT];
    int pointer_id_[MAX_TOUCH_POINT];
    int action_;
    int action_index_;
    int pointer_count_;

    long long down_time_;
    long long event_time_;
private:
    friend class KindleTS;
};

typedef shared_ptr<TouchEvent> TouchEventPtr;

#endif
