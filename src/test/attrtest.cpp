/****************************************************************************
 * Test the stat generation and race adjustment                 9 moons mud *
 *                                                                          *
 * The roll_real_abils and adjust_abils_for_race functions must exactly     *
 * match the ones from class.cpp and race.cpp for this to be useful.        *
 * The random_number function and constant value should match the ones from *
 * structs and utils files,  This wont be representative without a match.   *
 ****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include <time.h>
#include <string.h>
#include <math.h>

#include "../conf.h"
#include "../sysdep.h"s

// general utility
#define TIME_BUF_SZ 64
#define DEFAULT_STR_BUF 1024

#define C_NRM "\033[0m"
#define C_BLD "\033[1m"
#define C_ITL "\033[3m"
#define C_UND "\033[4m"
#define C_BLK "\033[30m"
#define C_RED "\033[31m"
#define C_GRN "\033[32m"
#define C_YEL "\033[33m"
#define C_BLU "\033[34m"
#define C_MAG "\033[35m"
#define C_CYN "\033[36m"
#define C_WHT "\033[37m"
#define C_BBLK "\033[90m"
#define C_BRED "\033[91m"
#define C_BGRN "\033[92m"
#define C_BYEL "\033[93m"
#define C_BBLU "\033[94m"
#define C_BMAG "\033[95m"
#define C_BCYN "\033[96m"
#define C_BWHT "\033[97m"

void basic_log(const char *format, ...);
void basic_vlog(const char *format, va_list args);

#define WriteLogf basic_log

typedef signed char sbyte;          //!< 1 byte; vals = -127 to 127
typedef unsigned char ubyte;        //!< 1 byte; vals = 0 to 255
typedef signed short int sh_int;    //!< 2 bytes; vals = -32,768 to 32,767
typedef unsigned short int ush_int; //!< 2 bytes; vals = 0 to 65,535

#if !defined(__cplusplus)   // Anyone know a portable method?
typedef char bool;          //!< Technically 1 signed byte; vals should only = TRUE or FALSE.
#endif

#if !defined(CIRCLE_WINDOWS) || defined(LCC_WIN32)    // Hm, sysdep.h?
typedef signed char byte;   //!< Technically 1 signed byte; vals should only = TRUE or FALSE.
#endif

inline int MIN(const int a, const int b) { return (a < b ? a : b); }
inline int MAX(const int a, const int b) { return (a > b ? a : b); }
inline int LIMIT(const int val, const int minv, const int maxv) {
    return (maxv >= minv ? MAX(minv, MIN(maxv, val)) : LIMIT(val, maxv, minv));
}

// random.cpp and utils.cpp
#define    m  (unsigned long)2147483647
#define    q  (unsigned long)127773

#define    a (unsigned int)16807
#define    r (unsigned int)2836

static unsigned long seed;

void circle_srandom(unsigned long initial_seed);
unsigned long circle_random(void);
const int rand_number(const int from, const int to);

// class and race stuff
/*
 *  PC classes
 */
#define CLASS_UNDEFINED     (-1)//!< PC Class undefined
#define CLASS_MAGIC_USER    0   //!< PC Class Magic User
#define CLASS_CLERIC        1   //!< PC Class Cleric
#define CLASS_THIEF         2   //!< PC Class Thief
#define CLASS_WARRIOR       3   //!< PC Class Warrior
// Total number of available PC Classes
#define NUM_CLASSES         4

// NPC classes (currently unused - feel free to implement!)
#define CLASS_OTHER        21   //!< NPC Class Other (or undefined)
#define CLASS_UNDEAD       22   //!< NPC Class Undead
#define CLASS_HUMANOID     23   //!< NPC Class Humanoid
#define CLASS_ANIMAL       24   //!< NPC Class Animal
#define CLASS_DRAGON       25   //!< NPC Class Dragon
#define CLASS_GIANT        26   //!< NPC Class Giant

// Number of available NPC classes
#define NUM_NPC_CLASSES     6

#define TOTAL_CLASSES       (NUM_CLASSES + NUM_NPC_CLASSES)

const char *class_name(const sbyte chclass) {
    switch (chclass) {
    case CLASS_UNDEFINED:     return "Undefined";
    case CLASS_MAGIC_USER:     return "Magic User";
    case CLASS_CLERIC:         return "Cleric";
    case CLASS_THIEF:         return "Thief";
    case CLASS_WARRIOR:     return "Warrior";
    case CLASS_OTHER:         return "Other";
    case CLASS_UNDEAD:         return "Undead";
    case CLASS_HUMANOID:     return "Humanoid";
    case CLASS_ANIMAL:         return "Animal";
    case CLASS_DRAGON:         return "Dragon";
    case CLASS_GIANT:         return "Giant";
    default:                 return "**UNKNOWN**";
    }
}

// does not need to be an enum, but...
enum eClassBits {
    CLASSB_NONE = 0,
    CLASSB_MAGIC_USER   = (1 << CLASS_MAGIC_USER),
    CLASSB_CLERIC       = (1 << CLASS_CLERIC),
    CLASSB_THIEF        = (1 << CLASS_THIEF),
    CLASSB_WARRIOR      = (1 << CLASS_WARRIOR),

    CLASSB_OTHER        = (1 << CLASS_OTHER),
    CLASSB_UNDEAD       = (1 << CLASS_UNDEAD),
    CLASSB_HUMANOID     = (1 << CLASS_HUMANOID),
    CLASSB_ANIMAL       = (1 << CLASS_ANIMAL),
    CLASSB_DRAGON       = (1 << CLASS_DRAGON),
    CLASSB_GIANT        = (1 << CLASS_GIANT),

    CLASSB_ANYPC        = (CLASSB_MAGIC_USER | CLASSB_CLERIC | CLASSB_THIEF | CLASSB_WARRIOR),
    CLASSB_ANYNPC       = (CLASSB_OTHER | CLASSB_UNDEAD | CLASSB_HUMANOID | CLASSB_ANIMAL | CLASSB_DRAGON | CLASSB_GIANT),
    CLASSB_ANY          = (CLASSB_ANYPC | CLASSB_ANYNPC)
};

enum eRaces {
    RACE_UNKNOWN = -1,
    RACE_HUMAN,
    RACE_TROLLOC,
    RACE_OGIER,
    RACE_FADE,

    NUM_RACES    // Must be last
};

enum eRaceBits {
    RACEB_NONE      = 0,
    RACEB_HUMAN     = (1 << RACE_HUMAN),
    RACEB_TROLLOC   = (1 << RACE_TROLLOC),
    RACEB_OGIER     = (1 << RACE_OGIER),
    RACEB_FADE      = (1 << RACE_FADE),

    // races that *can* be dark or light, not a must or should map
    RACEB_DARK      = (RACEB_HUMAN | RACEB_TROLLOC | RACEB_FADE),
    RACEB_LIGHT     = (RACEB_HUMAN | RACEB_OGIER)
};

// Sex
#define SEX_NEUTRAL     0   //!< Neutral Sex (Hermaphrodite)
#define SEX_MALE        1   //!< Male Sex (XY Chromosome)
#define SEX_FEMALE      2   //!< Female Sex (XX Chromosome)

#define NUM_GENDERS     3   //<! Total number of Genders


// dummy structs

struct attr_data {
    attr_data() : m_str(11), m_strAdd(11), m_dex(11), m_con(11), m_intel(11), m_wis(11), m_cha(11) {}
    attr_data(const sbyte str, const sbyte strAdd, const sbyte dex, const sbyte con,
            const  sbyte intel, const sbyte wis, const sbyte cha)
        : m_str(str), m_strAdd(strAdd), m_dex(dex), m_con(con), m_intel(intel), m_wis(wis), m_cha(cha) {}
    attr_data(const attr_data &rhs) : m_str(rhs.m_str), m_strAdd(rhs.m_strAdd), m_dex(rhs.m_dex),
        m_con(rhs.m_con), m_intel(rhs.m_intel), m_wis(rhs.m_wis), m_cha(rhs.m_cha) {}
    ~attr_data() { m_str = m_strAdd = m_dex = m_con = m_intel = m_wis = m_cha = 0; }

    const sbyte getStr() const { return m_str; }
    const sbyte setStr(const sbyte str) { return m_str = str; }

    const sbyte getStrAdd() const { return m_strAdd; }
    const sbyte setStrAdd(const sbyte strAdd) { return m_strAdd = strAdd; }

    const sbyte getDex() const { return m_dex; }
    const sbyte setDex(const sbyte dex) { return m_dex = dex; }

    const sbyte getCon() const { return m_con; }
    const sbyte setCon(const sbyte con) { return m_con = con; }

    const sbyte getIntel() const { return m_intel; }
    const sbyte setIntel(const sbyte intel) { return m_intel = intel; }

    const sbyte getWis() const { return m_wis; }
    const sbyte setWis(const sbyte wis) { return m_wis = wis; }

    const sbyte getCha() const { return m_cha; }
    const sbyte setCha(const sbyte cha) { return m_cha = cha; }

    attr_data& operator=(const attr_data &rhs) {
        m_str = rhs.m_str;
        m_strAdd = rhs.m_strAdd;
        m_dex = rhs.m_dex;
        m_con = rhs.m_con;
        m_intel = rhs.m_intel;
        m_wis = rhs.m_wis;
        m_cha = rhs.m_cha;
        return *this;
    }

    sbyte m_str;
    sbyte m_strAdd;
    sbyte m_dex;
    sbyte m_con;
    sbyte m_intel;
    sbyte m_wis;
    sbyte m_cha;
};

struct char_data {
    char_data(const char *new_name, const sbyte new_chclass, const sbyte new_chrace) 
    : chclass(new_chclass), chrace(new_chrace) {
        // we let the attr data sort it's own shit out
        memset(name, 0, sizeof(char)*DEFAULT_STR_BUF);
        strncpy(name, new_name, DEFAULT_STR_BUF-1);
    }

    char name[DEFAULT_STR_BUF];

    sbyte chclass;
    sbyte chrace;

    struct attr_data real_abils;
    struct attr_data aff_abils;

    operator const char *() const {
        static char buf[DEFAULT_STR_BUF*2];  // we are not multithreaded here, so this should be safe(ish)
        memset(buf, 0, sizeof(char)*(DEFAULT_STR_BUF*2));

        return buf;
    }
};

#define GET_STR(ch) (ch->real_abils.getStr())
#define GET_CON(ch) (ch->real_abils.getCon())
#define GET_RACE(ch) (ch->chrace)
#define GET_CLASS(ch) (ch->chclass)

#define FAILED_STR C_BRED"FAIL"C_NRM
#define PASSED_STR C_BGRN"PASS"C_NRM

void init_colour(int argc, char *argv[]);

bool min_tests(size_t &pass_count, size_t &test_count);
bool max_tests(size_t &pass_count, size_t &test_count);
bool limit_tests(size_t &pass_count, size_t &test_count);
bool rand_tests(size_t &pass_count, size_t &test_count);

//---------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    size_t pass_count,  test_count, total_count = 0, total_pass = 0;
    double total_pct_pass = 0.0;

    init_colour(argc, argv);

    circle_srandom(time(0));

    printf("Wooo?\n");
    basic_log("%s--- Test Min/Max/Limit ---%s", C_BYEL, C_NRM);

    min_tests(pass_count, test_count);
    total_pass += pass_count;
    total_count += test_count;

    max_tests(pass_count, test_count);
    total_pass += pass_count;
    total_count += test_count;

    limit_tests(pass_count, test_count);
    total_pass += pass_count;
    total_count += test_count;

    basic_log("%s--- Test Min/Max/Limit ---%s", C_BYEL, C_NRM);

    rand_tests(pass_count, test_count);
    total_pass += pass_count;
    total_count += test_count;

    total_pct_pass = ((double)total_pass / (double)total_count) * 100.0;
    basic_log("===== %u/%u tests passed (%s%.2f%%%s) =====\n", total_pass, total_count,
        (total_pct_pass >= 90.0 ? C_GRN : (total_pct_pass >= 75 ? C_YEL : (total_pct_pass >= 25.0 ? C_RED : C_BRED))),
        total_pct_pass, C_NRM);

    return 0;
}


//---------------------------------------------------------------------------
void init_colour(int argc, char *argv[]) {
#ifdef _WIN32
#include <Windows.h>
#include <winternl.h>

    HANDLE cmd = GetStdHandle(STD_OUTPUT_HANDLE);
    unsigned long ir = 0;
    NtQueryObject(cmd, ObjectTypeInformation, NULL, 0, &ir);
    PPUBLIC_OBJECT_TYPE_INFORMATION info = (PPUBLIC_OBJECT_TYPE_INFORMATION)(malloc(ir));
    NtQueryObject(cmd, ObjectTypeInformation, info, ir, &ir);
    if (info->TypeName.Buffer[0] == 'K' && info->TypeName.Buffer[1] == 'e' && info->TypeName.Buffer[2] == 'y') {
        /*cmd*/
        printf("Running as cmd\n");
    }
    else if (info->TypeName.Buffer[0] == 'F' && info->TypeName.Buffer[1] == 'i' && info->TypeName.Buffer[2] == 'l' && info->TypeName.Buffer[3] == 'e') {
        /*cygwin*/
        printf("Running as cygwin\n");
    }
    else{
        /*dont know*/
        printf("Running as unknown [%s]\n", info->TypeName);
    }
    free(info);
#else
    printf("Running on non windows\n");
#endif
}

//---------------------------------------------------------------------------
struct test_minmax {
    const char *name;
    const int val1;
    const int val2;
    const int expected;
};

bool min_tests(size_t &pass_count, size_t &test_count) {
    size_t test_num = 0;
    double pct_pass = 0.0;

    test_count = 0;
    pass_count = 0;

    struct test_minmax min_tests[] = {
        { "zero test", 0, 0, 0 },
        { "simple pos test", 1, 2, 1 },
        { "simple pos test2", 2, 1, 1 },
        { "simple pos test3", 0, 42, 0 },

        { "simple neg test", -1, -2, -2 },
        { "simple neg test2", -2, -1, -2 },
        { "simple neg test3", 0, -42, -42 },

        { "simple neg test3", -10, 5, -10 },
        { "simple neg test3", 5, -10, -10 }
    };

    test_count = sizeof(min_tests)/sizeof(test_minmax);
    basic_log("> Run [%lu] min val tests", test_count);
    for (test_num = 0; test_num < test_count; ++test_num) {
        struct test_minmax &test = min_tests[test_num];
        int result = MIN(test.val1, test.val2);
        basic_log("%s[%2u/%2u] %s %24.24s: %s - Val 1 [%s%3d%s], Val 2 [%s%3d%s], Got [%s%3d%s], Expected [%s%3d%s]",
            C_CYN, test_num+1, test_count, C_NRM, test.name, (result == test.expected ? PASSED_STR : FAILED_STR),
            C_BBLK, test.val1, C_NRM, C_BBLK, test.val2, C_NRM, C_BBLK, result, C_NRM, C_BBLK, test.expected, C_NRM);
        pass_count += (result == test.expected ? 1 : 0);
    }
    pct_pass = ((double)pass_count / (double)test_count) * 100.0;
    basic_log("> %u/%u tests passed (%s%.2f%%%s)\n", pass_count, test_count,
        (pct_pass >= 90.0 ? C_GRN : (pct_pass >= 75 ? C_YEL : (pct_pass >= 25.0 ? C_RED : C_BRED))),
        pct_pass, C_NRM);

    return pass_count == test_count;
}

bool max_tests(size_t &pass_count, size_t &test_count) {
    size_t test_num = 0;
    double pct_pass = 0.0;

    test_count = 0;
    pass_count = 0;

    struct test_minmax max_tests[] = {
        { "zero test", 0, 0, 0 },
        { "simple pos test", 1, 2, 2 },
        { "simple pos test2", 2, 1, 2 },
        { "simple pos test3", 0, 42, 42 },

        { "simple neg test", -1, -2, -1 },
        { "simple neg test2", -2, -1, -1 },
        { "simple neg test3", 0, -42, 0 },

        { "simple neg test3", -10, 5, 5 },
        { "simple neg test3", 5, -10, 5 }
    };

    test_count = sizeof(max_tests)/sizeof(test_minmax);
    basic_log("> Run [%lu] max val tests", test_count);
    for (test_num = 0; test_num < test_count; ++test_num) {
        struct test_minmax &test = max_tests[test_num];
        int result = MAX(test.val1, test.val2);
        basic_log("%s[%2u/%2u] %s %24.24s: %s - Val 1 [%s%3d%s], Val 2 [%s%3d%s], Got [%s%3d%s], Expected [%s%3d%s]",
            C_CYN, test_num + 1, test_count, C_NRM, test.name, (result == test.expected ? PASSED_STR : FAILED_STR),
            C_BBLK, test.val1, C_NRM, C_BBLK, test.val2, C_NRM, C_BBLK, result, C_NRM, C_BBLK, test.expected, C_NRM);
        pass_count += (result == test.expected ? 1 : 0);
    }
    pct_pass = ((double)pass_count / (double)test_count) * 100.0;
    basic_log("> %u/%u tests passed (%s%.2f%%%s)\n", pass_count, test_count,
        (pct_pass >= 90.0 ? C_GRN : (pct_pass >= 75 ? C_YEL : (pct_pass >= 25.0 ? C_RED : C_BRED))),
        pct_pass, C_NRM);

    return pass_count == test_count;
}

struct test_limit {
    const char *name;
    const int val;
    const int minv;
    const int maxv;
    const int expected;
};

bool limit_tests(size_t &pass_count, size_t &test_count) {
    size_t test_num = 0;
    double pct_pass = 0.0;

    test_count = 0;
    pass_count = 0;

    struct test_limit limit_tests[] = {
        { "zero test", 0, 0, 0, 0 },
        //             val, min, max, exp
        { "pos test  1", 0, 0, 2, 0 },
        { "pos test  2", 1, 0, 2, 1 },
        { "pos test  3", 2, 0, 2, 2 },
        { "pos test  4", -1, 0, 2, 0 },
        { "pos test  5", 3, 0, 2, 2 },

        { "pos test  6", 5, 5, 10, 5 },
        { "pos test  7", 7, 5, 10, 7 },
        { "pos test  8", 10, 5, 10, 10 },
        { "pos test  9", 1, 5, 10, 5 },
        { "pos test 10", 13, 5, 10, 10 },
        //             val, min, max, exp
        { "neg test  1", 0, -2, 0, 0 },
        { "neg test  2", -1, -2, 0, -1 },
        { "neg test  3", -2, -2, 0, -2 },
        { "neg test  4", 1, -2, 0, 0 },
        { "neg test  5", -3, -2, 0, -2 },

        { "neg test  6", -5, -5, -10, -5 },
        { "neg test  7", -7, -5, -10, -7 },
        { "neg test  8", -10, -5, -10, -10 },
        { "neg test  9", -1, -5, -10, -5 },
        { "neg test 10", -13, -5, -10, -10 },
        //             val, min, max, exp
        { "rev arg test 1", 7, 20, 2, 7 },
        { "rev arg test 2", 1, 20, 2, 2 },
        { "rev arg test 3", 21, 20, 2, 20 },
    };

    test_count = sizeof(limit_tests)/sizeof(test_limit);
    basic_log("> Run [%lu] limit val tests", test_count);
    for (test_num = 0; test_num < test_count; ++test_num) {
        struct test_limit &test = limit_tests[test_num];
        int result = LIMIT(test.val, test.minv, test.maxv);
        basic_log("%s[%2u/%2u] %s %24.24s: %s - Min [%s%3d%s], Val [%s%3d%s], Max [%s%3d%s] -> Got [%s%3d%s], Expected [%s%3d%s]",
            C_CYN, test_num + 1, test_count, C_NRM, test.name, (result == test.expected ? PASSED_STR : FAILED_STR),
            C_BBLK, test.minv, C_NRM, C_BBLK, test.val, C_NRM, C_BBLK, test.maxv, C_NRM,
            C_BBLK, result, C_NRM, C_BBLK, test.expected, C_NRM);
        pass_count += (result == test.expected ? 1 : 0);
    }
    pct_pass = ((double)pass_count / (double)test_count) * 100.0;
    basic_log("> %u/%u tests passed (%s%.2f%%%s)\n", pass_count, test_count,
        (pct_pass >= 90.0 ? C_GRN : (pct_pass >= 75 ? C_YEL : (pct_pass >= 25.0 ? C_RED : C_BRED))),
        pct_pass, C_NRM);

    return pass_count == test_count;
}

struct histo_t {
private:
    const char *gline(const size_t line_len) {
        basic_log("a. gline %lu", line_len);
        static char linebuf[81];
        memset(linebuf, 0, 81+sizeof(char));
        if (line_len > 80) {
            memset(linebuf, 'X', 73+sizeof(char));
            snprintf(linebuf+73, 7, " %5lu", line_len);
        }
        else {
            for (int idx = 0; idx < line_len; ++idx) {
                if ((idx>0) && ((idx % 10) == 0)) {
                    strcat(linebuf,"+");
                } else {
                    strcat(linebuf,"-");
                }
            }
        }
        return linebuf;
    }
public:
    histo_t(int minv, int maxv) {
        if (minv<maxv) {
            m_base = minv;
            m_top = maxv;
        }
        else {
            m_base = maxv;
            m_top = minv;
        }
        m_minVal = 0;
        m_maxVal = 0;
        m_numBelow = 0;
        m_numAbove = 0;
        m_count = 0;
        m_numVals = ((m_top - m_base)+1);
        if (m_numVals<=0) {
            m_histo = NULL;
        }
        else {
            m_histo = new size_t[m_numVals];
            for (int i =0; i < m_numVals; ++i) {
                m_histo[i] = 0;
            }
        }
        basic_log("%s# new%s base: %d, top %d, numv %d, data 0x%p", C_MAG, C_NRM, m_base, m_top, m_numVals, (void*)m_histo);
    }
    ~histo_t() {
        if (m_histo) {
            delete[] m_histo;
            m_histo = NULL;
        }
        m_numVals = m_minVal = m_maxVal = m_numBelow = m_numAbove = 0;
        m_base = m_top = 0;
    }

    histo_t &increment(const int val, const size_t amt = 1) {
        if (val < m_base) {
            m_numBelow += amt;
        } else if (val > m_top) {
            m_numAbove += amt;
        }
        else {
            m_histo[val-m_base] += amt;
        }
        m_count += amt;
        return *this;
    }
    const char *str() const {
        static char buf[DEFAULT_STR_BUF+1];
        size_t len = 0;

        memset(buf, 0, sizeof(char)*(DEFAULT_STR_BUF+1));
        if (!m_histo) {
            strncpy(buf, "NONE", DEFAULT_STR_BUF);
        }
        else {
            len = snprintf(buf, DEFAULT_STR_BUF, "[b:%lu], ", m_numBelow);

            for (int i = 0; i < m_numVals; i++) {
                //basic_log("%s#%s i: %d, v: %d", C_MAG, C_NRM, i, m_base+i);
                len += snprintf(buf+len, DEFAULT_STR_BUF-len,"[%d:%lu], ", m_base+i, m_histo[i]);
            }

            len += snprintf(buf+len, DEFAULT_STR_BUF-len, "[a:%lu] - total: %lu, range (%d -> %d)",
                m_numAbove, m_count, m_base, m_top);
        }

        return buf;
    }
    operator const char *() const { return this->str(); }
    const char *graph() {
	    //basic_log("a. graph");
        static char gbuf[DEFAULT_STR_BUF+1];
        size_t len = 0;

        memset(gbuf, 0, sizeof(char)*(DEFAULT_STR_BUF+1));
        if (!m_histo) {
    	    //basic_log("b. no graph");
            strncpy(gbuf, "NONE", DEFAULT_STR_BUF);
        }
        else {
            //basic_log("c. start graph");

            len = snprintf(gbuf, DEFAULT_STR_BUF, "---------------------------------------------\n");
            if ((m_numBelow+m_numAbove)>0) {
                len += snprintf(gbuf+len, DEFAULT_STR_BUF-len, "Below: %s\n", gline(m_numBelow));
                len += snprintf(gbuf+len, DEFAULT_STR_BUF-len, "---------------------------------------------\n");
            }
            for (int i = 0; i < m_numVals; ++i) {
                len += snprintf(gbuf+len, DEFAULT_STR_BUF-len, "[%3d]: %s\n", i+m_base, gline(m_histo[i]));
            }
            if ((m_numBelow+m_numAbove)>0) {
                len += snprintf(gbuf+len, DEFAULT_STR_BUF-len, "---------------------------------------------\n");
                len += snprintf(gbuf+len, DEFAULT_STR_BUF-len, "Above: %s\n", gline(m_numAbove));
            }
            len += snprintf(gbuf+len, DEFAULT_STR_BUF-len, "---------------------------------------------\n");
        }
        return gbuf;
    }

    size_t m_numVals;
    size_t *m_histo;

    int m_base;
    int m_top;

    size_t m_count;
    size_t m_minVal;
    size_t m_maxVal;
    size_t m_numBelow;
    size_t m_numAbove;
};

bool rand_tests(size_t &pass_count, size_t &test_count) {
    size_t test_num = 0;
    double pct_pass = 0.0;

    test_count = 0;
    pass_count = 0;

    // 100 random numbers in the range 1-6
    // for (int i = 0; i < 100; ++i) {

    // }

    histo_t foo(1, 3);
    basic_log("%s##%s Histo: %s", C_BMAG, C_NRM, (const char *)foo);
    foo.increment(1);
    basic_log("%s##%s Histo: %s", C_BMAG, C_NRM, (const char *)foo);
    foo.increment(1);
    basic_log("%s##%s Histo: %s", C_BMAG, C_NRM, (const char *)foo);
    foo.increment(3);
    basic_log("%s##%s Histo: %s", C_BMAG, C_NRM, (const char *)foo);
    foo.increment(2, 5);
    basic_log("%s##%s Histo: %s", C_BMAG, C_NRM, (const char *)foo);
    basic_log("%s##%s", C_BMAG, C_NRM);
    basic_log("%s##%s \n%s", C_BMAG, C_NRM, foo.graph());

    histo_t foo2(1, 6);
    for (int i = 0; i < 500; ++i) {
    	foo2.increment(rand_number(1,6));
    }
    basic_log("%s##%s \n%s", C_BMAG, C_NRM, foo2.graph());

    pct_pass = ((double)pass_count / (double)test_count) * 100.0;
    basic_log("> %u/%u tests passed (%s%.2f%%%s)\n", pass_count, test_count,
        (pct_pass >= 90.0 ? C_GRN : (pct_pass >= 75 ? C_YEL : (pct_pass >= 25.0 ? C_RED : C_BRED))),
        pct_pass, C_NRM);

    return pass_count == test_count;
}


//---------------------------------------------------------------------------
void basic_vlog(const char *format, va_list args)
{
    if (format == NULL)
    {
        format = "SYSERR: received a NULL format.";
    }

    char timeBuf[TIME_BUF_SZ];
    char timeFmt[TIME_BUF_SZ];

#ifdef CIRCLE_WINDOWS
    time_t ct = time(0);
    //strftime(timeBuf, sizeof(char)*TIME_BUF_SZ, "%b %d %H:%M:%S %Y", localtime(&ct));
    strftime(timeBuf, sizeof(char)*TIME_BUF_SZ, "%%Y-%%m-%%dT%%H:%%M:%%S", gmtime(&ct));
#else
    long            ms; // Milliseconds
    time_t          s;  // Seconds
    struct timespec spec;

    /* Clear buffers */
    memset(timeBuf, 0, sizeof(char)*TIME_BUF_SZ);
    memset(timeFmt, 0, sizeof(char)*TIME_BUF_SZ);

    /* Get the current system time using the realtime clock */
    clock_gettime(CLOCK_REALTIME, &spec);

    s  = spec.tv_sec;
    ms = round(spec.tv_nsec / 1.0e6); // Convert nanoseconds to milliseconds
    if (ms > 999)
    {
        s++;
        ms = 0;
    }

    sprintf(timeFmt, "%%Y-%%m-%%dT%%H:%%M:%%S.%03.3ld %%Z", ms);
    (void)strftime(timeBuf, TIME_BUF_SZ, timeFmt, gmtime(&s));
#endif

    fprintf(stderr, "[%s] ", timeBuf);
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
    
    fflush(stderr);
}

void basic_log(const char *format, ...)
{
    va_list args;

    va_start(args, format);
    basic_vlog(format, args);
    va_end(args);
}

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
        WriteLogf("SYSERR: rand_number() should be called with lowest, then highest. (%d, %d), not (%d, %d).", from, to, to, from);
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


void roll_real_abils(struct char_data *ch)
{
    int i, j, k, temp;
    ubyte table[6];
    ubyte rolls[4];

    for (i = 0; i < 6; i++) {
        table[i] = 0;
    }

    for (i = 0; i < 6; i++) {
        for (j = 0; j < 4; j++) {
            rolls[j] = rand_number(1, 6);
        }  // for (j ...

        temp = rolls[0] + rolls[1] + rolls[2] + rolls[3] -
            MIN(rolls[0], MIN(rolls[1], MIN(rolls[2], rolls[3])));

        for (k = 0; k < 6; k++) {
            if (table[k] < temp) {
                temp ^= table[k];
                table[k] ^= temp;
                temp ^= table[k];
            }
        }  // for (k ...
    }  // for (i ...

    ch->real_abils.setStrAdd(0);

    switch (GET_CLASS(ch)) {
    case CLASS_MAGIC_USER:
        ch->real_abils.setIntel(table[0]);
        ch->real_abils.setWis(table[1]);
        ch->real_abils.setDex(table[2]);
        ch->real_abils.setStr(table[3]);
        ch->real_abils.setCon(table[4]);
        ch->real_abils.setCha(table[5]);
        break;
    case CLASS_CLERIC:
        ch->real_abils.setWis(table[0]);
        ch->real_abils.setIntel(table[1]);
        ch->real_abils.setStr(table[2]);
        ch->real_abils.setDex(table[3]);
        ch->real_abils.setCon(table[4]);
        ch->real_abils.setCha(table[5]);
        break;
    case CLASS_THIEF:
        ch->real_abils.setDex(table[0]);
        ch->real_abils.setStr(table[1]);
        ch->real_abils.setCon(table[2]);
        ch->real_abils.setIntel(table[3]);
        ch->real_abils.setWis(table[4]);
        ch->real_abils.setCha(table[5]);
        break;
    case CLASS_WARRIOR:
        ch->real_abils.setStr(table[0]);
        ch->real_abils.setDex(table[1]);
        ch->real_abils.setCon(table[2]);
        ch->real_abils.setWis(table[3]);
        ch->real_abils.setIntel(table[4]);
        ch->real_abils.setCha(table[5]);
        if (ch->real_abils.getStr() >= 18) {
            ch->real_abils.setStrAdd(rand_number(0, 100));
        }
        break;
    }
    ch->aff_abils = ch->real_abils;
}

