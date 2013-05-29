#ifndef __UI_DKSOFTKEYBOARDIME_H__
#define __UI_DKSOFTKEYBOARDIME_H__
#include <QtGui>

namespace ui
{
class DKSoftKeyboardIME : public QWidget
{
    Q_OBJECT

public:
    static const int s_btnsCountPerPage;
    static const int s_shiftBtnIndex;
    static const int s_delBtnIndex;
    static const int s_numBtnIndex;
    static const int s_spaceBtnIndex;
    static const int s_comBtnIndex;
    static const int s_langBtnIndex;
    static const int s_enterBtnIndex;
    static const int s_keyboardRows = 4;
    static const int s_btnsEndIndex[s_keyboardRows];

    static DKSoftKeyboardIME* GetInstance();
    enum SoftKeyboardType
    {
        SKT_EnglishLower,
        SKT_EnglishUpper,
        SKT_EnglishNumber,
        SKT_EnglishSymbol,
        SKT_ChineseLower,
        SKT_ChineseUpper,
        SKT_ChineseNumber,
        SKT_ChineseSymbol,
        SKT_Count
    };

    DKSoftKeyboardIME();

protected:
    virtual void paintEvent(QPaintEvent* e);

signals:
    void textInput(const QString& text);
    void enterPressed();
    void delPressed();

private slots:
    void onButtonClicked(int);

private:
    Q_DISABLE_COPY(DKSoftKeyboardIME)

    void InitUI();
    void InitLayout();
    void InitSpecialBtns();

    void setupKeyboardWithType(SoftKeyboardType newType);
    //fast transform
    void setupKeyboardWithType(SoftKeyboardType oldType, SoftKeyboardType newType);

private:
    SoftKeyboardType m_currentType;
    QButtonGroup m_keyboardBtns;
    QHBoxLayout m_rowLayouts[s_keyboardRows];
    QVBoxLayout m_mainLayout;
};
}//ui
#endif//__UI_DKSOFTKEYBOARDIME_H__
