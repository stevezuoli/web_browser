#ifndef duokan_SCREEN_UPDATE_WATCHE_H_
#define duokan_SCREEN_UPDATE_WATCHE_H_

#include <QWidget>
#include "screen_proxy.h"

namespace duokan
{
namespace screen
{

class ScreenUpdateWatcher : public QObject
{
    Q_OBJECT

public:
    static ScreenUpdateWatcher & instance()
    {
        static ScreenUpdateWatcher instance_;
        return instance_;
    }
    ~ScreenUpdateWatcher();

public Q_SLOTS:
    void addWatcher(QWidget *widget, int gc_interval = -1);
    void removeWatcher(QWidget *widget);

    void enqueue(QWidget *widget, duokan::screen::ScreenProxy::Waveform w = duokan::screen::ScreenProxy::GC, duokan::screen::ScreenCommand::WaitMode wait = ScreenCommand::WAIT_BEFORE_UPDATE);
    void enqueue(QWidget *widget, const QRect & rc, duokan::screen::ScreenProxy::Waveform w, duokan::screen::ScreenCommand::WaitMode wait = ScreenCommand::WAIT_BEFORE_UPDATE);

    void flush(QWidget *widget, duokan::screen::ScreenProxy::Waveform w, duokan::screen::ScreenCommand::WaitMode wait = ScreenCommand::WAIT_BEFORE_UPDATE);

    void dwEnqueueStart(QWidget *widget, const QRect & rc);
    void dwEnqueueEnd(QWidget *widget, const QRect & rc);

    void updateScreen();
    void guUpdateScreen();
    void gcUpdateScreen();

    void enableUpdate(bool enable = true);

public:
    bool isQueueEmpty();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    ScreenUpdateWatcher();
    ScreenUpdateWatcher(ScreenUpdateWatcher &ref);

    void updateScreenInternal(bool automatic, duokan::screen::ScreenProxy::Waveform w = duokan::screen::ScreenProxy::GC);

    bool dwEnqueue() { return dw_enqueue_; }
    void useDwEnqueue(bool use) { dw_enqueue_ = use; }

private:
    struct UpdateItem
    {
        duokan::screen::ScreenProxy::Waveform waveform;
        duokan::screen::ScreenCommand::WaitMode wait;
        QRect rc;

        UpdateItem(){}

        UpdateItem(duokan::screen::ScreenProxy::Waveform w, duokan::screen::ScreenCommand::WaitMode wm, QRect rect= QRect())
            : waveform(w)
            , wait(wm)
            , rc(rect)
        {}
    };

    struct UpdateCount
    {
        int current_;
        int max_;

        UpdateCount()
            : current_(0)
            , max_(0)
        {}

        UpdateCount(int c, int m)
            : current_(c)
            , max_(m)
        {}
    };

private:
    bool enqueue(UpdateItem &, QWidget *, duokan::screen::ScreenProxy::Waveform, duokan::screen::ScreenCommand::WaitMode, const QRect & rc = QRect());

private:
    QQueue<UpdateItem> queue_;
    QMap<QWidget *, UpdateCount> widget_map_;
    bool dw_enqueue_;
};

ScreenUpdateWatcher & watcher();

};  // namespace screen

};  // namespace duokan

#endif
