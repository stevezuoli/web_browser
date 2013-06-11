#ifndef _SCALE_GESTUREDETECTOR_H_
#define _SCALE_GESTUREDETECTOR_H_

#include <QtCore>

class TouchEvent;
namespace gesture
{

class ScaleGestureDetector: public QObject
{
    Q_OBJECT
    ScaleGestureDetector();
    ScaleGestureDetector(const ScaleGestureDetector&);
public:
    virtual ~ScaleGestureDetector();

    static ScaleGestureDetector* instance();

    int getFocusX() const { return focus_x_; }
    int getFocusY() const { return focus_y_; }

    int getCurrentSpan();

    int getCurrentSpanX() const { return current_finger_diff_x_; }
    int getCurrentSpanY() const { return current_finger_diff_y_; }
    bool isScaling() const { return gesture_in_progress_; }

Q_SIGNALS:
    void scaleBegin();
    void scaleEnd();
    void scaling();

private Q_SLOTS:
    bool onTouchEvent(TouchEvent* moveEvent);

private:
    int  findNewActiveIndex(TouchEvent* moveEvent, int otherActiveId, int removedPointerIndex);
    void reset();
    void setContext(TouchEvent* moveEvent);

private:
    TouchEvent* prev_event_;
    TouchEvent* current_event_;

    bool gesture_in_progress_;
    int  active_id0_;
    int  active_id1_;

    bool active0_most_recent_;
    bool invalid_gesture_;
    int  prev_finger_diff_x_;
    int  prev_finger_diff_y_;
    int  current_finger_diff_x_;
    int  current_finger_diff_y_;
    int  focus_x_;
    int  focus_y_;
    int  current_len_;
    int  prev_len_;
};

}

#endif
