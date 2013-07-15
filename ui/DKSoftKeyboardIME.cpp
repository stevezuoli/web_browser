#include "ui/DKSoftKeyboardIME.h"
#include "ui/DKPushButton.h"
#include "common/WindowsMetrics.h"
#include "common/ImageManager.h"
#include "common/debug.h"
#include "Device/device.h"
#include <algorithm>

using namespace ui::windowsmetrics;

namespace ui
{
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
        "$", "&", "(", ")", "\"", "'", "-", "+", "/", 
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
        "$", "&", "（", "）", "“", "‘", "-", "+", "/", 
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

DKSoftKeyboardIME::DKSoftKeyboardIME()
    : current_type_(SKT_EnglishLower)
      , row_layouts_(NULL)
      , main_layout_(NULL)
      , key_receiver_(NULL)
      , current_btn_index_(14)
{
    InitUI();
    InitLayout();
}

void DKSoftKeyboardIME::InitUI()
{
    setWindowFlags(Qt::Popup);
    setFocusPolicy(Qt::NoFocus);
    setVisible(false);

    for (int i = 0; i < s_btnsCountPerPage; ++i)
    {
        DKPushButton* btn = new DKPushButton(this);
        btn->setFocusPolicy(Qt::NoFocus);
        btn->setShapeType(DKPushButton::ST_Shadow);
        //btn->setBackGroundImagePaths(ImageManager::GetImagePath(IMAGE_NORMAL_IME_KEY_PRESSED), ImageManager::GetImagePath(IMAGE_NORMAL_IME_KEY));
        btn->setFixedSize(GetWindowMetrics(UISoftKeyboardNormalBtnWidthIndex), GetWindowMetrics(UISoftKeyboardNormalBtnHeightIndex));
        keyboard_btns_.addButton(btn, i);
    }

    if (!Device::isTouch())
    {
        DKPushButton* btn = reinterpret_cast<DKPushButton*>(keyboard_btns_.button(current_btn_index_));
        if (btn)
        {
            btn->setPressed(true);
        }
    }

    InitSpecialBtns();

    setupKeyboardWithType(current_type_);
    connect(&keyboard_btns_, SIGNAL(buttonClicked(int)), this, SLOT(onButtonClicked(int)));

    int width = GetWindowMetrics(UIScreenWidthIndex);
    int height = GetWindowMetrics(UISoftKeyboardHeightIndex);
    setGeometry(0, GetWindowMetrics(UIScreenHeightIndex) - height, width, height);
}

void DKSoftKeyboardIME::InitLayout()
{
    Q_ASSERT(keyboard_btns_.buttons().count() == s_btnsCountPerPage);
    int line = 0;
    main_layout_ = new QVBoxLayout;
    row_layouts_ = new QHBoxLayout[s_keyboardRows];
    for (int i = 0; i < s_btnsCountPerPage; ++i)
    {
        if (i > s_btnsEndIndex[line])
        {
            ++line;
        }

        row_layouts_[line].addWidget(keyboard_btns_.button(i));
    }

    const int horiBorder = GetWindowMetrics(UISoftKeyboardHoriBorderIndex);
    const int bigHoriBorder = GetWindowMetrics(UISoftKeyboardBigHoriBorderIndex);
    const int mainSpace = GetWindowMetrics(UISoftKeyboardSpaceIndex);
    for (int i = 0; i < s_keyboardRows; ++i)
    {
        row_layouts_[i].insertSpacing(0, i == 1 ? bigHoriBorder : horiBorder);
        row_layouts_[i].addSpacing(i == 1 ? bigHoriBorder : horiBorder);
        main_layout_->addLayout(row_layouts_ + i);
    }

    main_layout_->setSpacing(mainSpace);
    setLayout(main_layout_);
}

void DKSoftKeyboardIME::setupKeyboardWithType(SoftKeyboardType newType)
{
    Q_ASSERT(keyboard_btns_.buttons().count() == s_btnsCountPerPage);
    int index = getKeyBoardIndex(current_type_);
    if ( index >= 0)
    {
        for (int i = 0; i < s_btnsCountPerPage; ++i)
        {
            keyboard_btns_.button(i)->setText(QString::fromUtf8(s_keyTables[index][i])); 
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
                int keyboardIndex = getKeyBoardIndex(current_type_);
                if (keyboardIndex >= 0)
                {
                    postKeyEvent(index, getTextByBtnIndex(keyboardIndex, index));
                }
            }
            return;
    }

    updateSpecialBtn();
}

QString DKSoftKeyboardIME::getTextByBtnIndex(int keyboardIndex, int index)
{
    if (keyboardIndex < 0)
    {
        return QString();
    }

    QString text = QString::fromUtf8(s_keyTables[keyboardIndex][index]);
    //if (index == s_ampersandBtnIndex && (current_type_ == SKT_EnglishDigit || current_type_ == SKT_ChineseDigit))
    //{
        //text = QString::fromUtf8("&");
    //}
    return text;
}

void DKSoftKeyboardIME::postKeyEvent(unsigned int code, const QString& text)
{
    if (key_receiver_)
    {
        int key = Qt::Key_Any;
        switch (code)
        {
            case s_delBtnIndex:
                key = Qt::Key_Backspace;
                break;
            case s_enterBtnIndex:
                {
                    key = Qt::Key_Return;
                    QKeyEvent* keyEvent = new QKeyEvent(QEvent::KeyPress, key, Qt::NoModifier, QString(QChar(0x0D)));
                    QApplication::postEvent(key_receiver_, keyEvent);
                    return;
                }
            default:
                break;
        }
        QKeyEvent* keyEvent = new QKeyEvent(QEvent::KeyPress, key, Qt::NoModifier, text);
        QApplication::postEvent(key_receiver_, keyEvent);
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

void DKSoftKeyboardIME::keyPressEvent(QKeyEvent* event)
{
    switch (event->key())
    {
        case Qt::Key_Left:
        case Qt::Key_Up:
        case Qt::Key_Right:
        case Qt::Key_Down:
            onArrowKeyPressed(event->key());
            return;
#ifndef Q_WS_QWS
        case Qt::Key_F1:
#else
        case Qt::Key_Select:
#endif
            onOkKeyPressed();
            return;
#ifndef Q_WS_QWS
        case Qt::Key_F2:
#else
        case Qt::Key_AltGr:
#endif
            setVisible(false);
            return;
    }
    event->ignore();
}

void DKSoftKeyboardIME::InitSpecialBtns()
{
    static const int specialBtnCounts = 5;
    static int specialIndex[specialBtnCounts] = {s_shiftBtnIndex, s_delBtnIndex/*, s_numBtnIndexi*/, s_spaceBtnIndex/*, s_comBtnIndex*/, s_langBtnIndex, s_enterBtnIndex};
    static QString focusOutBackGoundPaths[specialBtnCounts] = 
    {ImageManager::GetImagePath(IMAGE_IME_KEY_LOWER),
        ImageManager::GetImagePath(IMAGE_IME_KEY_DEL),
        //ImageManager::GetImagePath(IMAGE_MID_IME_KEY),
        ImageManager::GetImagePath(IMAGE_IME_KEY_SPACE),
        //ImageManager::GetImagePath(IMAGE_MID_IME_KEY),
        ImageManager::GetImagePath(IMAGE_IME_KEY_LANG_EN),
        ImageManager::GetImagePath(IMAGE_IME_KEY_ENTER)};
    static QString focusInBackGoundPaths[specialBtnCounts] = 
    {ImageManager::GetImagePath(IMAGE_IME_KEY_LOWER_PRESSED),
        ImageManager::GetImagePath(IMAGE_IME_KEY_DEL_PRESSED),
        //ImageManager::GetImagePath(IMAGE_MID_IME_KEY_PRESSED),
        ImageManager::GetImagePath(IMAGE_IME_KEY_SPACE_PRESSED),
        //ImageManager::GetImagePath(IMAGE_MID_IME_KEY_PRESSED),
        ImageManager::GetImagePath(IMAGE_IME_KEY_LANG_EN_PRESSED),
        ImageManager::GetImagePath(IMAGE_IME_KEY_ENTER_PRESSED)};
    for (int i = 0; i < specialBtnCounts; ++i)
    {
        DKPushButton* btn = reinterpret_cast<DKPushButton*>(keyboard_btns_.button(specialIndex[i]));
        if (btn)
        {
            QPixmap pixmap(focusInBackGoundPaths[i]);
            btn->setFixedSize(pixmap.size());
            btn->setBackGroundImagePaths(focusInBackGoundPaths[i], focusOutBackGoundPaths[i]);
        }
    }

    keyboard_btns_.button(s_langBtnIndex)->setEnabled(false);
    DKPushButton* btn = reinterpret_cast<DKPushButton*>(keyboard_btns_.button(s_langBtnIndex));
    if (btn)
    {
        btn->setDisableBackGroundImagePath(ImageManager::GetImagePath(IMAGE_IME_KEY_LANG_EN_DISABLE));
    }
    keyboard_btns_.button(s_numBtnIndex)->setFixedSize(QPixmap(ImageManager::GetImagePath(IMAGE_MID_IME_KEY)).size());
    keyboard_btns_.button(s_comBtnIndex)->setFixedSize(QPixmap(ImageManager::GetImagePath(IMAGE_MID_IME_KEY)).size());

}

void DKSoftKeyboardIME::updateShiftBtn()
{
    DKPushButton* btn = reinterpret_cast<DKPushButton*>(keyboard_btns_.button(s_shiftBtnIndex));
    if (btn)
    {
        if (current_type_ & SKT_Alpha)
        {
            btn->setText("");
            if (current_type_ & SKT_Lower)
            {
                btn->setBackGroundImagePaths(ImageManager::GetImagePath(IMAGE_IME_KEY_UPPER_PRESSED), ImageManager::GetImagePath(IMAGE_IME_KEY_UPPER));
            }
            else if (current_type_ & SKT_Upper)
            {
                btn->setBackGroundImagePaths(ImageManager::GetImagePath(IMAGE_IME_KEY_LOWER_PRESSED), ImageManager::GetImagePath(IMAGE_IME_KEY_LOWER));
            }
        }
        else if (current_type_ & SKT_DigSym)
        {
            //btn->setBackGroundImagePaths(ImageManager::GetImagePath(IMAGE_MID_IME_KEY_PRESSED), ImageManager::GetImagePath(IMAGE_MID_IME_KEY));
            btn->setBackGroundImagePaths("", "");
            btn->setShapeType(DKPushButton::ST_Shadow);
            if (current_type_ & SKT_Digit)
            {
                btn->setText(s_symbol);
            }
            else if (current_type_ & SKT_Symbol)
            {
                btn->setText(s_digit);
            }
        }
    }
}

void DKSoftKeyboardIME::updateLangBtn()
{
    DKPushButton* btn = reinterpret_cast<DKPushButton*>(keyboard_btns_.button(s_langBtnIndex));
    if (btn)
    {
        if (current_type_ & SKT_English)
        {
            btn->setBackGroundImagePaths(ImageManager::GetImagePath(IMAGE_IME_KEY_LANG_EN_PRESSED), ImageManager::GetImagePath(IMAGE_IME_KEY_LANG_EN));
        }
        else if (current_type_ & SKT_Chinese)
        {
            btn->setBackGroundImagePaths(ImageManager::GetImagePath(IMAGE_IME_KEY_LANG_CN_PRESSED), ImageManager::GetImagePath(IMAGE_IME_KEY_LANG_CN));
        }
    }
}

void DKSoftKeyboardIME::updateDigitBtn()
{
    DKPushButton* btn = reinterpret_cast<DKPushButton*>(keyboard_btns_.button(s_numBtnIndex));
    if (btn)
    {
        if (current_type_ & SKT_Alpha)
        {
            btn->setText(s_digit);
        }
        else if (current_type_ & SKT_DigSym)
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
    int all = SKT_Lower | SKT_Upper;
    current_type_ = (SoftKeyboardType)((current_type_ & ~all) | (~(current_type_ & all) & all));
    setupKeyboardWithType(current_type_);
}

void DKSoftKeyboardIME::onLangBtnClicked()
{
    current_type_ = (SoftKeyboardType)((current_type_ & ~SKT_Lang) | (~(current_type_ & SKT_Lang) & SKT_Lang));
    setupKeyboardWithType(current_type_);
}

void DKSoftKeyboardIME::onDigitBtnClicked()
{
    int all = SKT_Alpha | SKT_DigSym;
    current_type_ = (SoftKeyboardType)((current_type_ & ~all) | (~(current_type_ & all) & all));
    setupKeyboardWithType(current_type_);
}

void DKSoftKeyboardIME::setVisible(bool visible)
{
    if (visible != isVisible())
    {
        if (visible)
        {
            grabKeyboard();
        }
        else
        {
            releaseKeyboard();
        }
    }

    QWidget::setVisible(visible);
}

void DKSoftKeyboardIME::onArrowKeyPressed(int key)
{
    DKPushButton* btn = reinterpret_cast<DKPushButton*>(keyboard_btns_.button(current_btn_index_));
    if (btn)
    {
        btn->setPressed(false);
    }
    switch (key)
    {
        case Qt::Key_Left:
        case Qt::Key_Right:
            handlePrevNextFocus(key == Qt::Key_Right);
            break;
        case Qt::Key_Up:
        case Qt::Key_Down:
            handleAdjacentLineFocus(key == Qt::Key_Down);
            break;
        default:
            break;
    }
}
void DKSoftKeyboardIME::handlePrevNextFocus(bool next)
{
    next ? current_btn_index_++ : current_btn_index_--;
    current_btn_index_ = current_btn_index_ >= 0 ? current_btn_index_ : s_btnsCountPerPage - 1;
    current_btn_index_ = current_btn_index_ < s_btnsCountPerPage ? current_btn_index_ : 0;
    //hack: skip the langbtn
    if (current_btn_index_ == s_langBtnIndex && !keyboard_btns_.button(s_langBtnIndex)->isEnabled())
    {
        next ? current_btn_index_++ : current_btn_index_--;
    }
    DKPushButton* btn = reinterpret_cast<DKPushButton*>(keyboard_btns_.button(current_btn_index_));
    if (btn)
    {
        btn->setPressed(true);
    }
}

void DKSoftKeyboardIME::handleAdjacentLineFocus(bool next)
{
    int line = 0;
    for (; current_btn_index_ > s_btnsEndIndex[line] && line < s_keyboardRows; ++line)
        ;
    if (line >= s_keyboardRows)
        return;

    int relativeIndex = line == 0 ? current_btn_index_ : current_btn_index_ - s_btnsEndIndex[line - 1];

    next ? line++ : line--;
    line = line >= 0 ? line : s_keyboardRows - 1;
    line = line < s_keyboardRows ? line : 0;

    current_btn_index_ = line == 0 ? relativeIndex : relativeIndex + s_btnsEndIndex[line - 1];
    current_btn_index_ = current_btn_index_ <= s_btnsEndIndex[line] ? current_btn_index_ : s_btnsEndIndex[line];
    //hack: skip the langbtn
    if (current_btn_index_ == s_langBtnIndex && !keyboard_btns_.button(s_langBtnIndex)->isEnabled())
    {
        current_btn_index_++;
    }
    DKPushButton* btn = reinterpret_cast<DKPushButton*>(keyboard_btns_.button(current_btn_index_));
    if (btn)
    {
        btn->setPressed(true);
    }
}

void DKSoftKeyboardIME::onOkKeyPressed()
{
    onButtonClicked(current_btn_index_);
}
}

