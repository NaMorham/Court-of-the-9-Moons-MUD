#include "trackdata.h"


const char *CTrackData::ms_unknownTrackStr = "an unknown creature";

CTrackData::CTrackData(const char *name, const byte dir, const byte pct, 
                       const bool enter, const time_t create)
:   m_name(NULL), m_dir(dir), m_pctCovered(pct), 
    m_entered(enter), m_created(create)
{
    setName(name);
}

CTrackData::~CTrackData(void)
{
    clearName();
}

const char *CTrackData::setName(const char *name)
{
    size_t nameLen = 21;

    clearName();

    if (name && *name)
    {
        nameLen = strlen(name) + 1;
        m_name = new char[nameLen];
        memset(m_name, 0, nameLen);
        strcpy(m_name, name);
    }
    else
    {
        m_name = new char[nameLen];
        memset(m_name, 0, nameLen);
        strcpy(m_name, ms_unknownTrackStr);
    }

    return m_name;
}

void CTrackData::clearName()
{
    if (m_name)
    {
        delete[] m_name;
        m_name = NULL;
    }
}

const CTrackData &CTrackData::operator=(const CTrackData &rhs)
{
    setDirection(rhs.getDirection());
    setPctCovered(rhs.getPctCovered());
    setIsEntering(rhs.getIsEntering());
    setCreateTime(rhs.getCreateTime());
    setRace(rhs.getRace());
    setBarefoot(rhs.getBarefoot());
    setBurdened(rhs.getBurdened());

    return *this;
}

const bool CTrackData::operator==(const CTrackData &rhs) const
{
    return false;
}

const bool CTrackData::operator<(const CTrackData &rhs) const
{
    return false;
}

const bool CTrackData::operator<=(const CTrackData &rhs) const
{
    return false;
}

const bool CTrackData::operator>(const CTrackData &rhs) const
{
    return false;
}

const bool CTrackData::operator>=(const CTrackData &rhs) const
{
    return false;
}


// =========== Track List ============
const size_t CTracksList::ms_maxSize = 10;

CTracksList::~CTracksList()
{
    m_data.clear();
}

const bool CTracksList::addTrack(const byte dir, const byte pct, 
                const bool enter, const time_t create)
{
    if (m_data.size() > ms_maxSize)
    {
    }

    return true;
}

const bool CTracksList::update()
{
    return false;
}
