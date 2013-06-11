#ifndef SCREEN_MANAGER_H_
#define SCREEN_MANAGER_H_

#include <QColor>
#include <QtNetwork/QtNetwork>
#include "Screen/screen_proxy.h"
#include "FbLib/frame_buffer_lib.h"
#include "Base/base.h"

class ScreenManager : public QObject
{
    Q_OBJECT
public:
    ScreenManager();
    ~ScreenManager();

public:
    void enableUpdate(bool enable);
    bool isUpdateEnabled() { return enable_; }

    bool setGrayScale(int colors);
    int grayScale();

    bool isBusy() { return busy_; }
    void setBusy(bool busy = true, bool show_indicator = true);

    void refreshScreen(duokan::screen::ScreenProxy::Waveform waveform);
    void drawImage(const QImage &image);
    void fadeScreen();
    void showUSBScreen();
    void showDeepSleepScreen();

    void blit(const QRect & rc, const QImage& image, duokan::screen::ScreenProxy::Waveform waveform = duokan::screen::ScreenProxy::GU);
    void fillScreen(unsigned char color);
    bool sleep();
    void shutdown();

    void reset();
    void remap();

    bool dbgStateTest();

    void snapshot(const QString &path);
    void drawLine(duokan::screen::ScreenCommand & command);

private Q_SLOTS:
    void onScreenUpdate(ScreenCommand& command);
    void onBusyTimeout();

private:
    void start();
    void stop();

    bool ensureRunning();

    void ensureUpdateFinished();
    void sync(duokan::screen::ScreenCommand & command);
    void updateWidget(duokan::screen::ScreenCommand & command);
    void updateWidgetRegion(duokan::screen::ScreenCommand & command);
    void updateScreen(duokan::screen::ScreenCommand & command);

    void fillScreen(duokan::screen::ScreenCommand & command);

    QVector<QRgb> & colorTable();
    QImage imageFromScreen();
    QImage busyImage(int index = 0);

    QDir screenSaverDir();
    QImage nextScreenSaverImage();
    int screenSaverCount();

    void flashScreen(int waveform);

private:
    Fblib  fb_controller_;
    uchar* fb_buffer_;
    int    fb_buffer_size_;

    bool busy_;
    int busy_index_;
    scoped_ptr<QImage> busy_canvas_;

    int screen_saver_index_;

    bool sleeping_;
    bool enable_;

    int screen_width_;
    int screen_height_;
    QTimer busy_timer_;
};

#endif      // SCREEN_MANAGER_H_
