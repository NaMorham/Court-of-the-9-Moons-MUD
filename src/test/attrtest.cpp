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

const char *HISTO_DEF_PREFIX = "[...................] [.....] ";

#define GET_STR(ch) (ch->real_abils.getStr())
#define GET_CON(ch) (ch->real_abils.getCon())
#define GET_RACE(ch) (ch->chrace)
#define GET_CLASS(ch) (ch->chclass)
#define GET_NAME(ch) (ch->name)

bool min_tests(size_t &pass_count, size_t &test_count);
bool max_tests(size_t &pass_count, size_t &test_count);
bool limit_tests(size_t &pass_count, size_t &test_count);
bool histo_tests(size_t &pass_count, size_t &test_count);
bool rand_tests(size_t &pass_count, size_t &test_count);

bool roll_tests(size_t &pass_count, size_t &test_count);
bool roll_adj_tests(size_t &pass_count, size_t &test_count);

// messy, but idgaf
Colours_t &C_def = GetColoursObj();
Colours_t &C_none = GetColoursObj(COL_OFF);
Colours_t &C_on = GetColoursObj(COL_ON);
Colours_t *C = &C_def;

extern eLogLevels g_logLevel;
bool g_do_mintests = false;
bool g_do_maxtests = false;
bool g_do_limittests = false;
bool g_do_histotests = true;
bool g_do_randtests = false;
bool g_do_class_roll_tests = true;
bool g_do_race_adj_tests = true;

// reports for analysis
bool g_do_abil_histo_report = false;
bool g_do_raceadj_histo_report = false;

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

    if (g_do_mintests) {
        info_log("%s--- Test Min funcs ---%s", C->BYEL(), C->NRM());

        min_tests(pass_count, test_count);
        total_pass += pass_count;
        total_count += test_count;
    }

    if (g_do_maxtests) {
        info_log("%s--- Test Max funcs ---%s", C->BYEL(), C->NRM());

        max_tests(pass_count, test_count);
        total_pass += pass_count;
        total_count += test_count;
    }

    if (g_do_limittests) {
        info_log("%s--- Test Limit funcs ---%s", C->BYEL(), C->NRM());

        limit_tests(pass_count, test_count);
        total_pass += pass_count;
        total_count += test_count;
    }

    if (g_do_histotests) {
        info_log("%s--- Test histo type ---%s", C->BYEL(), C->NRM());

        histo_tests(pass_count, test_count);
        total_pass += pass_count;
        total_count += test_count;
    }

    if (g_do_randtests) {
        info_log("%s--- Test random numbers ---%s", C->BYEL(), C->NRM());

        rand_tests(pass_count, test_count);
        total_pass += pass_count;
        total_count += test_count;
    }

    if (g_do_class_roll_tests) {
        info_log("%s--- Test roll by class ---%s", C->BYEL(), C->NRM());

        roll_tests(pass_count, test_count);
        total_pass += pass_count;
        total_count += test_count;
    }

    if (g_do_race_adj_tests) {
        info_log("%s--- Test roll adjusted by race ---%s", C->BYEL(), C->NRM());

        roll_adj_tests(pass_count, test_count);
        total_pass += pass_count;
        total_count += test_count;
    }

    if (total_count < 1) {
        total_pct_pass = 0;
    }
    else {
        total_pct_pass = ((double)total_pass / (double)total_count) * 100.0;
    }
    basic_log((total_pass == total_count && total_count >= 1 ? LL_INFO : LL_WARN),
        "===== %u/%u tests passed (%s) =====\n", total_pass, total_count, C->ColPct(total_pct_pass));

    /*
    histo_t hv(3, 8);
    for (size_t n = 0; n < 150; n++) {
        hv.increment(rand_number(1, 10));
    }
    debug_log("%s##%s\n%s", C->BWHT(), C->NRM(), hv.str());
    debug_log("%s##%s\n%s", C->BWHT(), C->NRM(), hv.graph(HISTO_DEF_PREFIX));
     */

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
    int do_min = 0;
    int do_max = 0;
    int do_limit = 0;
    int do_histo = 0;
    int do_rand = 0;
    int skip_abil_test = 0;
    int do_abil_data = 0;
    int skip_adj_test = 0;
    int do_adj_data = 0;

    struct argparse_option ap_options[] = {
        OPT_HELP(),
        OPT_GROUP("Basic options"),
        OPT_STRING('v', "log-level", &ll_str, "Set the log level to use (fatal,error,warn,info,debug,trace,dev).", NULL, 0, 0),
        OPT_BOOLEAN('c', "no-colour", &no_colour, "Disable colour display", NULL, 0, 0),
        OPT_GROUP("Test options"),
        OPT_BOOLEAN('i', "do-min", &do_min, "Run the min value tests", NULL, 0, 0),
        OPT_BOOLEAN('x', "do-max", &do_max, "Run the max value tests", NULL, 0, 0),
        OPT_BOOLEAN('l', "do-limit", &do_limit, "Run the limit value tests", NULL, 0, 0),
        OPT_BOOLEAN(0, "do-histo", &do_histo, "Run the histo type tests", NULL, 0, 0),
        OPT_BOOLEAN('r', "do-rand", &do_rand, "Run the rand value tests", NULL, 0, 0),
        OPT_BOOLEAN('a', "skip-abil-test", &skip_abil_test, "Skip the roll abils class test", NULL, 0, 0),
        OPT_BOOLEAN('j', "skip-adj-test", &skip_adj_test, "Skip the race adjusted roll abils test", NULL, 0, 0),
        OPT_GROUP("Report options"),
        OPT_BOOLEAN('A', "do-abil-data", &do_abil_data, "Print the roll abils data to std out", NULL, 0, 0),
        OPT_BOOLEAN('J', "do-abil-data", &do_adj_data, "Print the race adjusted roll abils data to std out", NULL, 0, 0),
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

        if (do_min == 1) {
            dev_log("Perform min value tests");
            g_do_mintests = true;
        }

        if (do_max == 1) {
            dev_log("Perform max value tests");
            g_do_maxtests = true;
        }

        if (do_limit == 1) {
            dev_log("Perform limit value tests");
            g_do_limittests = true;
        }

        if (do_histo == 1) {
            dev_log("Perform histogram type tests");
            g_do_histotests = true;
        }

        if (do_rand == 1) {
            dev_log("Perform random value tests");
            g_do_randtests = true;
        }

        if (skip_abil_test == 1) {
            dev_log("Skip ability roll test");
            g_do_class_roll_tests = false;
        }

        if (skip_adj_test == 1) {
            dev_log("Skip race adjusted attribute roll test");
            g_do_race_adj_tests = false;
        }

        if (do_abil_data == 1) {
            dev_log("Generate the ability generation report");
            g_do_abil_histo_report = true;
        }

        if (do_adj_data == 1) {
            dev_log("Generate the attributes adjusted for race report");
            g_do_raceadj_histo_report = true;
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
    basic_log((pass_count == test_count ? LL_INFO : LL_WARN), "> %u/%u tests passed (%s)\n", pass_count, test_count, C->ColPct(pct_pass));

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
    basic_log((pass_count == test_count ? LL_INFO : LL_WARN), "> %u/%u tests passed (%s)\n", pass_count, test_count, C->ColPct(pct_pass));

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
    basic_log((pass_count == test_count ? LL_INFO : LL_WARN), "> %u/%u tests passed (%s)\n", pass_count, test_count, C->ColPct(pct_pass));

    return pass_count == test_count;
}

struct hist_test_data_t;

typedef void(*fDoTestVals)(histo_t &h, hist_test_data_t &);

void simpleTest(histo_t &h, hist_test_data_t &td);
void simpleTest10(histo_t &h, hist_test_data_t &td);
void simpleTestBelow(histo_t &h, hist_test_data_t &td);
void simpleTestAbove(histo_t &h, hist_test_data_t &td);

struct hist_test_data_t {
    const char *name;
    int base;
    int top;

    struct {
        size_t numBuckets;
        size_t numBelow;
        size_t numAbove;
        size_t numVals;

        int minVal;
        int maxVal;
    } exp;

    fDoTestVals testValsFunc;
}
hist_test_data[] = {
    { "Empty test", 0, 0, { 1, 0, 0, 0, INT_MAX, INT_MIN }, NULL },
    { "Simple empty test 1", 1, 3, { 3, 0, 0, 0, INT_MAX, INT_MIN }, NULL },
    { "Simple empty test 2", 2, 5, { 4, 0, 0, 0, INT_MAX, INT_MIN }, NULL },
    { "Reverse bounds test 3", 5, 2, { 4, 0, 0, 0, INT_MAX, INT_MIN }, NULL },
    { "Simple test 1", 1, 3, { 3, 0, 0, 4, 1, 3 }, &simpleTest },
    { "Simple test 2 (in range)", 1, 3, { 3, 0, 0, 30, 1, 3 }, &simpleTest10 },
    { "Simple test (below range)", 1, 3, { 3, 2, 0, 5, -1, 3 }, &simpleTestBelow },
    { "Simple test (above range)", 1, 3, { 3, 0, 2, 5, 1, 9000 }, &simpleTestAbove },
};

void simpleTest(histo_t &h, hist_test_data_t &td) {
    h.increment(1);
    h.increment(1);
    h.increment(3);
}

void simpleTest10(histo_t &h, hist_test_data_t &td) {
    for (size_t val = 0; val < h.m_numVals; ++val) {
        for (size_t i = 0; i < 10; ++i) {
            h.increment(val+h.m_base);
        }
    }
}

void simpleTestBelow(histo_t &h, hist_test_data_t &td) {
    h.increment(1);
    h.increment(2);
    h.increment(3);

    h.increment(0);
    h.increment(-1);
}

void simpleTestAbove(histo_t &h, hist_test_data_t &td) {
    h.increment(1);
    h.increment(2);
    h.increment(3);

    h.increment(4);
    h.increment(9000);
}

bool histo_tests(size_t &pass_count, size_t &test_count)
{
    size_t test_num = 0;
    size_t num_tests = 0;
    double pct_pass = 0.0;
    bool pass = false;

    test_count = 0;
    pass_count = 0;

    // -------------------------------------------
    //{
    //    pass = true;
    //    test_count++;
    //    debug_log("%sSimple test 1:%s 1 -> 3, empty", C->BMAG(), C->NRM());
    //    histo_t t1(1, 3);
    //    t1.debug_dump();
    //    if (t1.m_histo) {
    //        debug_log("  Has data object: %s", C->PASSED_STR());
    //    }
    //    else {
    //        debug_log("  Has data object: %s", C->FAILED_STR());
    //        pass = false;
    //    }
    //    if (t1.m_base == 1) {
    //        debug_log("  Base value (%ld == %ld): %s", t1.m_base, 1, C->PASSED_STR());
    //    }
    //    else {
    //        debug_log("  Base value (%ld == %ld): %s", t1.m_base, 1, C->FAILED_STR());
    //        pass = false;
    //    }
    //    if (t1.m_top == 3) {
    //        debug_log("  Top value (%ld == %ld): %s", t1.m_top, 3, C->PASSED_STR());
    //    }
    //    else {
    //        debug_log("  Top value (%ld == %ld): %s", t1.m_top, 3, C->FAILED_STR());
    //        pass = false;
    //    }
    //    if (t1.m_numVals == 3) {
    //        debug_log("  Num buckets (%ld == %ld): %s", t1.m_numVals, 3, C->PASSED_STR());
    //    }
    //    else {
    //        debug_log("  Num buckets (%ld == %ld): %s", t1.m_numVals, 3, C->FAILED_STR());
    //        pass = false;
    //    }
    //    pass_count += (pass ? 1 : 0);
    //    debug_log("--------------------------------------\n");
    //}

    //// -------------------------------------------
    //{
    //    pass = true;
    //    test_count++;
    //    debug_log("%sSimple test 2:%s 2 -> 5, empty", C->BMAG(), C->NRM());
    //    histo_t t2(2, 5);
    //    t2.debug_dump();
    //    if (t2.m_histo) {
    //        debug_log("  Has data object: %s", C->PASSED_STR());
    //    }
    //    else {
    //        debug_log("  Has data object: %s", C->FAILED_STR());
    //        pass = false;
    //    }
    //    if (t2.m_base == 2) {
    //        debug_log("  Base value (%ld == %ld): %s", t2.m_base, 2, C->PASSED_STR());
    //    }
    //    else {
    //        debug_log("  Base value (%ld == %ld): %s", t2.m_base, 2, C->FAILED_STR());
    //        pass = false;
    //    }
    //    if (t2.m_top == 5) {
    //        debug_log("  Top value (%ld == %ld): %s", t2.m_top, 5, C->PASSED_STR());
    //    }
    //    else {
    //        debug_log("  Top value (%ld == %ld): %s", t2.m_top, 5, C->FAILED_STR());
    //        pass = false;
    //    }
    //    if (t2.m_numVals == 4) {
    //        debug_log("  Num buckets (%ld == %ld): %s", t2.m_numVals, 4, C->PASSED_STR());
    //    }
    //    else {
    //        debug_log("  Num buckets (%ld == %ld): %s", t2.m_numVals, 4, C->FAILED_STR());
    //        pass = false;
    //    }
    //    pass_count += (pass ? 1 : 0);
    //    debug_log("--------------------------------------\n");
    //}

    // -------------------------------------------
    num_tests = sizeof(hist_test_data) / sizeof(hist_test_data_t);
    debug_log("%s=============== %u ===============%s", C->YEL(), num_tests, C->NRM());
    for (test_num = 0; test_num < num_tests; test_num++) {
        test_count++;
        hist_test_data_t &htd = hist_test_data[test_num];
        debug_log("%s## Histo test [%lu/%lu] -%s \"%s%s%s\" (%ld, %ld)",
            C->BMAG(), test_num+1, num_tests, C->NRM(),
            C->CYN(), htd.name, C->NRM(), htd.base, htd.top);

        histo_t tdata(htd.base, htd.top);
        //tdata.debug_dump();

        debug_log("  Has data object: %s",
            (tdata.m_histo ? C->PASSED_STR() : C->FAILED_STR()));
        pass = (tdata.m_histo != NULL);

        debug_log("  Base value (%ld == %ld): %s", tdata.m_base, (htd.base < htd.top ? htd.base : htd.top),
            ((tdata.m_base == (htd.base < htd.top ? htd.base : htd.top)) ? C->PASSED_STR() : C->FAILED_STR()));
        pass = pass && (tdata.m_base == (htd.base < htd.top ? htd.base : htd.top));

        debug_log("  Top value (%ld == %ld): %s", tdata.m_top, (htd.top > htd.base ? htd.top : htd.base),
            ((tdata.m_top == (htd.top > htd.base ? htd.top : htd.base)) ? C->PASSED_STR() : C->FAILED_STR()));
        pass = pass && (tdata.m_top == (htd.top > htd.base ? htd.top : htd.base));

        debug_log("  Num buckets (%ld == %ld): %s", tdata.m_numVals, htd.exp.numBuckets,
            ((tdata.m_numVals == htd.exp.numBuckets) ? C->PASSED_STR() : C->FAILED_STR()));
        pass = pass && (tdata.m_numVals == htd.exp.numBuckets);

        // apply the test data
        if (htd.testValsFunc) {
            debug_log("  %sTest data func %s = Found", C->GRN(), C->NRM());
            htd.testValsFunc(tdata, htd);
        } else {
            debug_log("  %sTest data func %s = Not Found", C->YEL(), C->NRM());
        }

        debug_log("  Num above (%ld == %ld): %s", tdata.m_numAbove, htd.exp.numAbove,
            ((tdata.m_numAbove == htd.exp.numAbove) ? C->PASSED_STR() : C->FAILED_STR()));
        pass = pass && (tdata.m_numAbove == htd.exp.numAbove);

        debug_log("  Num below (%ld == %ld): %s", tdata.m_numBelow, htd.exp.numBelow,
            ((tdata.m_numBelow == htd.exp.numBelow) ? C->PASSED_STR() : C->FAILED_STR()));
        pass = pass && (tdata.m_numBelow == htd.exp.numBelow);

        debug_log("  Min val (%ld == %ld): %s", tdata.m_minVal, htd.exp.minVal,
            ((tdata.m_minVal == htd.exp.minVal) ? C->PASSED_STR() : C->FAILED_STR()));
        pass = pass && (tdata.m_minVal == htd.exp.minVal);

        debug_log("  Max val (%ld == %ld): %s", tdata.m_maxVal, htd.exp.maxVal,
            ((tdata.m_maxVal == htd.exp.maxVal) ? C->PASSED_STR() : C->FAILED_STR()));
        pass = pass && (tdata.m_maxVal == htd.exp.maxVal);

        tdata.debug_dump();
        debug_log("%s data:%s %s", C->MAG(), C->NRM(), tdata.str());
        debug_log("%sgraph:%s\n%s", C->MAG(), C->NRM(), tdata.graph(HISTO_DEF_PREFIX));

        pass_count += (pass ? 1 : 0);
        debug_log("%s--------------------------------------%s", C->YEL(), C->NRM());
    }

    // -------------------------------------------
    if (test_count < 1) {
        pct_pass = 0;
    }
    else {
        pct_pass = ((double)pass_count / (double)test_count) * 100.0;
    }
    basic_log((pass_count == test_count ? LL_INFO : LL_WARN), "> %u/%u tests passed (%s)\n", pass_count, test_count, C->ColPct(pct_pass));

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
    basic_log((pass_count == test_count ? LL_INFO : LL_WARN), "> %u/%u tests passed (%s)\n", pass_count, test_count, C->ColPct(pct_pass));

    return pass_count == test_count;
}

#define C_YEL C->YEL()
#define C_CYN C->CYN()
#define C_NRM C->NRM()

void dump_stats(struct char_data &ch)
{
    trace_log("%sName:%s \"%s%s%s\", "
        "%sClass:%s \"%s%s%s\", "
        "%sRace:%s \"%s%s%s\", "
        "%sStr%s [%s%u%s/%s%u%s], %sDex%s [%s%u%s], %sCon%s [%s%u%s], "
        "%sInt%s [%s%u%s], %sWis%s [%s%u%s], %sCha%s [%s%u%s]",
        C_YEL, C_NRM, C_CYN, ch.name, C_NRM,
        C_YEL, C_NRM, C_CYN, class_name(ch.chclass), C_NRM,
        C_YEL, C_NRM, C_CYN, race_name(ch.chrace), C_NRM,
        C_YEL, C_NRM, C_CYN, ch.real_abils.getStr(), C_NRM, C_CYN, ch.real_abils.getStrAdd(), C_NRM,
        C_YEL, C_NRM, C_CYN, ch.real_abils.getDex(), C_NRM,
        C_YEL, C_NRM, C_CYN, ch.real_abils.getCon(), C_NRM,
        C_YEL, C_NRM, C_CYN, ch.real_abils.getIntel(), C_NRM,
        C_YEL, C_NRM, C_CYN, ch.real_abils.getWis(), C_NRM,
        C_YEL, C_NRM, C_CYN, ch.real_abils.getCha(), C_NRM);
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
    attr_data minvals;
    attr_data maxvals;
}
roll_race_class_data[] = {
    // RACE_HUMAN - ANY CLASS
    //{ "Hu_Wa", RACE_HUMAN, CLASS_WARRIOR, { 3, 3, 3, 3, 3, 3, 3 }, { 18, 18, 18, 18, 18, 18, 18 } },
    //{ "Hu_Cl", RACE_HUMAN, CLASS_CLERIC, { 3, 3, 3, 3, 3, 3, 3 }, { 18, 18, 18, 18, 18, 18, 18 } },
    //{ "Hu_Th", RACE_HUMAN, CLASS_THIEF, { 3, 3, 3, 3, 3, 3, 3 }, { 18, 18, 18, 18, 18, 18, 18 } },
    //{ "Hu_Mg", RACE_HUMAN, CLASS_MAGIC_USER, { 3, 3, 3, 3, 3, 3, 3 }, { 18, 18, 18, 18, 18, 18, 18 } },

    //// RACE_TROLLOC - CLASSB_WARRIOR | CLASSB_THIEF
    { "Tr_Wa", RACE_TROLLOC, CLASS_WARRIOR, { 6, 3, 6, 3, 3, 3, 3 }, { 21, 18, 21, 18, 18, 18, 18 } },
    //{ "Tr_Th", RACE_TROLLOC, CLASS_THIEF },

    //// RACE_OGIER - CLASSB_CLERIC | CLASSB_WARRIOR
    //{ "Og_Wa", RACE_OGIER, CLASS_WARRIOR },
    //{ "Og_Cl", RACE_OGIER, CLASS_CLERIC },

    //// RACE_FADE - CLASSB_MAGIC_USER | CLASSB_THIEF
    //{ "Fa_Th", RACE_FADE, CLASS_THIEF },
    //{ "Fa_Mg", RACE_FADE, CLASS_MAGIC_USER },

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
            dump_stats(ch);

            h_str.increment(ch.real_abils.getStr());
            h_dex.increment(ch.real_abils.getDex());
            h_con.increment(ch.real_abils.getCon());
            h_int.increment(ch.real_abils.getIntel());
            h_wis.increment(ch.real_abils.getWis());
            h_cha.increment(ch.real_abils.getCha());
        }  // for (num_gen ...
        debug_log("%sh_str%s: %s", C->MAG(), C->NRM(), h_str.str());
        debug_log("%sh_dex%s: %s", C->MAG(), C->NRM(), h_dex.str());
        debug_log("%sh_con%s: %s", C->MAG(), C->NRM(), h_con.str());
        debug_log("%sh_int%s: %s", C->MAG(), C->NRM(), h_int.str());
        debug_log("%sh_wis%s: %s", C->MAG(), C->NRM(), h_wis.str());
        debug_log("%sh_cha%s: %s", C->MAG(), C->NRM(), h_cha.str());
        debug_log("%s-------------------------------------------------------%s", C->MAG(), C->NRM());
        debug_log("%sh_str%s:\n%s", C->MAG(), C->NRM(), h_str.graph(HISTO_DEF_PREFIX));
        debug_log("%sh_dex%s:\n%s", C->MAG(), C->NRM(), h_dex.graph(HISTO_DEF_PREFIX));
        debug_log("%sh_con%s:]n%s", C->MAG(), C->NRM(), h_con.graph(HISTO_DEF_PREFIX));
        debug_log("%sh_int%s:\n%s", C->MAG(), C->NRM(), h_int.graph(HISTO_DEF_PREFIX));
        debug_log("%sh_wis%s:\n%s", C->MAG(), C->NRM(), h_wis.graph(HISTO_DEF_PREFIX));
        debug_log("%sh_cha%s:\n%s", C->MAG(), C->NRM(), h_cha.graph(HISTO_DEF_PREFIX));
        debug_log("%s-------------------------------------------------------%s", C->MAG(), C->NRM());


        if (g_do_abil_histo_report) {
            printf("\n\n%s,%s\n", "Class,Race,Attribute,Total", h_str.csvHeader(',', true));
            printf("%s,%s,%s,%lu,%s\n", "Str", class_name(rcData.chclass), race_name(rcData.chrace), h_str.m_numVals, h_str.csv(',', false, true));
            printf("%s,%s,%s,%lu,%s\n", "Dex", class_name(rcData.chclass), race_name(rcData.chrace), h_str.m_numVals, h_dex.csv(',', false, true));
            printf("%s,%s,%s,%lu,%s\n", "Con", class_name(rcData.chclass), race_name(rcData.chrace), h_str.m_numVals, h_con.csv(',', false, true));
            printf("%s,%s,%s,%lu,%s\n", "Int", class_name(rcData.chclass), race_name(rcData.chrace), h_str.m_numVals, h_int.csv(',', false, true));
            printf("%s,%s,%s,%lu,%s\n", "Wis", class_name(rcData.chclass), race_name(rcData.chrace), h_str.m_numVals, h_wis.csv(',', false, true));
            printf("%s,%s,%s,%lu,%s\n", "Cha", class_name(rcData.chclass), race_name(rcData.chrace), h_str.m_numVals, h_cha.csv(',', false, true));
        }

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
    basic_log((pass_count == test_count ? LL_INFO : LL_WARN), "> %u/%u tests passed (%s)\n", pass_count, test_count, C->ColPct(pct_pass));

    return pass_count == test_count;
}

void adjust_abils_for_race(struct char_data *ch) {
    // basic version.  stat generation eeds a redo anyway
    int irand = 0;

    /*
    CLASSB_ANYPC,                       //<! RACE_HUMAN,
    CLASSB_WARRIOR | CLASSB_THIEF,      //<! RACE_TROLLOC,
    CLASSB_CLERIC | CLASSB_WARRIOR,     //<! RACE_OGIER,
    CLASSB_MAGIC_USER | CLASSB_THIEF    //<! RACE_FADE,
    */
    //switch (ch->player.chrace) {
    switch (ch->chrace) {
    case RACE_HUMAN:
        //mudlog(CMP, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), TRUE, "Adjust stats for human player '%s'", GET_NAME(ch));
        dev_log("Adjust stats for human player '%s'", GET_NAME(ch));
        // nope, humans are middle of the road
        break;
    case RACE_TROLLOC:
        //mudlog(CMP, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), TRUE, "Adjust stats for trolloc player '%s'", GET_NAME(ch));
        // trollocs are big, strong or fast and dumb
        // allow for class choice?
        //switch (ch->player.chclass) {
        switch (ch->chclass) {
        case CLASS_WARRIOR:
            // str and con up.
            // stronger or tougher?
            irand = rand_number(1, 3);

            // if we have a 1, clearly stronger
            // if we have a 2 and are already stronger, enhance it
            if ((irand == 1) || ((irand == 2) && (GET_STR(ch)>GET_CON(ch)))) {
                // stronger
                // str up by 2-3
                ch->real_abils.setStr(LIMIT(ch->real_abils.getStr() + rand_number(2, 3), 7, 21));
                ch->real_abils.setCon(LIMIT(ch->real_abils.getCon() + rand_number(1, 2), 6, 21));
            }
            else {  // equivalent to (irand == 3) || ((irand == 2) && (GET_CON(ch)>=GET_STR(ch)))
                // tougher
                ch->real_abils.setStr(LIMIT(ch->real_abils.getStr() + rand_number(1, 2), 6, 21));
                ch->real_abils.setCon(LIMIT(ch->real_abils.getCon() + rand_number(2, 3), 7, 21));
            }
            break;
        case CLASS_THIEF:
            // dex and str up.
            ch->real_abils.setDex(LIMIT(ch->real_abils.getDex() + rand_number(2, 3), 8, 21));
            ch->real_abils.setStr(LIMIT(ch->real_abils.getStr() + rand_number(1, 2), 5, 20));
            break;
        default:
            warn_log("SYSERR: adjust_abils_for_race: Trolloc character '%s' has invalid class [%u]", GET_NAME(ch), ch->chclass);
        }
        //ch->real_abils.setStrAdd(LIMIT(ch->real_abils.getStrAdd()+rand_number(2,3), 7, 21));  // todo - adjust str add

        // regardless, wis down quite a lot - they cannot resist anything
        ch->real_abils.setWis(LIMIT(ch->real_abils.getWis() - rand_number(3, 5), 1, 12));  // if they are lucky they MIGHT reach average human
        // regardless, int down quite a bit
        ch->real_abils.setIntel(LIMIT(ch->real_abils.getIntel() - rand_number(2, 3), 2, 14));  // if they are lucky they MIGHT reach average human
        // regardless, cha down a bit
        ch->real_abils.setCha(LIMIT(ch->real_abils.getCha() - rand_number(2, 4), 1, 13));  // if they are lucky they MIGHT reach average human
        break;

    case RACE_OGIER:
        // Ogier are strong and intelligent, but not quick
        // allow for class choice?
        //switch (ch->player.chclass) {
        switch (ch->chclass) {
        case CLASS_CLERIC:
            // wis and con up
            ch->real_abils.setWis(LIMIT(ch->real_abils.getWis() + rand_number(2, 4), 14, 21));
            ch->real_abils.setCon(LIMIT(ch->real_abils.getCon() + rand_number(1, 2), 11, 20));
            break;
        case CLASS_WARRIOR:
            // wis and str up
            ch->real_abils.setStr(LIMIT(ch->real_abils.getStr() + rand_number(2, 4), 14, 21));
            ch->real_abils.setWis(LIMIT(ch->real_abils.getWis() + rand_number(1, 2), 11, 20));
            break;
        default:
            warn_log("SYSERR: adjust_abils_for_race: Ogier character '%s' has invalid class [%u]", GET_NAME(ch), ch->chclass);
        }
        //ch->real_abils.setStrAdd(LIMIT(ch->real_abils.getStrAdd()+rand_number(2,3), 7, 21));  // todo - adjust str add

        // regardless, dex down quite a lot
        ch->real_abils.setDex(LIMIT(ch->real_abils.getDex() - rand_number(2, 4), 2, 16));
        // regardless, int up a little
        ch->real_abils.setIntel(LIMIT(ch->real_abils.getIntel() + rand_number(1, 2), 12, 20));
        break;
    case RACE_FADE:
        // Fades are quick, dangerous and resistant to magical effects
        //switch (ch->player.chclass) {
        switch (ch->chclass) {
        case CLASS_WARRIOR:
            ch->real_abils.setDex(LIMIT(ch->real_abils.getDex() + rand_number(2, 3), 14, 21));
            ch->real_abils.setStr(LIMIT(ch->real_abils.getStr() + rand_number(1, 3), 10, 18));
            break;
        case CLASS_THIEF:
            ch->real_abils.setDex(LIMIT(ch->real_abils.getDex() + rand_number(4, 5), 17, 21));
        default:
            warn_log("SYSERR: adjust_abils_for_race: Fade character '%s' has invalid class [%u]", GET_NAME(ch), ch->chclass);
        }
        //ch->real_abils.setStrAdd(LIMIT(ch->real_abils.getStrAdd()+rand_number(2,3), 7, 21));  // todo - adjust str add

        // regardless, wis up a little
        ch->real_abils.setWis(LIMIT(ch->real_abils.getWis() + rand_number(0, 1), 12, 19));
        // cha 3 - nothing likes a fade
        ch->real_abils.setCha(3);  // @todo: D&D players option would make this better, fades are VERY intimidating, just not liked by anything
        // con down a bit - fast and dangerous, but for balance, not robust
        ch->real_abils.setCon(LIMIT(ch->real_abils.getCon() - rand_number(1, 5), 3, 13));
        break;
    default:
        warn_log("SYSERR: adjust_abils_for_race: Character '%s' has invalid race [%u]", GET_NAME(ch), ch->chrace);
        //mudlog(BRF, MAX(LVL_IMMORT, GET_INVIS_LEV(d->character)), TRUE, "Failure to AddRecentPlayer (returned FALSE).");
    }
}

bool roll_adj_tests(size_t &pass_count, size_t &test_count)
{
    size_t test_num = 0;
    double pct_pass = 0.0;

    char buf1[DEFAULT_STR_BUF + 1];

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

        histo_t h_str(rcData.minvals.getStr(), rcData.maxvals.getStr());
        histo_t h_dex(rcData.minvals.getDex(), rcData.maxvals.getDex());
        histo_t h_con(rcData.minvals.getCon(), rcData.maxvals.getCon());
        histo_t h_int(rcData.minvals.getIntel(), rcData.maxvals.getIntel());
        histo_t h_wis(rcData.minvals.getWis(), rcData.maxvals.getWis());
        histo_t h_cha(rcData.minvals.getCha(), rcData.maxvals.getCha());
        for (size_t num_gen = 0; num_gen < 5000; num_gen++) {
            snprintf(buf1, DEFAULT_STR_BUF, "%s_%03d", rcData.prefix, num_gen + 1);
            char_data ch(buf1, rcData.chclass, rcData.chrace);

            roll_real_abils(&ch);
            adjust_abils_for_race(&ch);
            dump_stats(ch);

            h_str.increment(ch.real_abils.getStr());
            h_dex.increment(ch.real_abils.getDex());
            h_con.increment(ch.real_abils.getCon());
            h_int.increment(ch.real_abils.getIntel());
            h_wis.increment(ch.real_abils.getWis());
            h_cha.increment(ch.real_abils.getCha());
        }  // for (num_gen ...
        debug_log("%sh_str%s: %s", C->MAG(), C->NRM(), h_str.str());
        debug_log("%sh_dex%s: %s", C->MAG(), C->NRM(), h_dex.str());
        debug_log("%sh_con%s: %s", C->MAG(), C->NRM(), h_con.str());
        debug_log("%sh_int%s: %s", C->MAG(), C->NRM(), h_int.str());
        debug_log("%sh_wis%s: %s", C->MAG(), C->NRM(), h_wis.str());
        debug_log("%sh_cha%s: %s", C->MAG(), C->NRM(), h_cha.str());
        debug_log("%s-------------------------------------------------------%s", C->MAG(), C->NRM());
        //debug_log("%sh_str%s: %s", C->MAG(), C->NRM(), h_str.graph(HISTO_DEF_PREFIX));
        //debug_log("%sh_dex%s: %s", C->MAG(), C->NRM(), h_dex.graph(HISTO_DEF_PREFIX));
        //debug_log("%sh_con%s: %s", C->MAG(), C->NRM(), h_con.graph(HISTO_DEF_PREFIX));
        //debug_log("%sh_int%s: %s", C->MAG(), C->NRM(), h_int.graph(HISTO_DEF_PREFIX));
        //debug_log("%sh_wis%s: %s", C->MAG(), C->NRM(), h_wis.graph(HISTO_DEF_PREFIX));
        //debug_log("%sh_cha%s: %s", C->MAG(), C->NRM(), h_cha.graph(HISTO_DEF_PREFIX));
        //debug_log("%s-------------------------------------------------------%s", C->MAG(), C->NRM());

        if (g_do_raceadj_histo_report) {
            printf("\n\n%s,%s\n", "Class,Race,Attribute,Total", h_str.csvHeader(',', true));
            printf("%s,%s,%s,%lu,%s\n", "Str", class_name(rcData.chclass), race_name(rcData.chrace), h_str.m_numVals, h_str.csv(',', false, true));
            printf("%s,%s,%s,%lu,%s\n", "Dex", class_name(rcData.chclass), race_name(rcData.chrace), h_str.m_numVals, h_dex.csv(',', false, true));
            printf("%s,%s,%s,%lu,%s\n", "Con", class_name(rcData.chclass), race_name(rcData.chrace), h_str.m_numVals, h_con.csv(',', false, true));
            printf("%s,%s,%s,%lu,%s\n", "Int", class_name(rcData.chclass), race_name(rcData.chrace), h_str.m_numVals, h_int.csv(',', false, true));
            printf("%s,%s,%s,%lu,%s\n", "Wis", class_name(rcData.chclass), race_name(rcData.chrace), h_str.m_numVals, h_wis.csv(',', false, true));
            printf("%s,%s,%s,%lu,%s\n", "Cha", class_name(rcData.chclass), race_name(rcData.chrace), h_str.m_numVals, h_cha.csv(',', false, true));
        }

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
    basic_log((pass_count == test_count ? LL_INFO : LL_WARN), "> %u/%u tests passed (%s)\n", pass_count, test_count, C->ColPct(pct_pass));

    return pass_count == test_count;
}