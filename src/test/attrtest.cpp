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

#include "test_utils.h"
#include "argparse.h"

#ifdef _WIN32
// sigh, more windows string fucker needs this
# include "../conf.h"
# include "../sysdep.h"
#endif

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
    case CLASS_UNDEFINED:   return "Undefined";
    case CLASS_MAGIC_USER:  return "Magic User";
    case CLASS_CLERIC:      return "Cleric";
    case CLASS_THIEF:       return "Thief";
    case CLASS_WARRIOR:     return "Warrior";
    case CLASS_OTHER:       return "Other";
    case CLASS_UNDEAD:      return "Undead";
    case CLASS_HUMANOID:    return "Humanoid";
    case CLASS_ANIMAL:      return "Animal";
    case CLASS_DRAGON:      return "Dragon";
    case CLASS_GIANT:       return "Giant";
    default:                return "**UNKNOWN**";
    }
}

const char *class_abbrev(const sbyte chclass) {
    switch (chclass) {
    case CLASS_UNDEFINED:   return "??";
    case CLASS_MAGIC_USER:  return "Mg";
    case CLASS_CLERIC:      return "Cl";
    case CLASS_THIEF:       return "Th";
    case CLASS_WARRIOR:     return "Wa";
    case CLASS_OTHER:       return "Ot";
    case CLASS_UNDEAD:      return "Ud";
    case CLASS_HUMANOID:    return "Hd";
    case CLASS_ANIMAL:      return "An";
    case CLASS_DRAGON:      return "Dg";
    case CLASS_GIANT:       return "Gt";
    default:                return "XX";
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

const char *race_name(const sbyte chrace) {
    switch (chrace) {
    case RACE_UNKNOWN:  return "Unknown";
    case RACE_HUMAN:    return "Human";
    case RACE_TROLLOC:  return "Trolloc";
    case RACE_OGIER:    return "Ogier";
    case RACE_FADE:     return "Fade";
    default:            return "**UNKNOWN**";
    }
}

const char *race_abbrev(const sbyte chrace) {
    switch (chrace) {
    case RACE_UNKNOWN:  return "??";
    case RACE_HUMAN:    return "Hu";
    case RACE_TROLLOC:  return "Tr";
    case RACE_OGIER:    return "Og";
    case RACE_FADE:     return "Fa";
    default:            return "XX";
    }
}

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

ush_int classes_for_race[] = {
    CLASSB_ANYPC,                       //<! RACE_HUMAN,
    CLASSB_WARRIOR | CLASSB_THIEF,      //<! RACE_TROLLOC,
    CLASSB_CLERIC | CLASSB_WARRIOR,     //<! RACE_OGIER,
    CLASSB_MAGIC_USER | CLASSB_THIEF    //<! RACE_FADE,
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

        snprintf(buf, DEFAULT_STR_BUF, "Name [%s],\n"
            "\tClass [%d], Race [%d],\n"
            "\tStr [%d], Dex [%d] Con [%d],\n"
            "\tInt [%d], Wis [%d], Cha [%d]\n",
            name, chclass, chrace,
            real_abils.getStr(), real_abils.getDex(), real_abils.getCon(),
            real_abils.getIntel(), real_abils.getWis(), real_abils.getCha());

        return buf;
    }
};

#define GET_STR(ch) (ch->real_abils.getStr())
#define GET_CON(ch) (ch->real_abils.getCon())
#define GET_RACE(ch) (ch->chrace)
#define GET_CLASS(ch) (ch->chclass)

bool min_tests(size_t &pass_count, size_t &test_count);
bool max_tests(size_t &pass_count, size_t &test_count);
bool limit_tests(size_t &pass_count, size_t &test_count);

bool rand_tests(size_t &pass_count, size_t &test_count);
bool roll_tests(size_t &pass_count, size_t &test_count);

// messy, but idgaf
Colours_t &C_def = GetColoursObj();
Colours_t &C_none = GetColoursObj(COL_OFF);
Colours_t &C_on = GetColoursObj(COL_ON);
Colours_t *C = &C_def;

extern eLogLevels g_logLevel;

int parse_args(int argc, const char *argv[]);

//---------------------------------------------------------------------------
int main(int argc, const char *argv[])
{
    int rc = parse_args(argc, argv);
    if (rc != 0)
    {
        basic_log(LL_ERROR, "Could not parse arguments.");
        return rc;
    }

    size_t pass_count,  test_count, total_count = 0, total_pass = 0;
    double total_pct_pass = 0.0;

    circle_srandom((unsigned long)time(0));

    info_log("%s--- Test Min/Max/Limit ---%s", C->BYEL(), C->NRM());

    min_tests(pass_count, test_count);
    total_pass += pass_count;
    total_count += test_count;

    max_tests(pass_count, test_count);
    total_pass += pass_count;
    total_count += test_count;

    limit_tests(pass_count, test_count);
    total_pass += pass_count;
    total_count += test_count;

    info_log("%s--- Test random numbers ---%s", C->BYEL(), C->NRM());

    rand_tests(pass_count, test_count);
    total_pass += pass_count;
    total_count += test_count;

    info_log("%s--- Test roll by class ---%s", C->BYEL(), C->NRM());

    roll_tests(pass_count, test_count);
    total_pass += pass_count;
    total_count += test_count;

    if (total_count < 1) {
        total_pct_pass = 0;
    }
    else {
        total_pct_pass = ((double)total_pass / (double)total_count) * 100.0;
    }
    basic_log(LL_INFO, "===== %u/%u tests passed (%s) =====\n", total_pass, total_count, C->ColPct(total_pct_pass));

    fprintf(stderr, "");
    fprintf(stdout, "");
    fflush(stderr);
    fflush(stdout);
    return 0;
}

//---------------------------------------------------------------------------
static const char *const ap_usages[] = {
    "attrtest [options]",
    NULL,
};

int parse_args(int argc, const char *argv[])
{
    int rc = -1;

    g_logLevel = LL_INFO; // default level is info
    //g_logLevel = LL_DEVTRC;
    C = &C_def;  // use the terminal default colour

    const char *ll_str = NULL;
    int no_colour = 0;  // parser is C so use an int for boolean stuff

    struct argparse_option ap_options[] = {
        OPT_HELP(),
        //OPT_GROUP("Basic options"),
        OPT_STRING('l', "loglevel", &ll_str, "Set the log level to use (fatal,error,warn,info,debug,trace,dev).", NULL, 0, 0),
        OPT_BOOLEAN('n', "no-colour", &no_colour, "Disable colour display", NULL, 0, 0),
        OPT_END(),
    };

    struct argparse ap;
    argparse_init(&ap, ap_options, ap_usages, 0);
    argparse_describe(&ap, "\nTest attribute generation and adjustment for race and class", "\n");
    rc = argparse_parse(&ap, argc, argv);

    if (rc < 0)
    {
        fprintf(stderr, "Usage:\n");
        // @todo: actually do this as a function
    }
    else {
        // log level
        if (ll_str == NULL) {
            // not passed
        }
        else if (strlen(ll_str) == 0) {
            // not passed?
            dev_log("got empty log level string.");
        }
        else {
            const char *_ll_val = strrchr(ll_str, '=');
            if (_ll_val && (*_ll_val == '=')) {
                _ll_val++;
            }
            if (strlen(_ll_val) == 0) {
                // not passed?
                dev_log("got empty log level string.");
            }
            else if (_stricmp(_ll_val, "none") == 0) {
                g_logLevel = LL_NONE;  // diable logging - not much point, but...
            }
            else if (_stricmp(_ll_val, "fatal") == 0) {
                g_logLevel = LL_FATAL;
            }
            else if (_stricmp(_ll_val, "error") == 0) {
                g_logLevel = LL_ERROR;
            }
            else if (_stricmp(_ll_val, "warn") == 0) {
                g_logLevel = LL_WARN;
            }
            else if (_stricmp(_ll_val, "info") == 0) {
                g_logLevel = LL_INFO;
            }
            else if (_stricmp(_ll_val, "debug") == 0) {
                g_logLevel = LL_DEBUG;
            }
            else if (_stricmp(_ll_val, "trace") == 0) {
                g_logLevel = LL_TRACE;
            }
            else if (_stricmp(_ll_val, "dev") == 0) {
                g_logLevel = LL_DEVTRC;
            }
            else {
                error_log("Invalid log level [%s]", _ll_val);
                rc = -1;
            }
        }

        // colour off
        if (no_colour == 1) {
            dev_log("disable colour");
            C = &C_none;
        }
    }

    return rc;
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
    debug_log("> Run [%lu] min val tests", test_count);
    for (test_num = 0; test_num < test_count; ++test_num) {
        struct test_minmax &test = min_tests[test_num];
        int result = MIN(test.val1, test.val2);
        debug_log("%s[%2u/%2u] %s %24.24s: %s - Val 1 [%s%3d%s], Val 2 [%s%3d%s], Got [%s%3d%s], Expected [%s%3d%s]",
            C->CYN(), test_num+1, test_count, C->NRM(), test.name, (result == test.expected ? C->PASSED_STR() : C->FAILED_STR()),
            C->BBLK(), test.val1, C->NRM(), C->BBLK(), test.val2, C->NRM(), C->BBLK(), result, C->NRM(), C->BBLK(), test.expected, C->NRM());
        pass_count += (result == test.expected ? 1 : 0);
    }

    if (test_count < 1) {
        pct_pass = 0;
    }
    else {
        pct_pass = ((double)pass_count / (double)test_count) * 100.0;
    }
    info_log("> %u/%u tests passed (%s)\n", pass_count, test_count, C->ColPct(pct_pass));

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
    debug_log("> Run [%lu] max val tests", test_count);
    for (test_num = 0; test_num < test_count; ++test_num) {
        struct test_minmax &test = max_tests[test_num];
        int result = MAX(test.val1, test.val2);
        debug_log("%s[%2u/%2u] %s %24.24s: %s - Val 1 [%s%3d%s], Val 2 [%s%3d%s], Got [%s%3d%s], Expected [%s%3d%s]",
            C->CYN(), test_num + 1, test_count, C->NRM(), test.name, (result == test.expected ? C->PASSED_STR() : C->FAILED_STR()),
            C->BBLK(), test.val1, C->NRM(), C->BBLK(), test.val2, C->NRM(), C->BBLK(), result, C->NRM(), C->BBLK(), test.expected, C->NRM());
        pass_count += (result == test.expected ? 1 : 0);
    }

    if (test_count < 1) {
        pct_pass = 0;
    }
    else {
        pct_pass = ((double)pass_count / (double)test_count) * 100.0;
    }
    info_log("> %u/%u tests passed (%s)\n", pass_count, test_count, C->ColPct(pct_pass));

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
    debug_log("> Run [%lu] limit val tests", test_count);
    for (test_num = 0; test_num < test_count; ++test_num) {
        struct test_limit &test = limit_tests[test_num];
        int result = LIMIT(test.val, test.minv, test.maxv);
        debug_log("%s[%2u/%2u] %s %24.24s: %s - Min [%s%3d%s], Val [%s%3d%s], Max [%s%3d%s] -> Got [%s%3d%s], Expected [%s%3d%s]",
            C->CYN(), test_num + 1, test_count, C->NRM(), test.name, (result == test.expected ? C->PASSED_STR() : C->FAILED_STR()),
            C->BBLK(), test.minv, C->NRM(), C->BBLK(), test.val, C->NRM(), C->BBLK(), test.maxv, C->NRM(),
            C->BBLK(), result, C->NRM(), C->BBLK(), test.expected, C->NRM());
        pass_count += (result == test.expected ? 1 : 0);
    }

    if (test_count < 1) {
        pct_pass = 0;
    }
    else {
        pct_pass = ((double)pass_count / (double)test_count) * 100.0;
    }
    info_log("> %u/%u tests passed (%s)\n", pass_count, test_count, C->ColPct(pct_pass));

    return pass_count == test_count;
}

bool rand_tests(size_t &pass_count, size_t &test_count)
{
    size_t test_num = 0;
    double pct_pass = 0.0;

    test_count = 0;
    pass_count = 0;

    // 100 random numbers in the range 1-6
    // for (int i = 0; i < 100; ++i) {

    // }

    histo_t foo(1, 3);
    //basic_log("%s##%s Histo: %s", C->BMAG(), C->NRM(), (const char *)foo);
    foo.increment(1);
    //basic_log("%s##%s Histo: %s", C->BMAG(), C->NRM(), (const char *)foo);
    foo.increment(1);
    //basic_log("%s##%s Histo: %s", C->BMAG(), C->NRM(), (const char *)foo);
    foo.increment(3);
    //basic_log("%s##%s Histo: %s", C->BMAG(), C->NRM(), (const char *)foo);
    foo.increment(2, 5);
    //basic_log("%s##%s Histo: %s", C->BMAG(), C->NRM(), (const char *)foo);
    //basic_log("%s##%s", C->BMAG(), C->NRM());
    debug_log("%s##%s \n%s", C->BMAG(), C->NRM(), foo.graph());
    test_count++;
    pass_count += ((foo.m_numAbove+foo.m_numBelow) == 0 ? 1 : 0);

    histo_t foo2(1, 6);
    for (int i = 0; i < 300; ++i) {
    	foo2.increment(rand_number(1,6));
    }
    debug_log("%s##%s \n%s", C->BMAG(), C->NRM(), foo2.graph());
    test_count++;
    pass_count += ((foo2.m_numAbove + foo2.m_numBelow) == 0 ? 1 : 0);

    if (test_count < 1) {
        pct_pass = 0;
    }
    else {
        pct_pass = ((double)pass_count / (double)test_count) * 100.0;
    }
    info_log("> %u/%u tests passed (%s)\n", pass_count, test_count, C->ColPct(pct_pass));

    return pass_count == test_count;
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

struct roll_class_race {
    const char *prefix;
    eRaces chrace;
    sbyte chclass;
}
roll_race_class_data[] = {
    // RACE_HUMAN - ANY CLASS
    { "Hu_Wa", RACE_HUMAN, CLASS_WARRIOR },
    { "Hu_Cl", RACE_HUMAN, CLASS_CLERIC },
    { "Hu_Th", RACE_HUMAN, CLASS_THIEF },
    { "Hu_Mg", RACE_HUMAN, CLASS_MAGIC_USER },

    // RACE_TROLLOC - CLASSB_WARRIOR | CLASSB_THIEF
    { "Tr_Wa", RACE_TROLLOC, CLASS_WARRIOR },
    { "Tr_Th", RACE_TROLLOC, CLASS_THIEF },

    // RACE_OGIER - CLASSB_CLERIC | CLASSB_WARRIOR
    { "Og_Wa", RACE_OGIER, CLASS_WARRIOR },
    { "Og_Cl", RACE_OGIER, CLASS_CLERIC },

    // RACE_FADE - CLASSB_MAGIC_USER | CLASSB_THIEF
    { "Fa_Th", RACE_FADE, CLASS_THIEF },
    { "Fa_Mg", RACE_FADE, CLASS_MAGIC_USER },

    { "", RACE_UNKNOWN, CLASS_UNDEFINED }  // MUST BE LAST
};

bool roll_tests(size_t &pass_count, size_t &test_count)
{
    size_t test_num = 0;
    double pct_pass = 0.0;

    char buf1[DEFAULT_STR_BUF+1];

    test_count = 0;
    pass_count = 0;

    debug_log("%s-------------------------------------------------------%s", C->MAG(), C->NRM());
    size_t num_rc_combo = sizeof(roll_race_class_data) / sizeof(struct roll_class_race);

    for (size_t rc_idx = 0; rc_idx < num_rc_combo; ++rc_idx) {
        roll_class_race &rcData = roll_race_class_data[rc_idx];
        if (rcData.chrace == RACE_UNKNOWN) {
            break;
        }
        info_log("h_test: %sRace: %s%s%s, Class: %s%s%s",
            C->YEL(), C->CYN(), race_name(rcData.chrace), C->YEL(), C->CYN(), class_name(rcData.chclass), C->NRM());

        histo_t h_str(3, 18), h_dex(3, 18), h_con(3, 18), h_int(3, 18), h_wis(3, 18), h_cha(3, 18);
        for (size_t num_gen = 0; num_gen < 5000; num_gen++) {
            snprintf(buf1, DEFAULT_STR_BUF, "%s_%03d", rcData.prefix, num_gen + 1);
            char_data ch(buf1, rcData.chclass, rcData.chrace);

            roll_real_abils(&ch);
            dev_log(ch);
            h_str.increment(ch.real_abils.getStr());
            h_dex.increment(ch.real_abils.getDex());
            h_con.increment(ch.real_abils.getCon());
            h_int.increment(ch.real_abils.getIntel());
            h_wis.increment(ch.real_abils.getWis());
            h_cha.increment(ch.real_abils.getCha());
        }  // for (num_gen ...
        info_log("h_str: %s", h_str.str());
        info_log("h_dex: %s", h_dex.str());
        info_log("h_con: %s", h_con.str());
        info_log("h_int: %s", h_int.str());
        info_log("h_wis: %s", h_wis.str());
        info_log("h_cha: %s", h_cha.str());
        debug_log("%s-------------------------------------------------------%s", C->MAG(), C->NRM());

        test_count++;
        pass_count += ((h_str.m_numAbove + h_str.m_numBelow + h_dex.m_numAbove + h_dex.m_numBelow + h_con.m_numAbove + h_con.m_numBelow +
            h_int.m_numAbove + h_int.m_numBelow + h_wis.m_numAbove + h_wis.m_numBelow + h_cha.m_numAbove + h_cha.m_numBelow) == 0 ? 1 : 0);
    }  // for (rc_idx ...

    if (test_count < 1) {
        pct_pass = 0;
    }
    else {
        pct_pass = ((double)pass_count / (double)test_count) * 100.0;
    }
    info_log("> %u/%u tests passed (%s)\n", pass_count, test_count, C->ColPct(pct_pass));

    return pass_count == test_count;
}
