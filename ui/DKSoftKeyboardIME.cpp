#include "ui/DKSoftKeyboardIME.h"
#include "ui/DKPushButton.h"
#include "ui/WindowsMetrics.h"
#include "ui/ImageManager.h"

namespace ui
{
using namespace windowsmetrics;

const int DKSoftKeyboardIME::s_btnsCountPerPage = 36;
const int DKSoftKeyboardIME::s_shiftBtnIndex = 19;
const int DKSoftKeyboardIME::s_delBtnIndex = 27;
const int DKSoftKeyboardIME::s_numBtnIndex = 28;
const int DKSoftKeyboardIME::s_spaceBtnIndex = 30;
const int DKSoftKeyboardIME::s_comBtnIndex = 31;
const int DKSoftKeyboardIME::s_langBtnIndex = 34;
const int DKSoftKeyboardIME::s_enterBtnIndex = 35;
const int DKSoftKeyboardIME::s_btnsEndIndex[s_keyboardRows] = {9, 18, 27, 35};

static const char* s_keyTables[DKSoftKeyboardIME::SKT_Count][DKSoftKeyboardIME::s_btnsCountPerPage] =
{
    // english lower
    {
        "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", 
        "a", "s", "d", "f", "g", "h", "j", "k", "l", 
        "", "z", "x", "c", "v", "b", "n", "m", "",
        "123!?", "@", " ", ".com", "/", ".", "", ""
    }, 
    // english upper
    {
        "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", 
        "A", "S", "D", "F", "G", "H", "J", "K", "L", 
        "", "Z", "X", "C", "V", "B", "N", "M", "",
        "123!?", "@", " ", ".com", "/", ".", "", "" 
    }, 
    // english number
    {
        "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", 
        "$", "&", "(", ")", "\"", "'", "-", "+", "/", 
        "#=~", "!", "?", ":", ";", ",", "_", "…", "",
        "ABC", "@", " ", ".com", "/", ".", "", "" 
    }, 
    // english symbol
    {
        "`",  "*",  "#",  "￥",  "%",  "|",  "\\",  "×",  "‘",  "’", 
        "=",  "~",  "[",  "]",  "{",  "}",  "<",  ">", "^", 
        "123!?", "™",  "§",  "®",  "¢",  "£",  "€",  "©", ""
        "ABC", "@", " ", ".com", "/", ".", "", "" 
    },

    // chinese lower
    {
        "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", 
        "a", "s", "d", "f", "g", "h", "j", "k", "l", 
        "", "z", "x", "c", "v", "b", "n", "m", "",
        "123!?", "。", " ", ".com", "/", "，", "", "" 
    }, 
    // chinese upper
    {
        "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", 
        "A", "S", "D", "F", "G", "H", "J", "K", "L", 
        "", "Z", "X", "C", "V", "B", "N", "M", "",
        "123!?", "。", " ", ".com", "/", "，", "", "" 
    },
    // chinese number
    {
        "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", 
        "$", "&", "（", "）", "“", "‘", "-", "+", "/", 
        "#=~", "！", "？", "：", "；", "@",  "_",  "……", "",
        "ABC", "。", " ", ".com", "/", "，", "", "" 
    }, 
    // chinese symbol
    {
        "`",  "、",  "#",  "￥",  "%",  "|",  "\\",  "×",  "“",  "”", 
        "—",  "‘",  "’",  "=",  "~",  "·",  "{",  "}", "^", 
        "123!?", "【",  "】",  "《",  "》",  "<",  ">",  "*", "",
        "ABC", "。", " ", ".com", "/", "，", "", "" 
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
        btn->setBackGroundImagePaths(ImageManger::GetImagePath(IMAGE_NORMAL_IME_KEY), ImageManger::GetImagePath(IMAGE_NORMAL_IME_KEY_PRESSED));
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
    for (int i = 0; i < s_btnsCountPerPage; ++i)
    {
        m_keyboardBtns.button(i)->setText(s_keyTables[newType][i]); 
    }
}

void DKSoftKeyboardIME::setupKeyboardWithType(SoftKeyboardType oldType, SoftKeyboardType newType)
{
}

void DKSoftKeyboardIME::onButtonClicked(int index)
{
    QString text = s_keyTables[m_currentType][index];
    postKeyEvent(index, text);
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
                                        {"",
                                         ImageManger::GetImagePath(IMAGE_IME_KEY_DEL),
                                         ImageManger::GetImagePath(IMAGE_MID_IME_KEY),
                                         ImageManger::GetImagePath(IMAGE_IME_KEY_SPACE),
                                         ImageManger::GetImagePath(IMAGE_MID_IME_KEY),
                                         ImageManger::GetImagePath(IMAGE_IME_KEY_LANG_EN),
                                         ImageManger::GetImagePath(IMAGE_IME_KEY_ENTER)};
    static QString focusOutBackGoundPaths[specialBtnCounts] = 
                                        {"",
                                         ImageManger::GetImagePath(IMAGE_IME_KEY_DEL_PRESSED),
                                         ImageManger::GetImagePath(IMAGE_MID_IME_KEY_PRESSED),
                                         ImageManger::GetImagePath(IMAGE_IME_KEY_SPACE_PRESSED),
                                         ImageManger::GetImagePath(IMAGE_MID_IME_KEY_PRESSED),
                                         ImageManger::GetImagePath(IMAGE_IME_KEY_LANG_EN_PRESSED),
                                         ImageManger::GetImagePath(IMAGE_IME_KEY_ENTER_PRESSED)};
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
}

