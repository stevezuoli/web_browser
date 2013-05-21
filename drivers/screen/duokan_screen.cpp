#include <QColor>
#include <QTime>
#include <qglobal.h>
#include "duokan_screen.h"

static const int DEFAULT_WIDTH = 600;
static const int DEFAULT_HEIGHT = 800;
static bool screen_debug = false;

DuokanScreen::DuokanScreen(int displayId)
: QScreen(displayId)
, memory_(0)
{
    // qDebug("Duokan screen created!");
    screen_debug = (qgetenv("DEBUG_SCREEN_DRIVER").toInt() > 0);
}

DuokanScreen::~DuokanScreen()
{
    if (memory_)
    {
        delete [] memory_;
        memory_ = 0;
    }
}

// Ref: http://doc.trolltech.com/4.2/qtopiacore-svgalib.html
// http://doc.trolltech.com/main-snapshot/ipc-sharedmemory-dialog-cpp.html
bool DuokanScreen::connect(const QString &displaySpec)
{
    Q_UNUSED(displaySpec);

    // Read environment variables.
    int width = qgetenv("SCREEN_WIDTH").toInt();
    int height = qgetenv("SCREEN_HEIGHT").toInt();

    // Fallback if not available.
    if (width <= 0)
    {
        width = DEFAULT_WIDTH;
    }
    if (height <= 0)
    {
        height = DEFAULT_HEIGHT;
    }

    QScreen::lstep = width;
    QScreen::w = width;
    QScreen::h = height;
    QScreen::dw = width;
    QScreen::dh = height;

    QScreen::d = 8;                     // Color depth.
    QScreen::size = width * height;
    QScreen::grayscale = true;
    QScreen::screencols = 16;

    setPixelFormat(QImage::Format_Indexed8);

    for(int i = 0; i < 256; ++i)
        screenclut[i] = qRgb(i, i, i);

    // When we can not create the shared memory, it could be caused that system manager crashed
    // or killed. On Linux, the shared memory is still there, so we can try to attach to the
    // existing shared memory.
    memory_ = new uchar[QScreen::size];
    QScreen::data = memory_;

    // Maybe need to change the dpi later.
    const int dpi = 72;
    QScreen::physWidth = qRound(QScreen::dw * 25.4 / dpi);
    QScreen::physHeight = qRound(QScreen::dh * 25.4 / dpi);
    return true;
}

bool DuokanScreen::initDevice()
{
    return true;
}

void DuokanScreen::shutdownDevice()
{
}

void DuokanScreen::disconnect()
{
}

void DuokanScreen::exposeRegion(QRegion r, int changing)
{
    QScreen::exposeRegion(r, changing);
}

//void DuokanScreen::blit(const QImage &img, const QPoint &topLeft, const QRegion &reg)
//{
//    bool do_fallback = true;
//    static QTime t;
//    if (screen_debug)
//    {
//        t.start();
//    }
//
//    // Only handle the indexed 8, as the eink screen works best with that.
//    // Not sure yet. It should be gray color instead of indexed 8.
//     qDebug("blit img format %d image color depth %d format %d",
//             img.format(), img.depth(), img.format());
//    if (depth() == 8 && img.format() == QImage::Format_Indexed8)
//        do_fallback = false;
//
//    //if (do_fallback)
//    {
//        qDebug( "call QScreen::blit now topLeft (%d,%d)- (%d,%d)",
//                 topLeft.x(), topLeft.y(),
//                 reg.boundingRect().width(), reg.boundingRect().height());
//        QScreen::blit(img, topLeft, reg);
//    }
//    /*else
//    {
//        qDebug("Implement it later.");
//    }*/
//
//    if (screen_debug)
//    {
//        qDebug("blit time %d", t.elapsed());
//    }
//}

void DuokanScreen::solidFill(const QColor &color, const QRegion &region)
{
    //qDebug("DuokanScreen::solidFill  ");
    if (depth() != 8)
    {
        qWarning("Fallback to QScreen as the depth is not 8 %d", depth());
        QScreen::solidFill(color, region);
        return;
    }

    unsigned char *base = QScreen::data;
    if (base == 0)
    {
        qWarning("Framebuffer is empty.");
        return;
    }

    unsigned char gray = (qGray(color.rgba()) & 0xff);
    const QVector<QRect> rects = region.rects();
    for (int i = 0; i < rects.size(); ++i)
    {
        const QRect r = rects.at(i);

        for(int y = r.top(); y < r.top() + r.height(); ++y)
        {
            unsigned char *p = base + y * QScreen::w + r.left();
            memset(p, r.width(), gray);
        }
    }
    // qDebug("DuokanScreen::solidFill done");
}

void DuokanScreen::blit16To4(const QImage &image,
                             const QPoint &topLeft, const QRegion &region)
{
    const int imageStride = image.bytesPerLine() / 2;
    const QVector<QRect> rects = region.rects();
    int scolor ;

    for (int i = 0; i < rects.size(); ++i) {
        const QRect r = rects.at(i).translated(-topLeft);
        int y = r.y();
        const quint16 *s = reinterpret_cast<const quint16*>(image.scanLine(y));

        while (y <= r.bottom()) {
            int x1 = r.x();
            while (x1 <= r.right()) {
                const quint16 c = s[x1];
                int x2 = x1;
                // find span length
                while ((x2+1 < r.right()) && (s[x2+1] == c))
                    ++x2;

                scolor = alloc((c>>11) & 0x1f, (c>>5) & 0x3f, c & 0x1f) ;

                k4_hline(x1 + topLeft.x(), y + topLeft.y(), x2 + topLeft.x(), scolor);

                x1 = x2 + 1;
            }
            s += imageStride;
            ++y;
        }
    }
}

void DuokanScreen::blit12To4(const QImage &image,
                             const QPoint &topLeft, const QRegion &region)
{
    const int imageStride = image.bytesPerLine() / 2;
    const QVector<QRect> rects = region.rects();
    int scolor ;

    for (int i = 0; i < rects.size(); ++i) {
        const QRect r = rects.at(i).translated(-topLeft);
        int y = r.y();
        const quint16 *s = reinterpret_cast<const quint16*>(image.scanLine(y));

        while (y <= r.bottom()) {
            int x1 = r.x();
            while (x1 <= r.right()) {
                const quint16 c = s[x1];
                int x2 = x1;
                // find span length
                while ((x2+1 < r.right()) && (s[x2+1] == c))
                    ++x2;

                scolor = alloc((c>>8) & 0x0f, (c>>4) & 0x0f, c & 0x0f) ;

                k4_hline(x1 + topLeft.x(), y + topLeft.y(), x2 + topLeft.x(), scolor);

                x1 = x2 + 1;
            }
            s += imageStride;
            ++y;
        }
    }
}


void DuokanScreen::blit32To4(const QImage &image,
                             const QPoint &topLeft, const QRegion &region)
{
    const int imageStride = image.bytesPerLine() / 4;
    const QVector<QRect> rects = region.rects();
    int scolor ;

    for (int i = 0; i < rects.size(); ++i) {
        const QRect r = rects.at(i).translated(-topLeft);
        int y = r.y();
        const quint32 *s = reinterpret_cast<const quint32*>(image.scanLine(y));

        while (y <= r.bottom()) {
            int x1 = r.x();
            while (x1 <= r.right()) {
                const quint32 c = s[x1];
                int x2 = x1;
                // find span length
                while ((x2+1 < r.right()) && (s[x2+1] == c))
                    ++x2;

                scolor = alloc(qRed(c), qGreen(c), qBlue(c)) ;

                k4_hline(x1 + topLeft.x(), y + topLeft.y(), x2 + topLeft.x(), scolor);

                x1 = x2 + 1;
            }
            s += imageStride;
            ++y;
        }
    }
}

void DuokanScreen::blit(const QImage& image, const QPoint& topLeft, const QRegion& region)
{
    QImage imageInverted = image;

    //if (!isKindle5 && !isKindleTouch)
    //    imageInverted.invertPixels();

#ifdef DEBUG_OUTPUT
    if (debugMode)
        qDebug(">>>>%s%s blit image %dx%dx%d type=%d", (isKindle4)? "K4":"", (isKindle5)? "K5":"",image.width(),image.height(), image.depth() ,image.format()) ;
#endif

    //if ((isKindle4) || (isKindle5))
    {
        blit_K4( imageInverted, topLeft, region);
    }
    /*else
    {
        QScreen::blit(imageInverted, topLeft, region);
    }*/
}

void DuokanScreen::blit_K4(const QImage &img, const QPoint &topLeft,
                        const QRegion &reg)
{

    switch (img.format()) {
    case QImage::Format_RGB16:
        blit16To4(img, topLeft, reg);
        return;
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
        blit32To4(img, topLeft, reg);
        return;
    case QImage::Format_RGB444:
        blit12To4(img, topLeft, reg);
        return;
    default:
        break;
    }

    QScreen::blit(img, topLeft, reg);
}

void DuokanScreen::k4_hline(int x1, int y, int x2, int c)
{
    uchar *p = base() + y*linestep() + x1 ;

    c = ((c<<4)+(c & 0x0f)) ;

    for (; x1 <= x2; x1++)
        *p++ = (uchar)  c ;
}





