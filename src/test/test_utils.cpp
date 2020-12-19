#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "test_utils.h"

#ifdef _WIN32
#pragma comment(lib, "ntdll")

#include "../conf.h"
#include "../sysdep.h"

#include <Windows.h>
#include <winternl.h>
#endif

// general utility
static struct _ColourVals_t g_colOn = {
    "\033[0m",    // C_NRM
    "\033[1m",    // C_BLD
    "\033[3m",    // C_ITL
    "\033[4m",    // C_UND

    "\033[30m",    // C_BLK
    "\033[31m",    // C_RED
    "\033[32m",    // C_GRN
    "\033[33m",    // C_YEL
    "\033[34m",    // C_BLU
    "\033[35m",    // C_MAG
    "\033[36m",    // C_CYN
    "\033[37m",    // C_WHT

    "\033[90m",    // C_BBLK
    "\033[91m",    // C_BRED
    "\033[92m",    // C_BGRN
    "\033[93m",    // C_BYEL
    "\033[94m",    // C_BBLU
    "\033[95m",    // C_BMAG
    "\033[96m",    // C_BCYN
    "\033[97m",    // C_BWHT

    "\033[91mFAIL\033[0m",  // FAILED_STR
    "\033[92mPASS\033[0m"   // PASSED_STR
};
static struct _ColourVals_t g_colOff = {
    "",     // C_NRM
    "",     // C_BLD
    "",     // C_ITL
    "",     // C_UND

    "",     // C_BLK
    "",     // C_RED
    "",     // C_GRN
    "",     // C_YEL
    "",     // C_BLU
    "",     // C_MAG
    "",     // C_CYN
    "",     // C_WHT

    "",     // C_BBLK
    "",     // C_BRED
    "",     // C_BGRN
    "",     // C_BYEL
    "",     // C_BBLU
    "",     // C_BMAG
    "",     // C_BCYN
    "",     // C_BWHT

    "FAIL", // FAILED_STR
    "PASS"  // PASSED_STR
};

//---------------------------------------------------------------------------
Colours_t &GetColoursObj(eColoursType colType /*= COL_DEFAULT*/) {
    static Colours_t _theColoursObj(COL_DEFAULT);
    static Colours_t _theOnColoursObj(COL_ON);
    static Colours_t _theOffColoursObj(COL_OFF);

    switch (colType){
    case COL_ON:
        return _theOnColoursObj;
    case COL_OFF:
        return _theOffColoursObj;
    default:
        return _theColoursObj;
    }
}

Colours_t::Colours_t(eColoursType colType) {
    init_colour();

    if (m_supportColour) {
        switch (colType) {
        case COL_ON:
            // supported and explicity off
            m_theColourVals = &g_colOn;
            break;
        case COL_OFF:
            // supported but explicity off
            m_theColourVals = &g_colOff;
            break;
        case COL_DEFAULT:
            // supported, and default is on
            m_theColourVals = &g_colOn;
            break;
        default:
            warn_log("Unknown colour type [%ld]", (long)colType);
            m_theColourVals = &g_colOff;
        }
    }
    else {
        // no supported, do not try - all off
        m_theColourVals = &g_colOff;
    }
}

Colours_t::~Colours_t() {
    m_theColourVals = NULL;
}

void Colours_t::init_colour() {
    m_supportColour = true;
#ifdef _WIN32
    HANDLE cmd = GetStdHandle(STD_OUTPUT_HANDLE);
    unsigned long ir = 0;
    NtQueryObject(cmd, ObjectTypeInformation, NULL, 0, &ir);
    PPUBLIC_OBJECT_TYPE_INFORMATION info = (PPUBLIC_OBJECT_TYPE_INFORMATION)(malloc(ir));
    NtQueryObject(cmd, ObjectTypeInformation, info, ir, &ir);

    size_t len = 0;
    char buf[DEFAULT_STR_BUF + 1];
    memset(buf, 0, sizeof(char)*(DEFAULT_STR_BUF + 1));

    if (g_logLevel >= LL_TRACE) {
        len += snprintf(buf, DEFAULT_STR_BUF, "Running as Typename [");
        int bullshit = 0;  // fucking M$, just use fucking strings
        do {
            len += snprintf(buf, DEFAULT_STR_BUF-len, "%c", info->TypeName.Buffer[bullshit++]);
        } while (info->TypeName.Buffer[bullshit] != 0);
        snprintf(buf, DEFAULT_STR_BUF - len, "]\n");
        dev_log(buf);
    }

    if (info->TypeName.Buffer[0] == 'K' && info->TypeName.Buffer[1] == 'e' && info->TypeName.Buffer[2] == 'y') {
        /*cmd*/
        m_supportColour = false;
        dev_log("Running as cmd");
        //m_theColourVals = &g_colOff;
    }
    else if (info->TypeName.Buffer[0] == 'F' && info->TypeName.Buffer[1] == 'i' && info->TypeName.Buffer[2] == 'l' && info->TypeName.Buffer[3] == 'e') {
        /*cygwin*/
        m_supportColour = true;
        dev_log("Running as xterm");
        //m_theColourVals = &g_colOn;
    }
    else{
        /*dont know*/
        m_supportColour = false;
        dev_log("Running as unknown");
        //m_theColourVals = &g_colOff;
    }
    free(info);
#else
    m_supportColour = true;
    dev_log("Running on non windows\n");
    //m_theColourVals = &g_colOn;
#endif
}

const char *Colours_t::ColPct(double pctVal){
    static char buf[DEFAULT_STR_BUF + 1];
    memset(buf, 0, sizeof(char)*(DEFAULT_STR_BUF + 1));

    //(total_pct_pass >= 90.0 ? C.GRN() : 
    //    (total_pct_pass >= 75 ? C.YEL() : 
    //    (total_pct_pass >= 25.0 ? C.RED() : C.BRED()))),
    //    total_pct_pass, C.NRM())

    if (pctVal == 0.0) {
        snprintf(buf, DEFAULT_STR_BUF, "%s%.2f%%%s", BRED(), pctVal, NRM());
    }
    else if (pctVal >= 100.0) {
        snprintf(buf, DEFAULT_STR_BUF, "%s%.2f%%%s", BGRN(), pctVal, NRM());
    }
    else if (pctVal >= 90.0) {
        snprintf(buf, DEFAULT_STR_BUF, "%s%.2f%%%s", GRN(), pctVal, NRM());
    }
    else if (pctVal >= 65.0) {
        snprintf(buf, DEFAULT_STR_BUF, "%s%.2f%%%s", YEL(), pctVal, NRM());
    }
    else if (pctVal >= 15.0) {
        snprintf(buf, DEFAULT_STR_BUF, "%s%.2f%%%s", RED(), pctVal, NRM());
    }
    else {
        snprintf(buf, DEFAULT_STR_BUF, "%s%.2f%%%s", BRED(), pctVal, NRM());
    }
    return buf;
}

//-----------------------------------------------------------------------------
// random.cpp and utils.cpp
#define    m  (unsigned long)2147483647
#define    q  (unsigned long)127773

#define    a (unsigned int)16807
#define    r (unsigned int)2836

static unsigned long seed;

//-----------------------------------------------------------------------------
const size_t DEFAULT_HISTO_STR_BUF = 1024;

const char *histo_t::gline(const size_t line_len) {
    //basic_log("a. gline %lu", line_len);
    static char linebuf[81];
    memset(linebuf, 0, 81 + sizeof(char));
    if (line_len > 80) {
        memset(linebuf, 'X', 74 + sizeof(char));
        snprintf(linebuf + 74, 6, " %5lu", line_len);
    }
    else {
        for (size_t idx = 0; idx < line_len; ++idx) {
            if ((idx>0) && ((idx % 10) == 0)) {
                strcat(linebuf, "+");
            }
            else {
                strcat(linebuf, "-");
            }
        }
    }
    return linebuf;
}

histo_t::histo_t(int minv, int maxv) {
    Colours_t &C = GetColoursObj();

    if (minv<maxv) {
        m_base = minv;
        m_top = maxv;
    }
    else {
        m_base = maxv;
        m_top = minv;
    }
    m_minVal = INT_MAX;
    m_maxVal = INT_MIN;
    m_numBelow = 0;
    m_numAbove = 0;
    m_count = 0;
    m_numVals = ((m_top - m_base) + 1);
    if (m_numVals <= 0) {
        m_histo = NULL;
    }
    else {
        m_histo = new size_t[m_numVals];
        for (size_t i = 0; i < m_numVals; ++i) {
            m_histo[i] = 0;
        }
    }
    //basic_log("%s# new%s base: %d, top %d, numv %d, data 0x%p", C.MAG(), C.NRM(), m_base, m_top, m_numVals, (void*)m_histo);
}

histo_t::~histo_t() {
    if (m_histo) {
        delete[] m_histo;
        m_histo = NULL;
    }
    m_numVals = m_minVal = m_maxVal = m_numBelow = m_numAbove = 0;
    m_base = m_top = 0;
}

histo_t &histo_t::increment(const int val, const size_t amt /*= 1*/) {
    if (val < m_base) {
        m_numBelow += amt;
    }
    else if (val > m_top) {
        m_numAbove += amt;
    }
    else {
        m_histo[val - m_base] += amt;
    }
    m_count += amt;

    if (val < m_minVal) {
        m_minVal = val;
    }
    if (val > m_maxVal) {
        m_maxVal = val;
    }
    return *this;
}

const char *histo_t::str() const {
    static char buf[DEFAULT_HISTO_STR_BUF + 1];
    size_t len = 0;

    memset(buf, 0, sizeof(char)*(DEFAULT_HISTO_STR_BUF + 1));
    if (!m_histo) {
        strncpy(buf, "NONE", DEFAULT_HISTO_STR_BUF);
    }
    else {
        len = snprintf(buf, DEFAULT_HISTO_STR_BUF, "[b:%lu], ", m_numBelow);

        for (size_t i = 0; i < m_numVals; i++) {
            //basic_log("%s#%s i: %d, v: %d", C_MAG, C_NRM, i, m_base+i);
            len += snprintf(buf + len, DEFAULT_HISTO_STR_BUF - len, "[%d:%lu], ", m_base + i, m_histo[i]);
        }

        len += snprintf(buf + len, DEFAULT_HISTO_STR_BUF - len, "[a:%lu] - total: %lu, range (%d -> %d)",
            m_numAbove, m_count, m_base, m_top);
    }

    return buf;
}

const char *histo_t::graph() {
    //basic_log("a. graph");
    static char gbuf[DEFAULT_HISTO_STR_BUF + 1];
    size_t len = 0;

    memset(gbuf, 0, sizeof(char)*(DEFAULT_HISTO_STR_BUF + 1));
    if (!m_histo) {
        //basic_log("b. no graph");
        strncpy(gbuf, "NONE", DEFAULT_HISTO_STR_BUF);
    }
    else {
        //basic_log("c. start graph");

        len = snprintf(gbuf, DEFAULT_HISTO_STR_BUF, "---------------------------------------------\n");
        if ((m_numBelow + m_numAbove)>0) {
            len += snprintf(gbuf + len, DEFAULT_HISTO_STR_BUF - len, "Below: %3u: %s\n", m_numBelow, gline(m_numBelow));
            len += snprintf(gbuf + len, DEFAULT_HISTO_STR_BUF - len, "---------------------------------------------\n");
        }
        for (size_t i = 0; i < m_numVals; ++i) {
            len += snprintf(gbuf + len, DEFAULT_HISTO_STR_BUF - len, "[%3d]: %3u: %s\n", i + m_base, m_histo[i], gline(m_histo[i]));
        }
        if ((m_numBelow + m_numAbove)>0) {
            len += snprintf(gbuf + len, DEFAULT_HISTO_STR_BUF - len, "---------------------------------------------\n");
            len += snprintf(gbuf + len, DEFAULT_HISTO_STR_BUF - len, "Above: %3u: %s\n", m_numAbove, gline(m_numAbove));
        }
        len += snprintf(gbuf + len, DEFAULT_HISTO_STR_BUF - len, "---------------------------------------------\n");
    }
    return gbuf;
}

//---------------------------------------------------------------------------
const char *LL_Str[] = {
    "NONE ",
    "FATAL",
    "ERROR",
    "WARN",
    "INFO ",
    "DEBUG",
    "TRACE",
    "DVTRC"
};

const char *getLogLevelText(eLogLevels ll)
{
    // sanity check?  it's not like we can accidently not use the enum
    return LL_Str[ll];
}

eLogLevels g_logLevel = LL_INFO;  // default to info

void basic_vlog(eLogLevels level, const char *format, va_list args)
{
    const size_t TIME_BUF_SZ = 64;
    const size_t DEFAULT_STR_BUF = 1024;

    if (format == NULL)
    {
        format = "SYSERR: received a NULL format.";
        level = LL_ERROR;
    }

    if (g_logLevel == LL_NONE) {
        // nothing to do
        return;
    }
    else if (level <= g_logLevel) {
        char timeBuf[TIME_BUF_SZ];

#ifdef CIRCLE_WINDOWS
        // not prepared to fuck around with the high frequency counter or any of the other MSVC bullshit to get ms value.
        time_t ct = time(0);
        //strftime(timeBuf, sizeof(char)*TIME_BUF_SZ, "%b %d %H:%M:%S %Y", localtime(&ct));
        strftime(timeBuf, sizeof(char)*TIME_BUF_SZ, "%Y-%m-%dT%H:%M:%S", gmtime(&ct));
#else
        // OSX/Linux this seems ok

        char timeFmt[TIME_BUF_SZ];
        long ms; // Milliseconds
        time_t s;  // Seconds
        struct timespec spec;

        /* Clear buffers */
        memset(timeBuf, 0, sizeof(char)*TIME_BUF_SZ);
        memset(timeFmt, 0, sizeof(char)*TIME_BUF_SZ);

        /* Get the current system time using the realtime clock */
        clock_gettime(CLOCK_REALTIME, &spec);

        s = spec.tv_sec;
        ms = round(spec.tv_nsec / 1.0e6); // Convert nanoseconds to milliseconds
        if (ms > 999)
        {
            s++;
            ms = 0;
        }

        //sprintf(timeFmt, "%%Y-%%m-%%dT%%H:%%M:%%S.%03.3ld %%Z", ms);
        strcpy(timeFmt, "%%Y-%%m-%%dT%%H:%%M:%%S %%Z");
        (void)strftime(timeBuf, TIME_BUF_SZ, timeFmt, gmtime(&s));
#endif

        fprintf(stderr, "[%s] [%s] ", timeBuf, getLogLevelText(level));
        vfprintf(stderr, format, args);
        fprintf(stderr, "\n");

        fflush(stderr);
    }
}

void basic_log(eLogLevels level, const char *format, ...)
{
    va_list args;

    va_start(args, format);
    basic_vlog(level, format, args);
    va_end(args);
}

void fatal_log(const char *format, ...)
{    va_list args;

    va_start(args, format);
    basic_vlog(LL_FATAL, format, args);
    va_end(args);
}

void error_log(const char *format, ...)
{    va_list args;

    va_start(args, format);
    basic_vlog(LL_ERROR, format, args);
    va_end(args);
}

void warn_log(const char *format, ...)
{
    va_list args;

    va_start(args, format);
    basic_vlog(LL_WARN, format, args);
    va_end(args);
}

void info_log(const char *format, ...)
{    va_list args;

    va_start(args, format);
    basic_vlog(LL_INFO, format, args);
    va_end(args);
}

void debug_log(const char *format, ...)
{    va_list args;

    va_start(args, format);
    basic_vlog(LL_DEBUG, format, args);
    va_end(args);
}

void trace_log(const char *format, ...)
{    va_list args;

    va_start(args, format);
    basic_vlog(LL_TRACE, format, args);
    va_end(args);
}

void dev_log(const char *format, ...)
{    va_list args;

    va_start(args, format);
    basic_vlog(LL_DEVTRC, format, args);
    va_end(args);
}

//-----------------------------------------------------------------------------
/*
* F(z)    = (az)%m
**    = az-m(az/m)
**
** F(z)  = G(z)+mT(z)
** G(z)  = a(z%q)- r(z/q)
** T(z)  = (z/q) - (az/m)
**
** F(z)  = a(z%q)- rz/q+ m((z/q) - a(z/m))
**      = a(z%q)- rz/q+ m(z/q) - az
*/

void circle_srandom(unsigned long initial_seed)
{
    seed = initial_seed;
}

unsigned long circle_random(void)
{
    int lo, hi, test;

    hi = seed / q;
    lo = seed % q;

    test = a*lo - r*hi;

    if (test > 0) {
        seed = test;
    }
    else {
        seed = test + m;
    }

    return (seed);
}

const int rand_number(const int from, const int to)
{
    // error checking in case people call this incorrectly
    if (from > to) {
        warn_log("SYSERR: rand_number() should be called with lowest, then highest. (%d, %d), not (%d, %d).", from, to, to, from);
        return rand_number(to, from);
    }
    else
    {
        // This should always be of the form: ((float)(to - from + 1) * rand() /
        // (float)(RAND_MAX + from) + from); If you are using rand() due to historical
        // non-randomness of the lower bits in older implementations.  We always use
        // circle_random() though, which shouldn't have that problem. Mean and
        // standard deviation of both are identical (within the realm of statistical
        // identity) if the rand() implementation is non-broken.
        return ((circle_random() % (to - from + 1)) + from);
    }
}

