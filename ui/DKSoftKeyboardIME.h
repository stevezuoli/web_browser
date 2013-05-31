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
    static const int s_ampersandBtnIndex;
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
        SKT_English = 0x1,
        SKT_Chinese = 0x2,
        SKT_Alpha = 0x10,
        SKT_DigSym = 0x20,
        SKT_Lower = 0x100,
        SKT_Upper = 0x200,
        SKT_Digit = SKT_Lower,
        SKT_Symbol = SKT_Upper,
        SKT_Lang = SKT_English | SKT_Chinese,
        SKT_EnglishLower = SKT_English | SKT_Lower | SKT_Alpha,
        SKT_EnglishUpper = SKT_English | SKT_Upper | SKT_Alpha,
        SKT_EnglishDigit = SKT_English | SKT_Digit | SKT_DigSym,
        SKT_EnglishSymbol = SKT_English | SKT_Symbol | SKT_DigSym,
        SKT_ChineseLower = SKT_Chinese | SKT_Lower | SKT_Alpha,
        SKT_ChineseUpper = SKT_Chinese | SKT_Upper | SKT_Alpha,
        SKT_ChineseDigit = SKT_Chinese | SKT_Digit | SKT_DigSym,
        SKT_ChineseSymbol = SKT_Chinese | SKT_Symbol | SKT_DigSym
    };

    DKSoftKeyboardIME();
    void attachReceiver(QObject* receiver)
    {
        m_keyReceiver = receiver;
    }

protected:
    virtual void paintEvent(QPaintEvent* e);

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
    void postKeyEvent(unsigned int code, const QString& text);
    int getKeyBoardIndex(SoftKeyboardType type) const;

    //btn's name in english-alpha status
    void updateSpecialBtn();
    void updateShiftBtn();
    void updateLangBtn();
    void updateDigitBtn();
    void onShiftBtnClicked();
    void onLangBtnClicked();
    void onDigitBtnClicked();

private:
    SoftKeyboardType m_currentType;
    QButtonGroup m_keyboardBtns;
    QHBoxLayout m_rowLayouts[s_keyboardRows];
    QVBoxLayout m_mainLayout;
    QObject* m_keyReceiver;
};
}//ui
#endif//__UI_DKSOFTKEYBOARDIME_H__
