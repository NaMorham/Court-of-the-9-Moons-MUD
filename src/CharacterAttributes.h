#ifndef __9MOONS__CHARACTERATTRIBUTES_H__
#define __9MOONS__CHARACTERATTRIBUTES_H__

#include "mudtypes.h"

/**
REPLACES
// Character abilities. Different instances of this structure are used for
// both inherent and current ability scores (like when poison affects the
// player strength).
struct char_ability_data
{
    sbyte str;     ///!< Strength.
    sbyte str_add; ///!< Strength multiplier if str = 18. Usually from 0 to 100
    sbyte intel;   ///!< Intelligence
    sbyte wis;     ///!< Wisdom
    sbyte dex;     ///!< Dexterity
    sbyte con;     ///!< Constitution
    sbyte cha;     ///!< Charisma
};
 */
class Attributes
{
public:
    Attributes(const sbyte str = 0, const sbyte str_add = 0,
              const sbyte intel = 0, const sbyte wis = 0,
              const sbyte dex = 0, const sbyte con = 0,
              const sbyte cha = 0);
    Attributes(const Attributes &orig);
    virtual ~Attributes();

    const Attributes &operator=(const Attributes &rhs);

    inline const sbyte getStr() const;
    inline const sbyte setStr(const sbyte str);

    inline const sbyte getStrAdd() const;
    inline const sbyte setStrAdd(const sbyte strAdd);

    inline const sbyte getIntel() const;
    inline const sbyte setIntel(const sbyte intel);

    inline const sbyte getWis() const;
    inline const sbyte setWis(const sbyte wis);

    inline const sbyte getDex() const;
    inline const sbyte setDex(const sbyte dex);

    inline const sbyte getCon() const;
    inline const sbyte setCon(const sbyte con);

    inline const sbyte getCha() const;
    inline const sbyte setCha(const sbyte cha);

private:

    sbyte m_str;     ///!< Strength.
    sbyte m_str_add; ///!< Strength multiplier if str = 18. Usually from 0 to 100
    sbyte m_intel;   ///!< Intelligence
    sbyte m_wis;     ///!< Wisdom
    sbyte m_dex;     ///!< Dexterity
    sbyte m_con;     ///!< Constitution
    sbyte m_cha;     ///!< Charisma
};

// inline functions
const sbyte Attributes::getStr() const
{
    return m_str;
}

const sbyte Attributes::setStr(const sbyte str)
{
    m_str = str;
    return m_str;
}

const sbyte Attributes::getStrAdd() const
{
    return m_str_add;
}

const sbyte Attributes::setStrAdd(const sbyte strAdd)
{
    m_str_add = strAdd;
    return m_str_add;
}

const sbyte Attributes::getIntel() const
{
    return m_intel;
}

const sbyte Attributes::setIntel(const sbyte intel)
{
    m_intel = intel;
    return m_intel;
}

const sbyte Attributes::getWis() const
{
    return m_wis;
}

const sbyte Attributes::setWis(const sbyte wis)
{
    m_wis = wis;
    return m_wis;
}

const sbyte Attributes::getDex() const
{
    return m_dex;
}

const sbyte Attributes::setDex(const sbyte dex)
{
    m_dex = dex;
    return m_dex;
}

const sbyte Attributes::getCon() const
{
    return m_con;
}

const sbyte Attributes::setCon(const sbyte con)
{
    m_con = con;
    return m_con;
}

const sbyte Attributes::getCha() const
{
    return m_cha;
}

const sbyte Attributes::setCha(const sbyte cha)
{
    m_cha = cha;
    return m_cha;
}

#endif // __9MOONS__CHARACTERATTRIBUTES_H__
