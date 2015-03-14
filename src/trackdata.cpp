#include "trackdata.h"
#include <ctime>

#include "structs.h"

const char *CTrackData::ms_unknownTrackStr = "an unknown creature";
const size_t CTrackData::ms_TimeBufSize = 96;


CTrackData::CTrackData(const char *name, const byte dir, const byte pct,
                       const bool enter, const time_t create)
:   m_name(NULL), m_dir(dir), m_pctCovered(pct),
    m_entered(enter), m_created(create)
{
    setName(name);
}

CTrackData::CTrackData(const CTrackData &orig)
:   m_name(NULL), m_dir(NODIRECTION), m_pctCovered(0),
    m_entered(false), m_created(0)
{
    *this = orig;
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

const char *CTrackData::getCreateTimeStr() const
{
    // this is only safe for single threaded use
    static char buf[ms_TimeBufSize];
    time_t tt = getCreateTime();
    strcpy(buf, ctime(&tt));
    return buf;
}

const CTrackData &CTrackData::operator=(const CTrackData &rhs)
{
    setName(rhs.getName());
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

const bool CTracksList::addTrack(const char *name, const byte dir, const byte pct,
                const bool enter, const time_t create)
{
    // we are at a max size, so obliterate (remove) the oldest
    if (m_data.size() > ms_maxSize)
    {
        removeOldest();
    }
    CTrackData track(name, dir, pct, enter, create);
    m_data.push_back(track);

    return true;
}

const bool CTracksList::update()
{
    // check for expired tracks
    // work backwards since the list is sorted by time
    return false;
}

void CTracksList::clearAll()
{
    m_data.clear();
}

void CTracksList::removeOldest()
{
    if (!m_data.empty())
    {
        m_data.pop_front();
    }
}
