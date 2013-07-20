#ifndef DUOKAN_BUTTONS_H_
#define DUOKAN_BUTTONS_H_

#include <QtGui/QtGui>

namespace framework
{

/// Check box for eink device. Remove unnecessary updates.
class DuokanCheckBox : public QCheckBox
{
public:
    DuokanCheckBox(const QString & text, QWidget *parent);
    ~DuokanCheckBox();

    void selectOnClicked(bool enable = true);
    bool isSelectOnClicked() { return select_on_clicked_; }

    void useDefaultHeight();
    static int defaultHeight();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent * event);
    bool event(QEvent *e);

private:
    bool select_on_clicked_;
};

/// PushButton for eink device. Remove unnecessary updates.
class DuokanPushButton : public QPushButton
{
public:
    DuokanPushButton(const QString & title, QWidget *parent);
    DuokanPushButton( const QIcon & icon, const QString & text, QWidget * parent = 0 );
    ~DuokanPushButton();

public:
    void setData(const QVariant & data);
    QVariant & data();

    const QString & defaultStyle();

    void useDefaultHeight();
    static int defaultHeight();

    void enableScreenUpdate(bool enable = true) { screen_update_ = enable; }

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent * event);
    virtual bool event(QEvent *e);

private:
    QVariant data_;
    bool screen_update_;   ///< Enable it to update screen or not.
};


/// Image widget.
class ImageWidget : public QWidget
{
    Q_OBJECT
public:
    ImageWidget(QImage image, QWidget *parent);
    ~ImageWidget();

public:
    QVariant data() { return data_; }
    void setData(const QVariant & data) { data_ = data; }

    bool isChecked() { return check_; }
    void setChecked(bool check);

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyReleaseEvent(QKeyEvent *);
    void paintEvent(QPaintEvent *pe);

Q_SIGNALS:
    void clicked(ImageWidget *);

private:
    QImage image_;
    QVariant data_;
    bool check_;
};

};

#endif  // DUOKAN_BUTTONS_H_
