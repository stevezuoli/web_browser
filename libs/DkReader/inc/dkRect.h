#ifndef _DKREADER_INC_DKRECT_H_
#define _DKREADER_INC_DKRECT_H_
#include <algorithm>

namespace DkFormat
{
class DkRect;
DkRect operator&(const DkRect& lhs, const DkRect& rhs);
DkRect operator|(const DkRect& lhs, const DkRect& rhs);

class DkRect
{
public:
    DkRect()
        :m_left(0), m_top(0), m_width(0), m_height(0)
    {
    }
    friend DkRect operator&(const DkRect& lhs, const DkRect& rhs);
    friend DkRect operator|(const DkRect& lhs, const DkRect& rhs);

    static DkRect FromLeftTopWidthHeight(int left, int top, int width, int height)
    {
        return DkRect(left, top, width, height);
    }
    static DkRect FromLeftTopRightBottom(int left, int top, int right, int bottom)
    {
        return FromLeftTopWidthHeight(left, top, right - left, bottom - top);
    }

    int Left() const
    {
        return m_left;
    }
    int Width() const
    {
        return m_width;
    }
    int Right() const
    {
        return m_left + m_width;
    }
    int Top() const
    {
        return m_top;
    }
    int Height() const
    {
        return m_height;
    }
    int Bottom() const
    {
        return m_top + m_height;
    }

    bool IsEmpty() const
    {
        return m_width <= 0 || m_height <= 0;
    }
    bool operator==(const DkRect& rhs) const
    {
        if (this == &rhs)
            return true;
        return m_left == rhs.m_left
            && m_top == rhs.m_top
            && m_width == rhs.m_width
            && m_height == rhs.m_height;
    }
    bool operator!=(const DkRect& rhs) const
    {
        return !(*this == rhs);
    }
    const DkRect& operator &=(const DkRect& rhs)
    {
        *this = (*this) & rhs;
        return *this;
    }
    const DkRect& operator |=(const DkRect& rhs)
    {
        *this = (*this) | rhs;
        return *this;
    }
    void MakeEmpty()
    {
        m_left = 0;
        m_top = 0;
        m_width = 0;
        m_height = 0;
    }
private:
    DkRect(int left, int top, int width, int height)
        :m_left(left), m_top(top), m_width(width), m_height(height)
    {

    }
    int m_left, m_top, m_width, m_height;
};
inline DkRect operator|(const DkRect& lhs, const DkRect& rhs)
{
    return DkRect::FromLeftTopRightBottom(
            std::min(lhs.Left(), rhs.Left()), 
            std::min(lhs.Top(), rhs.Top()),
            std::max(lhs.Right(), rhs.Right()), 
            std::max(lhs.Bottom(), rhs.Bottom()));
}

inline DkRect operator&(const DkRect& lhs, const DkRect& rhs)
{
    return DkRect::FromLeftTopRightBottom(
            std::max(lhs.Left(), rhs.Left()), 
            std::max(lhs.Top(), rhs.Top()),
            std::min(lhs.Right(), rhs.Right()), 
            std::min(lhs.Bottom(), rhs.Bottom()));
}

}
#endif
