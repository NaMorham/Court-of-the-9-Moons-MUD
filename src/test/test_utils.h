
void basic_log(const char *format, ...);
void basic_vlog(const char *format, va_list args);

#define WriteLogf basic_log

typedef signed char sbyte;          //!< 1 byte; vals = -127 to 127
typedef unsigned char ubyte;        //!< 1 byte; vals = 0 to 255
typedef signed short int sh_int;    //!< 2 bytes; vals = -32,768 to 32,767
typedef unsigned short int ush_int; //!< 2 bytes; vals = 0 to 65,535

#if !defined(__cplusplus)   // Anyone know a portable method?
typedef char bool;          //!< Technically 1 signed byte; vals should only = TRUE or FALSE.
# if !defined(CIRCLE_WINDOWS) || defined(LCC_WIN32)    // Hm, sysdep.h?
typedef signed char byte;   //!< Technically 1 signed byte; vals should only = TRUE or FALSE.
# endif
#endif

const size_t DEFAULT_STR_BUF = 1024;

// @todo: template this
inline int MIN(const int a, const int b) { return (a < b ? a : b); }
inline int MAX(const int a, const int b) { return (a > b ? a : b); }
inline int LIMIT(const int val, const int minv, const int maxv) {
    return (maxv >= minv ? MAX(minv, MIN(maxv, val)) : LIMIT(val, maxv, minv));
}

void circle_srandom(unsigned long initial_seed);
unsigned long circle_random(void);
const int rand_number(const int from, const int to);

//-----------------------------------------------------------------------------
struct histo_t {
private:
    const char *gline(const size_t line_len);
public:
    histo_t(int minv, int maxv);
    ~histo_t();
    histo_t &increment(const int val, const size_t amt = 1);
    const char *str() const;
    operator const char *() const { return this->str(); }
    const char *graph();
    size_t m_numVals;
    size_t *m_histo;

    int m_base;
    int m_top;

    size_t m_count;
    int m_minVal;
    int m_maxVal;
    size_t m_numBelow;
    size_t m_numAbove;

    static const size_t DEFAULT_STR_BUF;
};

//-----------------------------------------------------------------------------
struct _ColourVals_t {
    const char *C_NRM;
    const char *C_BLD;
    const char *C_ITL;
    const char *C_UND;
    const char *C_BLK;
    const char *C_RED;
    const char *C_GRN;
    const char *C_YEL;
    const char *C_BLU;
    const char *C_MAG;
    const char *C_CYN;
    const char *C_WHT;
    const char *C_BBLK;
    const char *C_BRED;
    const char *C_BGRN;
    const char *C_BYEL;
    const char *C_BBLU;
    const char *C_BMAG;
    const char *C_BCYN;
    const char *C_BWHT;

    const char *FAILED_STR;
    const char *PASSED_STR;
};

enum eColoursType {
    COL_DEFAULT = 0,
    COL_ON,
    COL_OFF
};

class Colours_t {
public:
    const char *NRM() { return m_theColourVals->C_NRM; }
    const char *BLD() { return m_theColourVals->C_BLD; }
    const char *ITL() { return m_theColourVals->C_ITL; }
    const char *UND() { return m_theColourVals->C_UND; }
    const char *BLK() { return m_theColourVals->C_BLK; }
    const char *RED() { return m_theColourVals->C_RED; }
    const char *GRN() { return m_theColourVals->C_GRN; }
    const char *YEL() { return m_theColourVals->C_YEL; }
    const char *BLU() { return m_theColourVals->C_BLU; }
    const char *MAG() { return m_theColourVals->C_MAG; }
    const char *CYN() { return m_theColourVals->C_CYN; }
    const char *WHT() { return m_theColourVals->C_WHT; }
    const char *BBLK() { return m_theColourVals->C_BBLK; }
    const char *BRED() { return m_theColourVals->C_BRED; }
    const char *BGRN() { return m_theColourVals->C_BGRN; }
    const char *BYEL() { return m_theColourVals->C_BYEL; }
    const char *BBLU() { return m_theColourVals->C_BBLU; }
    const char *BMAG() { return m_theColourVals->C_BMAG; }
    const char *BCYN() { return m_theColourVals->C_BCYN; }
    const char *BWHT() { return m_theColourVals->C_BWHT; }

    const char *FAILED_STR() { return m_theColourVals->FAILED_STR; }
    const char *PASSED_STR() { return m_theColourVals->PASSED_STR; }

    const char *ColPct(double pctVal);  // asumes a valid pct value

    friend Colours_t &GetColoursObj(eColoursType colType);

private:
    Colours_t(eColoursType colType);
    ~Colours_t();

    void init_colour();
    _ColourVals_t *m_theColourVals;
};

Colours_t& GetColoursObj(eColoursType colType = COL_DEFAULT);
