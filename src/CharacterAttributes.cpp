#include "CharacterAttributes.h"


Attributes::Attributes(const sbyte str /*= 0*/, const sbyte str_add /*= 0*/,
                     const sbyte intel /*= 0*/, const sbyte wis /*= 0*/,
                     const sbyte dex /*= 0*/, const sbyte con /*= 0*/,
                     const sbyte cha /*= 0*/)
:    m_str(str), m_str_add(str_add), m_intel(intel), m_wis(wis),
    m_dex(dex), m_con(con), m_cha(cha)
{
}

Attributes::Attributes(const Attributes &orig)
:    m_str(orig.getStr()), m_str_add(orig.getStrAdd()), m_intel(orig.getIntel()),
    m_wis(orig.getWis()), m_dex(orig.getDex()), m_con(orig.getCon()),
    m_cha(orig.getCha())
{
}

Attributes::~Attributes()
{
}

const Attributes &Attributes::operator=(const Attributes &rhs)
{
    m_str = rhs.getStr();
    m_str_add = rhs.getStrAdd();
    m_dex = rhs.getDex();
    m_con = rhs.getCon();
    m_intel = rhs.getIntel();
    m_wis = rhs.getWis();
    m_cha = rhs.getCha();

    return *this;
}
