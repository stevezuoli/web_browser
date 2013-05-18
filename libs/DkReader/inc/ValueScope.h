#ifndef _DKREADER_COMMON_VALUESCOPE_H_
#define _DKREADER_COMMON_VALUESCOPE_H_
namespace DkFormat
{
template<typename T>
class ValueScope
{
public:
    template <typename V>
    ValueScope(T& var, V const& set)
    :m_var(var), m_revert(var)
    {
        m_var = set;
    }
    template <typename V1, typename V2>
    ValueScope(T& var, V1 const& set, V2 const& revert)
    :m_var(var), m_revert(revert)
    {
        m_var = set;
    }
    ~ValueScope()
    {
        m_var = m_revert;
    }
private:
    ValueScope(const ValueScope&);
    ValueScope& operator=(const ValueScope&);
    T& m_var;
    T m_revert;
};
}
#endif

