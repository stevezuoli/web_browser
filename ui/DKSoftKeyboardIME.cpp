#include "ui/DKSoftKeyboardIME.h"
#include "ui/DKPushButton.h"
#include "common/WindowsMetrics.h"
#include "common/ImageManager.h"
#include <algorithm>

namespace ui
{
using namespace windowsmetrics;

const int DKSoftKeyboardIME::s_btnsCountPerPage = 36;
const int DKSoftKeyboardIME::s_ampersandBtnIndex = 11;
const int DKSoftKeyboardIME::s_shiftBtnIndex = 19;
const int DKSoftKeyboardIME::s_delBtnIndex = 27;
const int DKSoftKeyboardIME::s_numBtnIndex = 28;
const int DKSoftKeyboardIME::s_spaceBtnIndex = 30;
const int DKSoftKeyboardIME::s_comBtnIndex = 31;
const int DKSoftKeyboardIME::s_langBtnIndex = 34;
const int DKSoftKeyboardIME::s_enterBtnIndex = 35;
const int DKSoftKeyboardIME::s_btnsEndIndex[s_keyboardRows] = {9, 18, 27, 35};

const char* s_symbol = "#=~";
const char* s_digit = "123!?";
const char* s_alpha = "ABC";
static const char* s_keyTables[][DKSoftKeyboardIME::s_btnsCountPerPage] =
{
    // english lower
    {
        "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", 
        "a", "s", "d", "f", "g", "h", "j", "k", "l", 
        "", "z", "x", "c", "v", "b", "n", "m", "",
        s_digit, "@", " ", ".com", "/", ".", "", ""
    }, 
    // english upper
    {
        "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", 
        "A", "S", "D", "F", "G", "H", "J", "K", "L", 
        "", "Z", "X", "C", "V", "B", "N", "M", "",
        s_digit, "@", " ", ".com", "/", ".", "", "" 
    }, 
    // english number
    {
        "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", 
        "$", "&&", "(", ")", "\"", "'", "-", "+", "/", 
        s_symbol, "!", "?", ":", ";", ",", "_", "…", "",
        s_alpha, "@", " ", ".com", "/", ".", "", "" 
    }, 
    // english symbol
    {
        "`",  "*",  "#",  "￥",  "%",  "|",  "\\",  "×",  "‘",  "’", 
        "=",  "~",  "[",  "]",  "{",  "}",  "<",  ">", "^", 
        s_digit, "™",  "§",  "®",  "¢",  "£",  "€",  "©", "",
        s_alpha, "@", " ", ".com", "/", ".", "", "" 
    },

    // chinese lower
    {
        "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", 
        "a", "s", "d", "f", "g", "h", "j", "k", "l", 
        "", "z", "x", "c", "v", "b", "n", "m", "",
        s_digit, "。", " ", ".com", "/", "，", "", "" 
    }, 
    // chinese upper
    {
        "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", 
        "A", "S", "D", "F", "G", "H", "J", "K", "L", 
        "", "Z", "X", "C", "V", "B", "N", "M", "",
        s_digit, "。", " ", ".com", "/", "，", "", "" 
    },
    // chinese number
    {
        "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", 
        "$", "&&", "（", "）", "“", "‘", "-", "+", "/", 
        s_symbol, "！", "？", "：", "；", "@",  "_",  "……", "",
        s_alpha, "。", " ", ".com", "/", "，", "", "" 
    }, 
    // chinese symbol
    {
        "`",  "、",  "#",  "￥",  "%",  "|",  "\\",  "×",  "“",  "”", 
        "—",  "‘",  "’",  "=",  "~",  "·",  "{",  "}", "^", 
        s_digit, "【",  "】",  "《",  "》",  "<",  ">",  "*", "",
        s_alpha, "。", " ", ".com", "/", "，", "", "" 
    }
};

DKSoftKeyboardIME* DKSoftKeyboardIME::GetInstance()
{
    static DKSoftKeyboardIME ime;
    return &ime;
}

DKSoftKeyboardIME::DKSoftKeyboardIME()
    : m_currentType(SKT_EnglishLower)
    , m_keyReceiver(NULL)
{
    InitUI();
    InitLayout();
}

void DKSoftKeyboardIME::InitUI()
{
    setWindowFlags(Qt::Popup);
    setFocusPolicy(Qt::NoFocus);

    for (int i = 0; i < s_btnsCountPerPage; ++i)
    {
        DKPushButton* btn = new DKPushButton(this);
        btn->setFocusPolicy(Qt::NoFocus);
        btn->setBackGroundImagePaths(ImageManager::GetImagePath(IMAGE_NORMAL_IME_KEY), ImageManager::GetImagePath(IMAGE_NORMAL_IME_KEY_PRESSED));
        btn->setFixedSize(GetWindowMetrics(UISoftKeyboardNormalBtnWidthIndex), GetWindowMetrics(UISoftKeyboardNormalBtnHeightIndex));
        m_keyboardBtns.addButton(btn, i);
    }

    InitSpecialBtns();

    setupKeyboardWithType(m_currentType);
    connect(&m_keyboardBtns, SIGNAL(buttonClicked(int)), this, SLOT(onButtonClicked(int)));

    int width = GetWindowMetrics(UIScreenWidthIndex);
    int height = GetWindowMetrics(UISoftKeyboardHeightIndex);
    setGeometry(0, GetWindowMetrics(UIScreenHeightIndex) - height, width, height);
}

void DKSoftKeyboardIME::InitLayout()
{
    Q_ASSERT(m_keyboardBtns.buttons().count() == s_btnsCountPerPage);
    int line = 0;
    for (int i = 0; i < s_btnsCountPerPage; ++i)
    {
        if (i > s_btnsEndIndex[line])
        {
            ++line;
        }

        m_rowLayouts[line].addWidget(m_keyboardBtns.button(i));
    }

    const int horiBorder = GetWindowMetrics(UISoftKeyboardHoriBorderIndex);
    const int bigHoriBorder = GetWindowMetrics(UISoftKeyboardBigHoriBorderIndex);
    const int mainSpace = GetWindowMetrics(UISoftKeyboardSpaceIndex);
    for (int i = 0; i < s_keyboardRows; ++i)
    {
        m_rowLayouts[i].insertSpacing(0, i == 1 ? bigHoriBorder : horiBorder);
        m_rowLayouts[i].addSpacing(i == 1 ? bigHoriBorder : horiBorder);
        m_mainLayout.addLayout(m_rowLayouts + i);
    }

    m_mainLayout.setSpacing(mainSpace);
    setLayout(&m_mainLayout);
}

void DKSoftKeyboardIME::setupKeyboardWithType(SoftKeyboardType newType)
{
    Q_ASSERT(m_keyboardBtns.buttons().count() == s_btnsCountPerPage);
    int index = getKeyBoardIndex(m_currentType);
    if ( index >= 0)
    {
        for (int i = 0; i < s_btnsCountPerPage; ++i)
        {
            m_keyboardBtns.button(i)->setText(QString::fromUtf8(s_keyTables[index][i])); 
        }
    }
}

int DKSoftKeyboardIME::getKeyBoardIndex(SoftKeyboardType type) const
{
    static int keyTableKind[] = {SKT_EnglishLower, SKT_EnglishUpper, SKT_EnglishDigit, SKT_EnglishSymbol, 
                               SKT_ChineseLower, SKT_ChineseUpper, SKT_ChineseDigit, SKT_ChineseSymbol};
    static int len = sizeof(keyTableKind)/sizeof(keyTableKind[0]);

    int index = std::find(keyTableKind, keyTableKind + len, type) - keyTableKind;
    if ( index != len)
    {
        return index;
    }

    return -1;
}

void DKSoftKeyboardIME::setupKeyboardWithType(SoftKeyboardType oldType, SoftKeyboardType newType)
{
}

void DKSoftKeyboardIME::onButtonClicked(int index)
{
    switch (index)
    {
        case s_shiftBtnIndex:
            onShiftBtnClicked();
            break;
        case s_numBtnIndex:
            onDigitBtnClicked();
            break;
        case s_langBtnIndex:
            onLangBtnClicked();
            break;
        default:
            {
                int keyboardIndex = getKeyBoardIndex(m_currentType);
                if (keyboardIndex >= 0)
                {
                    QString text = QString::fromUtf8(s_keyTables[keyboardIndex][index]);
                    if (index == s_ampersandBtnIndex && (m_currentType == SKT_EnglishDigit || m_currentType == SKT_ChineseDigit))
                    {
                        text = QString::fromUtf8("&");
                    }
                    postKeyEvent(index, text);
                }
            }
            return;
    }

    updateSpecialBtn();
}

void DKSoftKeyboardIME::postKeyEvent(unsigned int code, const QString& text)
{
    if (m_keyReceiver)
    {
        int key = Qt::Key_Any;
        switch (code)
        {
            case s_delBtnIndex:
                key = Qt::Key_Backspace;
                break;
            case s_enterBtnIndex:
                key = Qt::Key_Return;
                break;
            default:
                break;
        }
        QKeyEvent* keyEvent = new QKeyEvent(QEvent::KeyPress, key, Qt::NoModifier, text);
        QApplication::postEvent(m_keyReceiver, keyEvent);
    }
}

void DKSoftKeyboardIME::paintEvent(QPaintEvent* e)
{
    QPainter painter(this);
    QRect rc(rect());

    painter.fillRect(rc, QBrush(QColor(255,255,255)));

    QPen pen(QColor(0,0,0));
    pen.setWidth(2);
    painter.setPen(pen);
    QPainterPath path;
    path.addRoundedRect(rc.left() + 10, rc.top() + 2, rc.width() - 20, rc.height() - 2, 10, 10);
    painter.drawPath(path);
}

void DKSoftKeyboardIME::InitSpecialBtns()
{
    static const int specialBtnCounts = 7;
    static int specialIndex[specialBtnCounts] = {s_shiftBtnIndex, s_delBtnIndex, s_numBtnIndex, s_spaceBtnIndex, s_comBtnIndex, s_langBtnIndex, s_enterBtnIndex};
    static QString focusInBackGoundPaths[specialBtnCounts] = 
                                        {ImageManager::GetImagePath(IMAGE_IME_KEY_LOWER),
                                         ImageManager::GetImagePath(IMAGE_IME_KEY_DEL),
                                         ImageManager::GetImagePath(IMAGE_MID_IME_KEY),
                                         ImageManager::GetImagePath(IMAGE_IME_KEY_SPACE),
                                         ImageManager::GetImagePath(IMAGE_MID_IME_KEY),
                                         ImageManager::GetImagePath(IMAGE_IME_KEY_LANG_EN),
                                         ImageManager::GetImagePath(IMAGE_IME_KEY_ENTER)};
    static QString focusOutBackGoundPaths[specialBtnCounts] = 
                                        {ImageManager::GetImagePath(IMAGE_IME_KEY_LOWER_PRESSED),
                                         ImageManager::GetImagePath(IMAGE_IME_KEY_DEL_PRESSED),
                                         ImageManager::GetImagePath(IMAGE_MID_IME_KEY_PRESSED),
                                         ImageManager::GetImagePath(IMAGE_IME_KEY_SPACE_PRESSED),
                                         ImageManager::GetImagePath(IMAGE_MID_IME_KEY_PRESSED),
                                         ImageManager::GetImagePath(IMAGE_IME_KEY_LANG_EN_PRESSED),
                                         ImageManager::GetImagePath(IMAGE_IME_KEY_ENTER_PRESSED)};
    for (int i = 0; i < specialBtnCounts; ++i)
    {
        DKPushButton* btn = reinterpret_cast<DKPushButton*>(m_keyboardBtns.button(specialIndex[i]));
        if (btn)
        {
            QPixmap pixmap(focusInBackGoundPaths[i]);
            btn->setFixedSize(pixmap.size());
            btn->setBackGroundImagePaths(focusInBackGoundPaths[i], focusOutBackGoundPaths[i]);
        }
    }

}

void DKSoftKeyboardIME::updateShiftBtn()
{
    DKPushButton* btn = reinterpret_cast<DKPushButton*>(m_keyboardBtns.button(s_shiftBtnIndex));
    if (btn)
    {
        if (m_currentType & SKT_Alpha)
        {
            btn->setText("");
            if (m_currentType & SKT_Lower)
            {
                btn->setBackGroundImagePaths(ImageManager::GetImagePath(IMAGE_IME_KEY_UPPER), ImageManager::GetImagePath(IMAGE_IME_KEY_UPPER_PRESSED));
            }
            else if (m_currentType & SKT_Upper)
            {
                btn->setBackGroundImagePaths(ImageManager::GetImagePath(IMAGE_IME_KEY_LOWER), ImageManager::GetImagePath(IMAGE_IME_KEY_LOWER_PRESSED));
            }
        }
        else if (m_currentType & SKT_DigSym)
        {
            btn->setBackGroundImagePaths(ImageManager::GetImagePath(IMAGE_MID_IME_KEY), ImageManager::GetImagePath(IMAGE_MID_IME_KEY_PRESSED));
            if (m_currentType & SKT_Digit)
            {
                btn->setText(s_symbol);
            }
            else if (m_currentType & SKT_Symbol)
            {
                btn->setText(s_digit);
            }
        }
    }
}

void DKSoftKeyboardIME::updateLangBtn()
{
    DKPushButton* btn = reinterpret_cast<DKPushButton*>(m_keyboardBtns.button(s_langBtnIndex));
    if (btn)
    {
        if (m_currentType & SKT_English)
        {
            btn->setBackGroundImagePaths(ImageManager::GetImagePath(IMAGE_IME_KEY_LANG_EN), ImageManager::GetImagePath(IMAGE_IME_KEY_LANG_EN_PRESSED));
        }
        else if (m_currentType & SKT_Chinese)
        {
            btn->setBackGroundImagePaths(ImageManager::GetImagePath(IMAGE_IME_KEY_LANG_CN), ImageManager::GetImagePath(IMAGE_IME_KEY_LANG_CN_PRESSED));
        }
    }
}

void DKSoftKeyboardIME::updateDigitBtn()
{
    DKPushButton* btn = reinterpret_cast<DKPushButton*>(m_keyboardBtns.button(s_numBtnIndex));
    if (btn)
    {
        if (m_currentType & SKT_Alpha)
        {
            btn->setText(s_digit);
        }
        else if (m_currentType & SKT_DigSym)
        {
            btn->setText(s_alpha);
        }
    }
}

void DKSoftKeyboardIME::updateSpecialBtn()
{
    updateShiftBtn();
    updateLangBtn();
    updateDigitBtn();
}

void DKSoftKeyboardIME::onShiftBtnClicked()
{
    qDebug("enter DKSoftKeyboardIME::onShiftBtnClicked %d", m_currentType);
    int all = SKT_Lower | SKT_Upper;
    m_currentType = (SoftKeyboardType)((m_currentType & ~all) | (~(m_currentType & all) & all));
    setupKeyboardWithType(m_currentType);
    qDebug("leave DKSoftKeyboardIME::onShiftBtnClicked %d", m_currentType);
}

void DKSoftKeyboardIME::onLangBtnClicked()
{
    qDebug("enter DKSoftKeyboardIME::onLangBtnClicked %d", m_currentType);
    m_currentType = (SoftKeyboardType)((m_currentType & ~SKT_Lang) | (~(m_currentType & SKT_Lang) & SKT_Lang));
    setupKeyboardWithType(m_currentType);
    qDebug("leave DKSoftKeyboardIME::onLangBtnClicked %d", m_currentType);
}

void DKSoftKeyboardIME::onDigitBtnClicked()
{
    qDebug("enter DKSoftKeyboardIME::onDigitBtnClicked %d", m_currentType);
    int all = SKT_Alpha | SKT_DigSym;
    m_currentType = (SoftKeyboardType)((m_currentType & ~all) | (~(m_currentType & all) & all));
    setupKeyboardWithType(m_currentType);
    qDebug("leave DKSoftKeyboardIME::onDigitBtnClicked %d", m_currentType);
}
}

