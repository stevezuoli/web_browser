/// Author John
/// Qt based Eink screen plugin for Duokan project.

#ifndef DUOKAN_SCREEN_H_
#define DUOKAN_SCREEN_H_

#include <QScreen>
#include <QSharedMemory>

/// Eink screen driver. It initialize the screen size
/// according to the environment variables. It provides
/// the memory chunk for qt screen driver. Check the
/// environment: SCREEN_WIDTH and SCREEN_HEIGHT.
/// The main idea to introduce this driver is to avoid implementing
/// a Linux kernel level framebuffer driver. This driver does
/// use any platform or hardware related API.
class DuokanScreen : public QScreen
{
public:
    DuokanScreen(int displayId);
    ~DuokanScreen();

    bool connect(const QString &displaySpec);
    bool initDevice();
    void shutdownDevice();
    void disconnect();

    // TODO.
    void setMode(int, int, int) {}
    void blank(bool) {}

    void exposeRegion(QRegion r, int changing);
    void blit(const QImage &img, const QPoint &topLeft, const QRegion &region);
    void solidFill(const QColor &color, const QRegion &region);

private:
    void k4_hline(int x1, int y, int x2, int c) ;
    void blit_K4(const QImage& image, const QPoint& topLeft, const QRegion& region);
    void blit12To4(const QImage &image, const QPoint &topLeft, const QRegion &region);
    void blit16To4(const QImage &image, const QPoint &topLeft, const QRegion &region);
    void blit32To4(const QImage &image, const QPoint &topLeft, const QRegion &region);

private:
    uchar *memory_;
};

#endif  // DUOKAN_SCREEN_H_

