/**
 * @file trackdata.h
 * Class used to contain and manage tracks.
 *
 * Part of the core tbaMUD source code distribution, which is a derivative
 * of, and continuation of, CircleMUD.
 *
 * All rights reserved.  See license for complete information.
 * Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University
 * CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.
 */

#ifndef __CIRCLE_TRACKDATA_H__
#define __CIRCLE_TRACKDATA_H__

#include "mudtypes.h"
#include <list>

class CTrackData
{
public:
    CTrackData(const char *name, const byte dir, const byte pct, 
               const bool enter, const time_t create);
    ~CTrackData(void);

    inline const char *getName() const;
    const char *setName(const char *name);
    void clearName();

    inline const byte getDirection() const;
    inline const byte setDirection(const byte dir);

    inline const byte getPctCovered() const;
    inline const byte setPctCovered(const byte pct);

    inline const bool getIsEntering() const;
    inline const bool setIsEntering(const bool enter);

    inline const time_t getCreateTime() const;
    inline const time_t setCreateTime(const time_t created);

    inline const byte getRace() const;
    inline const byte setRace(const byte race);

    inline const byte getBarefoot() const;
    inline const byte setBarefoot(const byte barefoot);

    inline const byte getBurdened() const;
    inline const byte setBurdened(const byte burdened);

    const CTrackData &operator=(const CTrackData &rhs);

    const bool operator==(const CTrackData &rhs) const;
    const bool operator<(const CTrackData &rhs) const;
    const bool operator<=(const CTrackData &rhs) const;
    const bool operator>(const CTrackData &rhs) const;
    const bool operator>=(const CTrackData &rhs) const;
    inline const bool operator!=(const CTrackData &rhs) const;

    static const char *ms_unknownTrackStr;

private:
    char *m_name;       ///! The name of the char making the track
    byte m_dir;         ///! The direction of the tracks
    byte m_pctCovered;  ///! How well covered are they
    bool m_entered;     ///! Are the tracks emntering or leaving
    time_t m_created;   ///! Used to determine how old the tracks are
    byte m_race;        ///! Not implemented yet
    byte m_barefoot;    ///! Do the tracks indicate race
    byte m_burdened;    ///! Is the creator loaded or mounted
};

/// inline functions
const byte CTrackData::getDirection() const
{
	return m_dir;
}
const byte CTrackData::setDirection(const byte dir)
{
	return m_dir = dir;
}

const byte CTrackData::getPctCovered() const
{
	return m_pctCovered;
}
const byte CTrackData::setPctCovered(const byte pct)
{
	return m_pctCovered = pct;
}

const bool CTrackData::getIsEntering() const
{
	return m_entered;
}
const bool CTrackData::setIsEntering(const bool enter)
{
	return m_entered = enter;
}

const time_t CTrackData::getCreateTime() const
{
	return m_created;
}
const time_t CTrackData::setCreateTime(const time_t created)
{
	return m_created = created;
}

const byte CTrackData::getRace() const
{
	return m_race;
}
const byte CTrackData::setRace(const byte race)
{
	return m_race = race;
}

const byte CTrackData::getBarefoot() const
{
	return m_barefoot;
}
const byte CTrackData::setBarefoot(const byte barefoot)
{
	return m_barefoot = barefoot;
}

const byte CTrackData::getBurdened() const
{
	return m_burdened;
}
const byte CTrackData::setBurdened(const byte burdened)
{
	return m_burdened = burdened;
}

const bool CTrackData::operator!=(const CTrackData &rhs) const
{
    return !operator ==(rhs);
}


//=============================================================================
class CTracksList
{
public:
    CTracksList() {}
    virtual ~CTracksList();

    const bool addTrack(const byte dir, const byte pct, 
                        const bool enter, const time_t create);
    /**
        Update the tracks, expiring if needed
     */
    const bool update();

private:
    static const size_t ms_maxSize;
    std::list<CTrackData> m_data;
};


#endif
