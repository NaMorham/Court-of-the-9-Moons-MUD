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

class CTrackData
{
public:
    CTrackData(void);
    ~CTrackData(void);

private:
    byte m_dir;         ///! The direction of the tracks
    byte m_pctCovered;  ///! How well covered are they
    bool m_entered;     ///! Are the tracks emntering or leaving
    time_t m_created;   ///! Used to determine how old the tracks are
    byte m_race;        ///! Not implemented yet
    byte barefoot;        
    byte burdened;
};

//class 

#endif
