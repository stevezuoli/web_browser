#include <QtGui/QtGui>
#include <algorithm>
#include <math.h>
#include <cstdio>
#include <QtGui/qscreen_qws.h>
#include <QtGui/qwsdisplay_qws.h>

#include "screen_manager.h"
#include "Screen/screen_proxy.h"

using namespace std;
using namespace duokan::screen;

std::vector<unsigned char> img;

static QVector<QRgb> color_table;

/// Screen mananger is designed to help applications to:
/// - Copy data from Qt framebuffer.
/// - Update the screen by specified waveform.
/// There are some good reasons that we move the data copying from
/// Qt screen driver to application level:
/// - More easy to deploy. Don't need to hack the Qt transform driver.
/// - More robust.
/// - More flexible, application can control everything. It's even possible
/// for application to maintain several buffer blocks.
/// - Better performance. Don't need to copy all changes to controller especially
/// window switching.
ScreenManager::ScreenManager()
: fb_controller_()
, busy_(false)
, busy_index_(0)
, screen_saver_index_(-1)
, sleeping_(true)
, enable_(true)
, screen_width_(fb_controller_.width())
, screen_height_(fb_controller_.height())
{
    start();

    // setup connection.
    busy_timer_.setInterval(1200);
    connect(&busy_timer_, SIGNAL(timeout()), this, SLOT(onBusyTimeout()));
}

ScreenManager::~ScreenManager()
{
    stop();
}

void ScreenManager::enableUpdate(bool enable)
{
    enable_ = enable;
}

void ScreenManager::start()
{
    fb_buffer_size_ = 0;
    fb_buffer_ = (uchar *)fb_controller_.mmapFb((unsigned long *)&fb_buffer_size_);
    qDebug("FB init done. Buffer %p size %d", fb_buffer_, fb_buffer_size_);

    duokan::screen::ScreenProxy & screen_proxy = duokan::screen::instance();
    connect(&screen_proxy, SIGNAL(screenUpdate(ScreenCommand&)), this, SLOT(onScreenUpdate(ScreenCommand&)));
}

void ScreenManager::stop()
{
    duokan::screen::ScreenProxy & screen_proxy = duokan::screen::instance();
    disconnect(&screen_proxy, SIGNAL(screenUpdate(ScreenCommand&)), this, SLOT(onScreenUpdate(ScreenCommand&)));
}

bool ScreenManager::setGrayScale(int colors)
{
    return true;
}

int ScreenManager::grayScale()
{
    return 8;
}


QVector<QRgb> & ScreenManager::colorTable()
{
    if (color_table.size() <= 0)
    {
        for(int i = 0; i < 256; ++i)
        {
            color_table.push_back(qRgb(i, i, i));
        }
    }
    return color_table;
}

void ScreenManager::setBusy(bool busy, bool show_indicator)
{
    busy_ = busy;
    busy_index_ = 0;
    busy_canvas_.reset(0);
    if (busy_ && show_indicator)
    {
        onBusyTimeout();
        busy_timer_.start();
    }
    else
    {
        busy_timer_.stop();
    }
}

void ScreenManager::fillScreen(unsigned char color)
{
    ensureRunning();
    ScreenCommand command;
    command.type = ScreenCommand::FILL_SCREEN;
    command.color = color;
    command.update_flags = ScreenCommand::FULL_UPDATE;
    command.waveform = duokan::screen::ScreenProxy::GC;
    command.wait_flags = ScreenCommand::WAIT_BEFORE_UPDATE;
    fillScreen(command);
}

void ScreenManager::refreshScreen(duokan::screen::ScreenProxy::Waveform waveform)
{
    ensureRunning();
    ScreenCommand command;
    command.type = ScreenCommand::SYNC_AND_UPDATE;
    command.update_flags = ScreenCommand::FULL_UPDATE;
    command.left = 0;
    command.top = 0;
    command.width = screen_width_;
    command.height = screen_height_;
    command.waveform = waveform;
    command.wait_flags = ScreenCommand::WAIT_BEFORE_UPDATE;
    updateWidget(command);
}

void ScreenManager::drawImage(const QImage &image)
{
    QRect rc(0, 0, screen_width_, screen_height_);
    blit(rc, image, duokan::screen::ScreenProxy::GC);
}

void ScreenManager::fadeScreen()
{
    // Have to use two images as QPainter does not support all features on index 8 image.
    QImage image(imageFromScreen());
    QRect rc(0, 0, screen_width_, screen_height_);

    {
        QImage argb = image.convertToFormat(QImage::Format_ARGB32);
        QPainter p(&argb);
        QBrush brush(QColor(0, 0, 0, 100), Qt::DiagCrossPattern);
        p.fillRect(rc, brush);
        image = argb.convertToFormat(QImage::Format_Indexed8, image.colorTable());
    }

    blit(rc, image, duokan::screen::ScreenProxy::GU);
}

/// Show USB connection image on screen.
void ScreenManager::showUSBScreen()
{
}

void ScreenManager::showDeepSleepScreen()
{
    QImage image(nextScreenSaverImage());
    if (!image.isNull())
    {
        drawImage(image);
    }
    else
    {
        fillScreen(0xff);
    }
}

void ScreenManager::blit(const QRect & rc,
                         const QImage& image,
                         duokan::screen::ScreenProxy::Waveform waveform)
{
    // Convert from Qt framebuffer to marvell buffer.
    //flashScreen(waveform);
    fb_controller_.blit(rc, image, rc.left(), rc.top(), waveform);
}

void ScreenManager::onScreenUpdate(ScreenCommand& command)
{
    qDebug("onScreenUpdate at %s sleeping %d", qPrintable(QTime::currentTime().toString("mm:ss.zzz")), sleeping_);

    // Wakeup controller.
    ensureRunning();
    if (isBusy() || !isUpdateEnabled())
    {
        return;
    }

    switch (command.type)
    {
    case ScreenCommand::WAIT_FOR_FINISHED:
        ensureUpdateFinished();
        break;
    case ScreenCommand::SYNC:
        sync(command);
        break;
    case ScreenCommand::UPDATE:
        updateScreen(command);
        break;
    case ScreenCommand::SYNC_AND_UPDATE:
        updateWidget(command);
        break;
    case ScreenCommand::DRAW_POINTS:
        drawLine(command);
        break;
    case ScreenCommand::FILL_SCREEN:
        fillScreen(command);
        break;
    default:
        break;
    }
}

/// This function uses screen coordinates. TODO: Optimize it later.
void ScreenManager::onBusyTimeout()
{
    // Using current screen data as the image.
    if (!busy_canvas_)
    {
        QImage image(imageFromScreen());
        if (image.format() == QImage::Format_Indexed8)
        {
            busy_canvas_.reset(new QImage(image.convertToFormat(QImage::Format_ARGB32, image.colorTable())));
        }
        else
        {
            busy_canvas_.reset(new QImage(image.convertToFormat(QImage::Format_ARGB32)));
        }
    }

    // TODO, we may need to rotate here.
    QImage busy_image = busyImage(busy_index_++);
    int x = (screen_width_  - busy_image.width()) / 2;
    int y = (screen_height_  - busy_image.height()) / 2;
    QRect rc(x, y, busy_image.width(), busy_image.height());
    QImage dst = busy_canvas_->copy(rc);
    {
        QPainter painter(&dst);
        painter.drawImage(0, 0, busy_image);
    }
    dst = dst.convertToFormat(QImage::Format_Indexed8, colorTable());
    blit(rc, dst, duokan::screen::ScreenProxy::GU);
}

/// Make sure the update request has been processed.
void ScreenManager::ensureUpdateFinished()
{
}

/// This function copies data from Qt framebuffer to controller.
void ScreenManager::sync(ScreenCommand & command)
{
    qDebug("sync data now...%d %d %d %d", command.left, command.top, command.width, command.height);

    snapshot("/mnt/us/DK_System/duokan/a.png");

    QTime t; t.start();
    uchar * dst  = fb_buffer_ + command.top * screen_width_ / 2 + command.left / 2;
    uchar * base = QScreen::instance()->base();
    base = base + command.top * QScreen::instance()->linestep() + command.left * 4;
    for(int i = 0; i < command.height; ++i)
    {
        uint * s = reinterpret_cast<uint *>(base);
        uchar * t = dst;
        for(int j = 0; j < command.width/2; ++j, ++t)
        {
            *t = (qGray(*s++) >> 4);
            *t += (qGray(*s++) & 0xf0);
        }
        dst += screen_width_ / 2;
        base += QScreen::instance()->linestep();
    }

#if 0
    fb_controller_.blit(QRect(command.left, command.top, command.width, command.height),
        imageFromScreen(),
        command.left,
        command.top,
        static_cast<ScreenProxy::Waveform>(command.waveform));
#endif

    qDebug("elapsed %d ms", t.elapsed());
}

/// This function copies data for the specified widget from Qt framebuffer
/// to display controller and updates the screen by using the waveform.
void ScreenManager::updateWidget(ScreenCommand & command)
{
    sync(command);
    updateScreen(command);
}

/// Update the specified region of the widget.
void ScreenManager::updateWidgetRegion(ScreenCommand & command)
{
    sync(command);
    updateScreen(command);
}

/// Update screen without copying any data.
void ScreenManager::updateScreen(ScreenCommand & command)
{
    if (command.wait_flags & ScreenCommand::WAIT_BEFORE_UPDATE)
    {
        ensureUpdateFinished();
    }

    if (command.waveform == duokan::screen::ScreenProxy::DW)
    {
        flashScreen(command.waveform);
        return;
    }

    if (command.update_flags == ScreenCommand::FULL_UPDATE)
    {
        if (command.waveform == duokan::screen::ScreenProxy::GU)
        {
            flashScreen(command.waveform);
        }
        else if (command.waveform == duokan::screen::ScreenProxy::GC)
        {
            flashScreen(command.waveform);
        }
    }
    else
    {
        // Only update the specified region.
        if (command.waveform == duokan::screen::ScreenProxy::GU)
        {
            flashScreen(command.waveform);
        }
        else if (command.waveform == duokan::screen::ScreenProxy::GC)
        {
            flashScreen(command.waveform);
        }
    }
}

void ScreenManager::reset()
{
}

bool ScreenManager::ensureRunning()
{
    return true;
}

bool ScreenManager::sleep()
{
    return true;
}

void ScreenManager::shutdown()
{
}

void ScreenManager::remap()
{
}

bool ScreenManager::dbgStateTest()
{
    if (!ensureRunning())
    {
        return false;
    }

    refreshScreen(duokan::screen::ScreenProxy::GC);

    sleep();

    return true;
}

void ScreenManager::snapshot(const QString &path)
{
    QImage image(imageFromScreen());
    image.save(path, "png");
}

/// Draw line on screen directly. Make sure you call the enableFastestUpdate
/// before using this function.
void ScreenManager::drawLine(ScreenCommand & command)
{
    // Check range.
    static const int MAX_SIZE = 50;
    if (command.size >= MAX_SIZE || command.size <= 0)
    {
        return;
    }

    int size = command.size * command.size;
    if (static_cast<int>(img.size()) < size)
    {
        img.resize(size);
    }
    memset(&img[0], command.color, size);

    int rad = command.size / 2;
    int px1 = command.left - rad;
    int py1 = command.top - rad;
    int px2 = command.left + command.width - rad;
    int py2 = command.top + command.height - rad;

#ifdef ENABLE_EINK_SCREEN
    // draw the end point
    // bs_cmd_ld_img_area_data(DEFAULT_PIXEL_FORMAT, px2, py2, command.size, command.size, &img[0]);
#endif

    bool is_steep = abs(py2 - py1) > abs(px2 - px1);
    if (is_steep)
    {
        swap(px1, py1);
        swap(px2, py2);
    }

    // setup line draw
    int deltax   = abs(px2 - px1);
    int deltaerr = abs(py2 - py1);
    int error = 0;
    int x = px1;
    int y = py1;

    // setup step increment
    int xstep = (px1 < px2) ? 1 : -1;
    int ystep = (py1 < py2) ? 1 : -1;

    if (is_steep)
    {
        // draw swapped line
        for (int numpix = 0; numpix < deltax; numpix++)
        {
            x += xstep;
            error += deltaerr;

            if (2 * error > deltax)
            {
                y += ystep;
                error -= deltax;
            }

#ifdef ENABLE_EINK_SCREEN
            // draw the interposed point
            // bs_cmd_ld_img_area_data(DEFAULT_PIXEL_FORMAT, y, x, command.size, command.size, &img[0]);
#endif
        }
    }
    else
    {
        // draw normal line
        for (int numpix = 0; numpix < deltax; numpix++)
        {
            x += xstep;
            error += deltaerr;

            if (2 * error > deltax)
            {
                y += ystep;
                error -= deltax;
            }

#ifdef ENABLE_EINK_SCREEN
            // draw the interposed point
            // bs_cmd_ld_img_area_data(DEFAULT_PIXEL_FORMAT, x, y, command.size, command.size, &img[0]);
#endif
        }
    }
}

void ScreenManager::fillScreen(ScreenCommand & command)
{
#ifdef BUILD_FOR_ARM

#endif
    updateScreen(command);
}

QImage ScreenManager::busyImage(int index)
{
    return QImage();
}

QDir ScreenManager::screenSaverDir()
{
    QDir dir(SHARE_ROOT);
    dir.cd("system_manager/images");
    return dir;
}

QImage ScreenManager::nextScreenSaverImage()
{
    if (++screen_saver_index_ >= screenSaverCount())
    {
        screen_saver_index_ = 0;
    }

    // Place images in $SHARE_ROOT/system_manager/images/
    QString path("screen_saver%1.png");
    path = path.arg(screen_saver_index_);
    path = screenSaverDir().absoluteFilePath(path);
    QImage result(path);
    if (!result.isNull())
    {
        if (result.format() != QImage::Format_Indexed8)
        {
            return result.convertToFormat(QImage::Format_Indexed8, colorTable());
        }
    }
    return result;
}

int ScreenManager::screenSaverCount()
{
    return screenSaverDir().entryInfoList(QDir::NoDotAndDotDot|QDir::Files).size();
}

void ScreenManager::flashScreen(int waveform)
{
    //if (waveform == duokan::screen::ScreenProxy::GC)
    //{
    //    kermit_set_waveform(WF_GC);
    //    kermit_flash_screen();
    //}
    //else if (waveform == duokan::screen::ScreenProxy::DW)
    //{
    //    kermit_set_waveform(WF_MU);
    //}
    //else
    //{
    //    kermit_set_waveform(WF_GU);
    //}
    //kermit_display_on();
    //kermit_display_off();
    qDebug("Flash Screen:%d, width:%d, height:%d", waveform, screen_width_, screen_height_);
    fb_controller_.flash(QRect(0, 0, screen_width_, screen_height_),
        static_cast<ScreenProxy::Waveform>(waveform));
}

QImage ScreenManager::imageFromScreen()
{
    // Takes a non-const data buffer, this version will never alter the contents of the buffer.
    QScreen *instance = QScreen::instance();
    const uchar *data = instance->base();
    int degree = 0;
    degree = instance->transformOrientation() * 90;
    int width = instance->width();
    int height = instance->height();
    if (degree == 90 || degree == 270)
    {
        std::swap(width, height);
    }
    QImage image(data, width, height, instance->pixelFormat());

    // Necessary to check as when rotation, it uses different pixel format.
    if (instance->pixelFormat() == QImage::Format_Indexed8)
    {
        image.setColorTable(colorTable());
    }
    return image;
}


