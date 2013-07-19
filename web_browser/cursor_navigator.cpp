#include "cursor_navigator.h"

namespace webbrowser
{

static const int THRES_HOLD = 5;
static const int CURSOR_WIDTH_THRESHOLD = 120;
static const int CURSOR_HEIGHT_THRESHOLD = 60;

typedef QList<QRect> QWebElementsRects;

// Return screen x position of a widget.
static int screen_x_pos(const QRect& rect)
{
    return rect.topLeft().x();
}

static int screen_y_pos(const QRect& rect)
{
    return rect.topLeft().y();
}

static int screen_y_center(const QRect& rect)
{
    int y = screen_y_pos(rect);
    return y + rect.height() / 2;
}

static int screen_x_center(const QRect& rect)
{
    int x = screen_x_pos(rect);
    return x + rect.width() / 2;
}

struct LessByYCenter
{
    bool operator()(const QRect& a, const QRect& b ) const
    {
        return (screen_y_center(a) < screen_y_center(b));
    }
};

struct GreaterByYCenter
{
    bool operator() ( const QRect& a, const QRect& b) const
    {
        return (screen_y_center(a) > screen_y_center(b));
    }
};

struct LessByXPos
{
    bool operator () (const QRect& a, const QRect& b) const
    {
        return (screen_x_pos(a) < screen_x_pos(b));
    }
};

struct GreaterByXPos
{
    bool operator() ( const QRect& a, const QRect& b) const
    {
        return (screen_x_pos(a) > screen_x_pos(b));
    }
};

struct LessByXCenter
{
    bool operator()(const QRect& a, const QRect& b ) const
    {
        return (screen_x_center(a) < screen_x_center(b));
    }
};

struct GreaterByXCenter
{
    bool operator() ( const QRect& a, const QRect& b) const
    {
        return (screen_x_center(a) > screen_x_center(b));
    }
};

struct LessByYPos
{
    bool operator() ( const QRect& a, const QRect& b) const
    {
        return (screen_y_pos(a) < screen_y_pos(b));
    }
};

struct GreaterByYPos
{
    bool operator() ( const QRect& a, const QRect& b) const
    {
        return (screen_y_pos(a) > screen_y_pos(b));
    }
};

static bool mapToDesktop(QWebView* view, const QWebElement& element, QRect& maped_rect)
{
    QRect element_rect = element.geometry();
    if (element_rect.isEmpty() || element_rect.isNull())
    {
        return false;
    }
    QPoint element_topleft = element_rect.topLeft();
    QWebFrame* parent_frame = element.webFrame();
    while (parent_frame != 0)
    {
        // scroll of frame
        element_topleft -= parent_frame->scrollPosition();

        // offset of frame
        element_topleft += parent_frame->geometry().topLeft();
        parent_frame = parent_frame->parentFrame();
    }

    element_topleft = view->mapToGlobal(element_topleft);
    element_rect.moveTo(element_topleft);
    maped_rect = element_rect;
    return true;
}

static bool isRectsIntersected(const QRect& r1, const QRect& r2, const QWebElementsRects& rects)
{
    //int p1_x = p1.x();
    //int p2_x = p2.x();
    //int p1_y = p1.y();
    //int p2_y = p2.y();
    //QPoint top_left(qMin(p1_x, p2_x), qMin(p1_y, p2_y));
    //QPoint bottom_right(qMax(p1_x, p2_x) + 1, qMax(p1_y, p2_y) + 1);
    //QRect check_rect(top_left, bottom_right);

    QRect check_rect = r1.united(r2);
    foreach(const QRect& rect, rects)
    {
        QRect intersect_rect = check_rect.intersected(rect);
        if (!intersect_rect.isEmpty() && !intersect_rect.contains(r1) && !intersect_rect.contains(r2))
        {
            return true;
        }
    }
    return false;
}

static bool isRectVisible(QWebView* view, const QRect& rect)
{
    QPoint view_pos = view->mapToGlobal(view->rect().topLeft());
    QRect view_rect(view_pos, view->size());
    return view_rect.contains(rect.center());
}

static bool isSameRect(const QRect& r1, const QRect& r2)
{
    return r1 == r2 || r1.contains(r2) || r2.contains(r1);
}

template<class Comp>
bool collect(QWebView * view,
             QWebElementCollection & collection,
             QWebElementsRects & candidates,
             const QRect& current_rect,
             Comp c)
{
    foreach(QWebElement p, collection)
    {
        QRect p_rect;
        mapToDesktop(view, p, p_rect);
        
        //if (isRectVisible(view, p_rect))
        //{
            if (!isSameRect(p_rect, current_rect) && c(p_rect, current_rect))
            {
                candidates.push_back(p_rect);
            }
        //}
    }
    return (candidates.count() > 0);
}

static int lineLength(const QPoint& p1, const QPoint& p2)
{
    return qSqrt((p2.x() - p1.x()) * (p2.x() - p1.x()) + (p2.y() - p1.y()) * (p2.y() - p1.y()));
}

static QRect searchYLess(QWebView* view,
                         QWebElementCollection & collection,
                         const QRect& current_rect)
{

    QWebElementsRects candidates;
    if (!collect(view, collection, candidates, current_rect, LessByYCenter()))
    {
        //if (!collect(view, collection, candidates, current_rect, GreaterByYPos()))
        //{
            return QRect();
        //}
    }

    qStableSort(candidates.begin(), candidates.end(), GreaterByYCenter());
    int dist = INT_MAX;
    QRect result = candidates.front();
    foreach(QRect p, candidates)
    {
        int diff_x = abs(screen_x_center(p) - screen_x_center(current_rect));
        if (diff_x < THRES_HOLD && !isRectsIntersected(p, current_rect, candidates))
        {
            qDebug("searchYLess found (%d, %d, %d, %d) ------ (%d, %d, %d, %d)",
                p.left(), p.top(), p.width(), p.height(),
                current_rect.left(), current_rect.top(), current_rect.width(), current_rect.height());
            return p;
        }

        int diff = lineLength(p.center(), current_rect.center());
        if (dist > diff)
        {
            dist = diff;
            result = p;
        }
    }
    return result;
}

static QRect searchYGreater(QWebView* view,
                            QWebElementCollection & collection,
                            const QRect& current_rect)
{
    QWebElementsRects candidates;
    if (!collect(view, collection, candidates, current_rect, GreaterByYCenter()))
    {
        //if (!collect(view, collection, candidates, current_rect, LessByYPos()))
        //{
            return QRect();
        //}
    }
    qStableSort(candidates.begin(), candidates.end(), LessByYCenter());
    int dist = INT_MAX;
    QRect result = candidates.front();
    foreach(QRect p, candidates)
    {
        int diff_x = abs(screen_x_center(p)- screen_x_center(current_rect));
        if (diff_x < THRES_HOLD && !isRectsIntersected(p, current_rect, candidates))
        {
            qDebug("searchYGreater found (%d, %d, %d, %d) ------ (%d, %d, %d, %d)",
                p.left(), p.top(), p.width(), p.height(),
                current_rect.left(), current_rect.top(), current_rect.width(), current_rect.height());
            return p;
        }

        int diff = lineLength(p.center(), current_rect.center());
        if (dist > diff)
        {
            dist = diff;
            result = p;
        }
    }
    return result;
}

static QRect searchXLess(QWebView* view,
                         QWebElementCollection & collection,
                         const QRect& current_rect)
{
    QWebElementsRects candidates;
    if (!collect(view, collection, candidates, current_rect, LessByXCenter()))
    {
        //if (!collect(view, collection, candidates, current_rect, GreaterByXPos()))
        //{
            return QRect();
        //}
    }

    qStableSort(candidates.begin(), candidates.end(), GreaterByXCenter());
    int dist = INT_MAX;
    QRect result = candidates.front();
    foreach(QRect p, candidates)
    {
        int diff_y = abs(screen_y_center(p) - screen_y_center(current_rect));
        if (diff_y < THRES_HOLD && !isRectsIntersected(p, current_rect, candidates))
        {
            qDebug("searchXLess found (%d, %d, %d, %d) ------ (%d, %d, %d, %d)",
                p.left(), p.top(), p.width(), p.height(),
                current_rect.left(), current_rect.top(), current_rect.width(), current_rect.height());
            return p;
        }

        int diff = lineLength(p.center(), current_rect.center());
        if (dist > diff)
        {
            dist = diff;
            result = p;
        }
    }
    return result;
}

static QRect searchXGreater(QWebView* view,
                            QWebElementCollection & collection,
                            const QRect& current_rect)
{
    QWebElementsRects candidates;
    if (!collect(view, collection, candidates, current_rect, GreaterByXCenter()))
    {
        //if (!collect(view, collection, candidates, current_rect, LessByXPos()))
        //{
            return QRect();
        //}
    }

    qStableSort(candidates.begin(), candidates.end(), LessByXCenter());
    int dist = INT_MAX;
    QRect result = candidates.front();
    foreach(QRect p, candidates)
    {
        int diff_y = abs(screen_y_center(p) - screen_y_center(current_rect));
        if (diff_y < THRES_HOLD && !isRectsIntersected(p, current_rect, candidates))
        {
            qDebug("searchXGreater found (%d, %d, %d, %d) ------ (%d, %d, %d, %d)",
                p.left(), p.top(), p.width(), p.height(),
                current_rect.left(), current_rect.top(), current_rect.width(), current_rect.height());
            return p;
        }

        int diff = lineLength(p.center(), current_rect.center());
        if (dist > diff)
        {
            dist = diff;
            result = p;
        }
    }
    return result;
}

static QRect search(QWebView* view,
                    QWebElementCollection & collection,
                    const QRect& current_rect,
                    int key)
{
    // Sort them based on key.
    if (key == Qt::Key_Up)
    {
        return searchYLess(view, collection, current_rect);
    }
    else if (key == Qt::Key_Down)
    {
        return searchYGreater(view, collection, current_rect);
    }
    else if (key == Qt::Key_Left)
    {
       return searchXLess(view, collection, current_rect);
    }
    else if (key == Qt::Key_Right)
    {
        return searchXGreater(view, collection, current_rect);
    }
    return QRect();
}

CursorNavigator::CursorNavigator(QWebView* view)
    : view_(view)
    , finger_cursor_(QPixmap(":/res/finger.png"), 0, 0)
    , pointer_cursor_(QPixmap(":/res/cursor.png"), 0, 0)
{
    connect(view_, SIGNAL(loadStarted(void)), this, SLOT(onViewLoadStarted(void)));
    connect(view_, SIGNAL(loadFinished(bool)), this, SLOT(onViewLoadFinished(bool)));
    view_->setCursor(finger_cursor_);
}

CursorNavigator::~CursorNavigator()
{
}

void CursorNavigator::updateCursor()
{
    //test for linkelement
    //QWebHitTestResult hr = view_->page()->currentFrame()->hitTestContent(QCursor::pos() - view_->pos());
    //if (hr.linkElement().isNull())
    //{
    //    view_->setCursor(pointer_cursor_);
    //}
    //else
    //{
    //    view_->setCursor(finger_cursor_);
    //}
    view_->setCursor(finger_cursor_);
}

QRect CursorNavigator::findCursorElementRect()
{
    QPoint current_cursor_pos = QCursor::pos();
    QPoint view_cursor_pos = view_->mapFromGlobal(current_cursor_pos);
    QWebHitTestResult hit_test = view_->page()->currentFrame()->hitTestContent(view_cursor_pos);

    QWebElement hit_element = hit_test.element();
    qDebug("Tag Name:%s", qPrintable(hit_element.tagName()));

    if (!hit_test.linkElement().isNull() ||
        hit_element.tagName().compare("input", Qt::CaseInsensitive) == 0 ||
        hit_element.tagName().compare("textarea", Qt::CaseInsensitive) == 0)
    {
        QRect e_rect;
        if (!mapToDesktop(view_, hit_test.linkElement(), e_rect) &&
            (hit_element.tagName().compare("input", Qt::CaseInsensitive) == 0 ||
             hit_element.tagName().compare("textarea", Qt::CaseInsensitive) == 0))
        {
            foreach(QWebElement e, form_elements_)
            {
                if (mapToDesktop(view_, e, e_rect) && e_rect.contains(current_cursor_pos))
                {
                    break;
                }
            }
        }
        if (!e_rect.isEmpty() && !e_rect.isNull() &&
            e_rect.width() <= CURSOR_WIDTH_THRESHOLD && e_rect.height() <= CURSOR_HEIGHT_THRESHOLD)
        {
            return e_rect;
        }
    }
    return QRect();
}

bool CursorNavigator::moveCursor(int key)
{
    if (view_ == 0 || link_elements_.count() == 0)
    {
        return false;
    }

    //QRect current_rect = findCursorElementRect();
    //if (current_rect.isNull())
    //{
    QRect current_rect = QRect(QCursor::pos(), QSize(2, 2));
    //}

    QRect move_to = search(view_, link_elements_, current_rect, key);
    if (move_to.isNull() || !isRectVisible(view_, move_to))
    {
        return false;
    }

    //qDebug("Set Cursor to (%d, %d)", move_to.center().x(), move_to.center().y());
    QCursor::setPos(move_to.center());
    updateCursor();
    return true;
}

void CursorNavigator::moveCursorToCenter(int key)
{
    QPoint view_pos = view_->mapToGlobal(view_->rect().topLeft());
    QRect  view_rect = view_->rect();
    view_rect.moveTo(view_pos);
    QPoint center = view_rect.center();

    switch (key)
    {
    case Qt::Key_Left:
    case Qt::Key_Right:
        {
            qDebug("Previous:%d, %d", QCursor::pos().x(), QCursor::pos().y());
            QPoint pos(center.x(), QCursor::pos().y());
            qDebug("Current:%d, %d", pos.x(), pos.y());
            QCursor::setPos(pos);
        }
        break;
    case Qt::Key_Up:
    case Qt::Key_Down:
        {
            qDebug("Previous:%d, %d", QCursor::pos().x(), QCursor::pos().y());
            QPoint pos(QCursor::pos().x(), center.y());
            qDebug("Current:%d, %d", pos.x(), pos.y());
            QCursor::setPos(pos);
        }
        break;
    default:
        break;
    }
}

void CursorNavigator::onViewLoadStarted()
{
}

void CursorNavigator::onViewLoadFinished(bool ok)
{
    //qDebug() << page()->currentFrame()->documentElement().toOuterXml();
    //collect the form elements in current page
    form_elements_ = view_->page()->currentFrame()->documentElement().findAll("input, textarea");
    link_elements_ = view_->page()->currentFrame()->documentElement().findAll("a, link, visited, input, textarea");
    qDebug("Link Elements:%d", link_elements_.count());
    view_->setCursor(finger_cursor_);
}

void CursorNavigator::postMousePressEvent()
{
    QPoint pos_in_view = view_->mapFromGlobal(QCursor::pos());
    QMouseEvent* me = new QMouseEvent(QEvent::MouseButtonPress, pos_in_view, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::postEvent(view_, me);
}

void CursorNavigator::postMouseReleaseEvent()
{
    QPoint pos_in_view = view_->mapFromGlobal(QCursor::pos());
    QMouseEvent* me = new QMouseEvent(QEvent::MouseButtonRelease, pos_in_view, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::postEvent(view_, me);
}

}
