/**************************************************************************
*  File: race.c                                           Part of tbaMUD *
*  Usage: Source file for race-specific code.                            *
*                                                                         *
*  All rights reserved.  See license for complete information.            *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
**************************************************************************/

/** Help buffer the global variable definitions */
#define __RACE_C__

/*
 * This file attempts to concentrate most of the code which must be changed
 * in order for new racees to be added.  If you're adding a new race, you
 * should go through this entire file from beginning to end and add the
 * appropriate new special cases for your new race.
 */
#include "conf.h"
#include "sysdep.h"
#include "structs.h"
#include "utils.h"
#include "db.h"
#include "spells.h"
#include "interpreter.h"
#include "constants.h"
#include "act.h"
#include "race.h"


// @todo: this... better... as a type
/*
 * Names first - must be in the same order as the enum
 */
const char *race_abbrevs[] =
{
    "Hu",
    "Tr",
    "Og",
    "Fa",
    "\n"
};

// Must be in the same order as the abbreviations (and enum)
const char *pc_race_types[] =
{
    "Human",
    "Trolloc",
    "Ogier",
    "Fade",
    "\n"
};

/*
 * The menu for choosing a race in interpreter.c:
 */
const char *race_menu =
    "\r\n"
    "Select a race:\r\n"
    "    [\t(T\t)]rolloc\r\n"
    "    [\t(H\t)]uman\r\n"
    "    [\t(O\t)]gier\r\n"
    "    [\t(F\t)]ade\r\n";

/*
 * The code to interpret a race letter -- used in interpreter.c when a new
 * character is selecting a race and by 'set race' in act.wizard.c.
 */
int parse_race(char arg)
{
    arg = LOWER(arg);

    switch (arg)
    {
    case 't':
        return RACE_TROLLOC;
    case 'h':
        return RACE_HUMAN;
    case 'o':
        return RACE_OGIER;
    case 'f':
        return RACE_FADE;
    default:
        return RACE_UNKNOWN;
    }
}

/*
 * bitvectors (i.e., powers of two) for each race, mainly for use in do_who
 * and do_users.  Add new racees at the end so that all racees use sequential
 * powers of two (1 << 0, 1 << 1, 1 << 2, 1 << 3, 1 << 4, 1 << 5, etc.) up to
 * the limit of your bitvector_t, typically 0-31.
 */
bitvector_t find_race_bitvector(const char *arg)
{
    size_t rpos, ret = 0;

    for (rpos = 0; rpos < strlen(arg); rpos++) {
        ret |= (1 << parse_race(arg[rpos]));
    }

    return (ret);
}

#if 0
/*
 * Saving throws for : MCTW : PARA, ROD, PETRI, BREATH, SPELL. Levels 0-40. Do
 * not forget to change extern declaration in magic.c if you add to this.
 */
byte saving_throws(int race_num, int type, int level)
{
    switch (race_num)
    {
    case RACE_TROLLOC:
        switch (type)
        {
        case SAVING_PARA:    // Paralyzation
            switch (level)
            {
            case  0: return 90;
            case  1: return 70;
            case  2: return 69;
            case  3: return 68;
            case  4: return 67;
            case  5: return 66;
            case  6: return 65;
            case  7: return 63;
            case  8: return 61;
            case  9: return 60;
            case 10: return 59;
            case 11: return 57;
            case 12: return 55;
            case 13: return 54;
            case 14: return 53;
            case 15: return 53;
            case 16: return 52;
            case 17: return 51;
            case 18: return 50;
            case 19: return 48;
            case 20: return 46;
            case 21: return 45;
            case 22: return 44;
            case 23: return 42;
            case 24: return 40;
            case 25: return 38;
            case 26: return 36;
            case 27: return 34;
            case 28: return 32;
            case 29: return 30;
            case 30: return 28;
            case 31: return  0;
            case 32: return  0;
            case 33: return  0;
            case 34: return  0;
            case 35: return  0;
            case 36: return  0;
            case 37: return  0;
            case 38: return  0;
            case 39: return  0;
            case 40: return  0;
            default:
                WriteLogf("SYSERR: Missing level for mage paralyzation saving throw.");
                break;
            }
        case SAVING_ROD:    // Rods
            switch (level)
            {
            case  0: return 90;
            case  1: return 55;
            case  2: return 53;
            case  3: return 51;
            case  4: return 49;
            case  5: return 47;
            case  6: return 45;
            case  7: return 43;
            case  8: return 41;
            case  9: return 40;
            case 10: return 39;
            case 11: return 37;
            case 12: return 35;
            case 13: return 33;
            case 14: return 31;
            case 15: return 30;
            case 16: return 29;
            case 17: return 27;
            case 18: return 25;
            case 19: return 23;
            case 20: return 21;
            case 21: return 20;
            case 22: return 19;
            case 23: return 17;
            case 24: return 15;
            case 25: return 14;
            case 26: return 13;
            case 27: return 12;
            case 28: return 11;
            case 29: return 10;
            case 30: return  9;
            case 31: return  0;
            case 32: return  0;
            case 33: return  0;
            case 34: return  0;
            case 35: return  0;
            case 36: return  0;
            case 37: return  0;
            case 38: return  0;
            case 39: return  0;
            case 40: return  0;
            default:
                WriteLogf("SYSERR: Missing level for mage rod saving throw.");
                break;
            }
        case SAVING_PETRI:    // Petrification
            switch (level)
            {
            case  0: return 90;
            case  1: return 65;
            case  2: return 63;
            case  3: return 61;
            case  4: return 59;
            case  5: return 57;
            case  6: return 55;
            case  7: return 53;
            case  8: return 51;
            case  9: return 50;
            case 10: return 49;
            case 11: return 47;
            case 12: return 45;
            case 13: return 43;
            case 14: return 41;
            case 15: return 40;
            case 16: return 39;
            case 17: return 37;
            case 18: return 35;
            case 19: return 33;
            case 20: return 31;
            case 21: return 30;
            case 22: return 29;
            case 23: return 27;
            case 24: return 25;
            case 25: return 23;
            case 26: return 21;
            case 27: return 19;
            case 28: return 17;
            case 29: return 15;
            case 30: return 13;
            case 31: return  0;
            case 32: return  0;
            case 33: return  0;
            case 34: return  0;
            case 35: return  0;
            case 36: return  0;
            case 37: return  0;
            case 38: return  0;
            case 39: return  0;
            case 40: return  0;
            default:
                WriteLogf("SYSERR: Missing level for mage petrification saving throw.");
                break;
            }
        case SAVING_BREATH:    // Breath weapons
            switch (level)
            {
            case  0: return 90;
            case  1: return 75;
            case  2: return 73;
            case  3: return 71;
            case  4: return 69;
            case  5: return 67;
            case  6: return 65;
            case  7: return 63;
            case  8: return 61;
            case  9: return 60;
            case 10: return 59;
            case 11: return 57;
            case 12: return 55;
            case 13: return 53;
            case 14: return 51;
            case 15: return 50;
            case 16: return 49;
            case 17: return 47;
            case 18: return 45;
            case 19: return 43;
            case 20: return 41;
            case 21: return 40;
            case 22: return 39;
            case 23: return 37;
            case 24: return 35;
            case 25: return 33;
            case 26: return 31;
            case 27: return 29;
            case 28: return 27;
            case 29: return 25;
            case 30: return 23;
            case 31: return  0;
            case 32: return  0;
            case 33: return  0;
            case 34: return  0;
            case 35: return  0;
            case 36: return  0;
            case 37: return  0;
            case 38: return  0;
            case 39: return  0;
            case 40: return  0;
            default:
                WriteLogf("SYSERR: Missing level for mage breath saving throw.");
                break;
            }
        case SAVING_SPELL:    // Generic spells
            switch (level)
            {
            case  0: return 90;
            case  1: return 60;
            case  2: return 58;
            case  3: return 56;
            case  4: return 54;
            case  5: return 52;
            case  6: return 50;
            case  7: return 48;
            case  8: return 46;
            case  9: return 45;
            case 10: return 44;
            case 11: return 42;
            case 12: return 40;
            case 13: return 38;
            case 14: return 36;
            case 15: return 35;
            case 16: return 34;
            case 17: return 32;
            case 18: return 30;
            case 19: return 28;
            case 20: return 26;
            case 21: return 25;
            case 22: return 24;
            case 23: return 22;
            case 24: return 20;
            case 25: return 18;
            case 26: return 16;
            case 27: return 14;
            case 28: return 12;
            case 29: return 10;
            case 30: return  8;
            case 31: return  0;
            case 32: return  0;
            case 33: return  0;
            case 34: return  0;
            case 35: return  0;
            case 36: return  0;
            case 37: return  0;
            case 38: return  0;
            case 39: return  0;
            case 40: return  0;
            default:
                WriteLogf("SYSERR: Missing level for mage spell saving throw.");
                break;
            }
        default:
            WriteLogf("SYSERR: Invalid saving throw type.");
            break;
        }
        break;
    case RACE_HUMAN:
        switch (type)
        {
        case SAVING_PARA:    // Paralyzation
            switch (level)
            {
            case  0: return 90;
            case  1: return 60;
            case  2: return 59;
            case  3: return 48;
            case  4: return 46;
            case  5: return 45;
            case  6: return 43;
            case  7: return 40;
            case  8: return 37;
            case  9: return 35;
            case 10: return 34;
            case 11: return 33;
            case 12: return 31;
            case 13: return 30;
            case 14: return 29;
            case 15: return 27;
            case 16: return 26;
            case 17: return 25;
            case 18: return 24;
            case 19: return 23;
            case 20: return 22;
            case 21: return 21;
            case 22: return 20;
            case 23: return 18;
            case 24: return 15;
            case 25: return 14;
            case 26: return 12;
            case 27: return 10;
            case 28: return  9;
            case 29: return  8;
            case 30: return  7;
            case 31: return  0;
            case 32: return  0;
            case 33: return  0;
            case 34: return  0;
            case 35: return  0;
            case 36: return  0;
            case 37: return  0;
            case 38: return  0;
            case 39: return  0;
            case 40: return  0;
            default:
                WriteLogf("SYSERR: Missing level for cleric paralyzation saving throw.");
                break;
            }
        case SAVING_ROD:    // Rods
            switch (level)
            {
            case  0: return 90;
            case  1: return 70;
            case  2: return 69;
            case  3: return 68;
            case  4: return 66;
            case  5: return 65;
            case  6: return 63;
            case  7: return 60;
            case  8: return 57;
            case  9: return 55;
            case 10: return 54;
            case 11: return 53;
            case 12: return 51;
            case 13: return 50;
            case 14: return 49;
            case 15: return 47;
            case 16: return 46;
            case 17: return 45;
            case 18: return 44;
            case 19: return 43;
            case 20: return 42;
            case 21: return 41;
            case 22: return 40;
            case 23: return 38;
            case 24: return 35;
            case 25: return 34;
            case 26: return 32;
            case 27: return 30;
            case 28: return 29;
            case 29: return 28;
            case 30: return 27;
            case 31: return  0;
            case 32: return  0;
            case 33: return  0;
            case 34: return  0;
            case 35: return  0;
            case 36: return  0;
            case 37: return  0;
            case 38: return  0;
            case 39: return  0;
            case 40: return  0;
            default:
                WriteLogf("SYSERR: Missing level for cleric rod saving throw.");
                break;
            }
        case SAVING_PETRI:    // Petrification
            switch (level)
            {
            case  0: return 90;
            case  1: return 65;
            case  2: return 64;
            case  3: return 63;
            case  4: return 61;
            case  5: return 60;
            case  6: return 58;
            case  7: return 55;
            case  8: return 53;
            case  9: return 50;
            case 10: return 49;
            case 11: return 48;
            case 12: return 46;
            case 13: return 45;
            case 14: return 44;
            case 15: return 43;
            case 16: return 41;
            case 17: return 40;
            case 18: return 39;
            case 19: return 38;
            case 20: return 37;
            case 21: return 36;
            case 22: return 35;
            case 23: return 33;
            case 24: return 31;
            case 25: return 29;
            case 26: return 27;
            case 27: return 25;
            case 28: return 24;
            case 29: return 23;
            case 30: return 22;
            case 31: return  0;
            case 32: return  0;
            case 33: return  0;
            case 34: return  0;
            case 35: return  0;
            case 36: return  0;
            case 37: return  0;
            case 38: return  0;
            case 39: return  0;
            case 40: return  0;
            default:
                WriteLogf("SYSERR: Missing level for cleric petrification saving throw.");
                break;
            }
        case SAVING_BREATH:    // Breath weapons
            switch (level)
            {
            case  0: return 90;
            case  1: return 80;
            case  2: return 79;
            case  3: return 78;
            case  4: return 76;
            case  5: return 75;
            case  6: return 73;
            case  7: return 70;
            case  8: return 67;
            case  9: return 65;
            case 10: return 64;
            case 11: return 63;
            case 12: return 61;
            case 13: return 60;
            case 14: return 59;
            case 15: return 57;
            case 16: return 56;
            case 17: return 55;
            case 18: return 54;
            case 19: return 53;
            case 20: return 52;
            case 21: return 51;
            case 22: return 50;
            case 23: return 48;
            case 24: return 45;
            case 25: return 44;
            case 26: return 42;
            case 27: return 40;
            case 28: return 39;
            case 29: return 38;
            case 30: return 37;
            case 31: return  0;
            case 32: return  0;
            case 33: return  0;
            case 34: return  0;
            case 35: return  0;
            case 36: return  0;
            case 37: return  0;
            case 38: return  0;
            case 39: return  0;
            case 40: return  0;
            default:
                WriteLogf("SYSERR: Missing level for cleric breath saving throw.");
                break;
            }
        case SAVING_SPELL:    // Generic spells
            switch (level)
            {
            case  0: return 90;
            case  1: return 75;
            case  2: return 74;
            case  3: return 73;
            case  4: return 71;
            case  5: return 70;
            case  6: return 68;
            case  7: return 65;
            case  8: return 63;
            case  9: return 60;
            case 10: return 59;
            case 11: return 58;
            case 12: return 56;
            case 13: return 55;
            case 14: return 54;
            case 15: return 53;
            case 16: return 51;
            case 17: return 50;
            case 18: return 49;
            case 19: return 48;
            case 20: return 47;
            case 21: return 46;
            case 22: return 45;
            case 23: return 43;
            case 24: return 41;
            case 25: return 39;
            case 26: return 37;
            case 27: return 35;
            case 28: return 34;
            case 29: return 33;
            case 30: return 32;
            case 31: return  0;
            case 32: return  0;
            case 33: return  0;
            case 34: return  0;
            case 35: return  0;
            case 36: return  0;
            case 37: return  0;
            case 38: return  0;
            case 39: return  0;
            case 40: return  0;
            default:
                WriteLogf("SYSERR: Missing level for cleric spell saving throw.");
                break;
            }
        default:
            WriteLogf("SYSERR: Invalid saving throw type.");
            break;
        }
        break;
    case RACE_OGIER:
        switch (type)
        {
        case SAVING_PARA:    // Paralyzation
            switch (level)
            {
            case  0: return 90;
            case  1: return 65;
            case  2: return 64;
            case  3: return 63;
            case  4: return 62;
            case  5: return 61;
            case  6: return 60;
            case  7: return 59;
            case  8: return 58;
            case  9: return 57;
            case 10: return 56;
            case 11: return 55;
            case 12: return 54;
            case 13: return 53;
            case 14: return 52;
            case 15: return 51;
            case 16: return 50;
            case 17: return 49;
            case 18: return 48;
            case 19: return 47;
            case 20: return 46;
            case 21: return 45;
            case 22: return 44;
            case 23: return 43;
            case 24: return 42;
            case 25: return 41;
            case 26: return 40;
            case 27: return 39;
            case 28: return 38;
            case 29: return 37;
            case 30: return 36;
            case 31: return  0;
            case 32: return  0;
            case 33: return  0;
            case 34: return  0;
            case 35: return  0;
            case 36: return  0;
            case 37: return  0;
            case 38: return  0;
            case 39: return  0;
            case 40: return  0;
            default:
                WriteLogf("SYSERR: Missing level for thief paralyzation saving throw.");
                break;
            }
        case SAVING_ROD:    // Rods
            switch (level)
            {
            case  0: return 90;
            case  1: return 70;
            case  2: return 68;
            case  3: return 66;
            case  4: return 64;
            case  5: return 62;
            case  6: return 60;
            case  7: return 58;
            case  8: return 56;
            case  9: return 54;
            case 10: return 52;
            case 11: return 50;
            case 12: return 48;
            case 13: return 46;
            case 14: return 44;
            case 15: return 42;
            case 16: return 40;
            case 17: return 38;
            case 18: return 36;
            case 19: return 34;
            case 20: return 32;
            case 21: return 30;
            case 22: return 28;
            case 23: return 26;
            case 24: return 24;
            case 25: return 22;
            case 26: return 20;
            case 27: return 18;
            case 28: return 16;
            case 29: return 14;
            case 30: return 13;
            case 31: return  0;
            case 32: return  0;
            case 33: return  0;
            case 34: return  0;
            case 35: return  0;
            case 36: return  0;
            case 37: return  0;
            case 38: return  0;
            case 39: return  0;
            case 40: return  0;
            default:
                WriteLogf("SYSERR: Missing level for thief rod saving throw.");
                break;
            }
        case SAVING_PETRI:    // Petrification
            switch (level)
            {
            case  0: return 90;
            case  1: return 60;
            case  2: return 59;
            case  3: return 58;
            case  4: return 58;
            case  5: return 56;
            case  6: return 55;
            case  7: return 54;
            case  8: return 53;
            case  9: return 52;
            case 10: return 51;
            case 11: return 50;
            case 12: return 49;
            case 13: return 48;
            case 14: return 47;
            case 15: return 46;
            case 16: return 45;
            case 17: return 44;
            case 18: return 43;
            case 19: return 42;
            case 20: return 41;
            case 21: return 40;
            case 22: return 39;
            case 23: return 38;
            case 24: return 37;
            case 25: return 36;
            case 26: return 35;
            case 27: return 34;
            case 28: return 33;
            case 29: return 32;
            case 30: return 31;
            case 31: return  0;
            case 32: return  0;
            case 33: return  0;
            case 34: return  0;
            case 35: return  0;
            case 36: return  0;
            case 37: return  0;
            case 38: return  0;
            case 39: return  0;
            case 40: return  0;
            default:
                WriteLogf("SYSERR: Missing level for thief petrification saving throw.");
                break;
            }
        case SAVING_BREATH:    // Breath weapons
            switch (level)
            {
            case  0: return 90;
            case  1: return 80;
            case  2: return 79;
            case  3: return 78;
            case  4: return 77;
            case  5: return 76;
            case  6: return 75;
            case  7: return 74;
            case  8: return 73;
            case  9: return 72;
            case 10: return 71;
            case 11: return 70;
            case 12: return 69;
            case 13: return 68;
            case 14: return 67;
            case 15: return 66;
            case 16: return 65;
            case 17: return 64;
            case 18: return 63;
            case 19: return 62;
            case 20: return 61;
            case 21: return 60;
            case 22: return 59;
            case 23: return 58;
            case 24: return 57;
            case 25: return 56;
            case 26: return 55;
            case 27: return 54;
            case 28: return 53;
            case 29: return 52;
            case 30: return 51;
            case 31: return  0;
            case 32: return  0;
            case 33: return  0;
            case 34: return  0;
            case 35: return  0;
            case 36: return  0;
            case 37: return  0;
            case 38: return  0;
            case 39: return  0;
            case 40: return  0;
            default:
                WriteLogf("SYSERR: Missing level for thief breath saving throw.");
                break;
            }
        case SAVING_SPELL:    // Generic spells
            switch (level)
            {
            case  0: return 90;
            case  1: return 75;
            case  2: return 73;
            case  3: return 71;
            case  4: return 69;
            case  5: return 67;
            case  6: return 65;
            case  7: return 63;
            case  8: return 61;
            case  9: return 59;
            case 10: return 57;
            case 11: return 55;
            case 12: return 53;
            case 13: return 51;
            case 14: return 49;
            case 15: return 47;
            case 16: return 45;
            case 17: return 43;
            case 18: return 41;
            case 19: return 39;
            case 20: return 37;
            case 21: return 35;
            case 22: return 33;
            case 23: return 31;
            case 24: return 29;
            case 25: return 27;
            case 26: return 25;
            case 27: return 23;
            case 28: return 21;
            case 29: return 19;
            case 30: return 17;
            case 31: return  0;
            case 32: return  0;
            case 33: return  0;
            case 34: return  0;
            case 35: return  0;
            case 36: return  0;
            case 37: return  0;
            case 38: return  0;
            case 39: return  0;
            case 40: return  0;
            default:
                WriteLogf("SYSERR: Missing level for thief spell saving throw.");
                break;
            }
        default:
            WriteLogf("SYSERR: Invalid saving throw type.");
            break;
        }
        break;
    case RACE_FADE:
        switch (type)
        {
        case SAVING_PARA:    // Paralyzation
            switch (level)
            {
            case  0: return 90;
            case  1: return 70;
            case  2: return 68;
            case  3: return 67;
            case  4: return 65;
            case  5: return 62;
            case  6: return 58;
            case  7: return 55;
            case  8: return 53;
            case  9: return 52;
            case 10: return 50;
            case 11: return 47;
            case 12: return 43;
            case 13: return 40;
            case 14: return 38;
            case 15: return 37;
            case 16: return 35;
            case 17: return 32;
            case 18: return 28;
            case 19: return 25;
            case 20: return 24;
            case 21: return 23;
            case 22: return 22;
            case 23: return 20;
            case 24: return 19;
            case 25: return 17;
            case 26: return 16;
            case 27: return 15;
            case 28: return 14;
            case 29: return 13;
            case 30: return 12;
            case 31: return 11;
            case 32: return 10;
            case 33: return  9;
            case 34: return  8;
            case 35: return  7;
            case 36: return  6;
            case 37: return  5;
            case 38: return  4;
            case 39: return  3;
            case 40: return  2;
            case 41: return  1;    // Some mobiles.
            case 42: return  0;
            case 43: return  0;
            case 44: return  0;
            case 45: return  0;
            case 46: return  0;
            case 47: return  0;
            case 48: return  0;
            case 49: return  0;
            case 50: return  0;
            default:
                WriteLogf("SYSERR: Missing level for warrior paralyzation saving throw.");
                break;
            }
        case SAVING_ROD:    // Rods
            switch (level)
            {
            case  0: return 90;
            case  1: return 80;
            case  2: return 78;
            case  3: return 77;
            case  4: return 75;
            case  5: return 72;
            case  6: return 68;
            case  7: return 65;
            case  8: return 63;
            case  9: return 62;
            case 10: return 60;
            case 11: return 57;
            case 12: return 53;
            case 13: return 50;
            case 14: return 48;
            case 15: return 47;
            case 16: return 45;
            case 17: return 42;
            case 18: return 38;
            case 19: return 35;
            case 20: return 34;
            case 21: return 33;
            case 22: return 32;
            case 23: return 30;
            case 24: return 29;
            case 25: return 27;
            case 26: return 26;
            case 27: return 25;
            case 28: return 24;
            case 29: return 23;
            case 30: return 22;
            case 31: return 20;
            case 32: return 18;
            case 33: return 16;
            case 34: return 14;
            case 35: return 12;
            case 36: return 10;
            case 37: return  8;
            case 38: return  6;
            case 39: return  5;
            case 40: return  4;
            case 41: return  3;
            case 42: return  2;
            case 43: return  1;
            case 44: return  0;
            case 45: return  0;
            case 46: return  0;
            case 47: return  0;
            case 48: return  0;
            case 49: return  0;
            case 50: return  0;
            default:
                WriteLogf("SYSERR: Missing level for warrior rod saving throw.");
                break;
            }
        case SAVING_PETRI:    // Petrification
            switch (level)
            {
            case  0: return 90;
            case  1: return 75;
            case  2: return 73;
            case  3: return 72;
            case  4: return 70;
            case  5: return 67;
            case  6: return 63;
            case  7: return 60;
            case  8: return 58;
            case  9: return 57;
            case 10: return 55;
            case 11: return 52;
            case 12: return 48;
            case 13: return 45;
            case 14: return 43;
            case 15: return 42;
            case 16: return 40;
            case 17: return 37;
            case 18: return 33;
            case 19: return 30;
            case 20: return 29;
            case 21: return 28;
            case 22: return 26;
            case 23: return 25;
            case 24: return 24;
            case 25: return 23;
            case 26: return 21;
            case 27: return 20;
            case 28: return 19;
            case 29: return 18;
            case 30: return 17;
            case 31: return 16;
            case 32: return 15;
            case 33: return 14;
            case 34: return 13;
            case 35: return 12;
            case 36: return 11;
            case 37: return 10;
            case 38: return  9;
            case 39: return  8;
            case 40: return  7;
            case 41: return  6;
            case 42: return  5;
            case 43: return  4;
            case 44: return  3;
            case 45: return  2;
            case 46: return  1;
            case 47: return  0;
            case 48: return  0;
            case 49: return  0;
            case 50: return  0;
            default:
                WriteLogf("SYSERR: Missing level for warrior petrification saving throw.");
                break;
            }
        case SAVING_BREATH:    // Breath weapons
            switch (level)
            {
            case  0: return 90;
            case  1: return 85;
            case  2: return 83;
            case  3: return 82;
            case  4: return 80;
            case  5: return 75;
            case  6: return 70;
            case  7: return 65;
            case  8: return 63;
            case  9: return 62;
            case 10: return 60;
            case 11: return 55;
            case 12: return 50;
            case 13: return 45;
            case 14: return 43;
            case 15: return 42;
            case 16: return 40;
            case 17: return 37;
            case 18: return 33;
            case 19: return 30;
            case 20: return 29;
            case 21: return 28;
            case 22: return 26;
            case 23: return 25;
            case 24: return 24;
            case 25: return 23;
            case 26: return 21;
            case 27: return 20;
            case 28: return 19;
            case 29: return 18;
            case 30: return 17;
            case 31: return 16;
            case 32: return 15;
            case 33: return 14;
            case 34: return 13;
            case 35: return 12;
            case 36: return 11;
            case 37: return 10;
            case 38: return  9;
            case 39: return  8;
            case 40: return  7;
            case 41: return  6;
            case 42: return  5;
            case 43: return  4;
            case 44: return  3;
            case 45: return  2;
            case 46: return  1;
            case 47: return  0;
            case 48: return  0;
            case 49: return  0;
            case 50: return  0;
            default:
                WriteLogf("SYSERR: Missing level for warrior breath saving throw.");
                break;
            }
        case SAVING_SPELL:    // Generic spells
            switch (level)
            {
            case  0: return 90;
            case  1: return 85;
            case  2: return 83;
            case  3: return 82;
            case  4: return 80;
            case  5: return 77;
            case  6: return 73;
            case  7: return 70;
            case  8: return 68;
            case  9: return 67;
            case 10: return 65;
            case 11: return 62;
            case 12: return 58;
            case 13: return 55;
            case 14: return 53;
            case 15: return 52;
            case 16: return 50;
            case 17: return 47;
            case 18: return 43;
            case 19: return 40;
            case 20: return 39;
            case 21: return 38;
            case 22: return 36;
            case 23: return 35;
            case 24: return 34;
            case 25: return 33;
            case 26: return 31;
            case 27: return 30;
            case 28: return 29;
            case 29: return 28;
            case 30: return 27;
            case 31: return 25;
            case 32: return 23;
            case 33: return 21;
            case 34: return 19;
            case 35: return 17;
            case 36: return 15;
            case 37: return 13;
            case 38: return 11;
            case 39: return  9;
            case 40: return  7;
            case 41: return  6;
            case 42: return  5;
            case 43: return  4;
            case 44: return  3;
            case 45: return  2;
            case 46: return  1;
            case 47: return  0;
            case 48: return  0;
            case 49: return  0;
            case 50: return  0;
            default:
                WriteLogf("SYSERR: Missing level for warrior spell saving throw.");
                break;
            }
        default:
            WriteLogf("SYSERR: Invalid saving throw type.");
            break;
        }
    default:
        WriteLogf("SYSERR: Invalid race saving throw.");
        break;
    }

    // Should not get here unless something is wrong.
    return 100;
}

/*
 * THAC0 for racees and levels.  (To Hit Armor Class 0)
 */
int thaco(int race_num, int level)
{
    switch (race_num)
    {
    case RACE_TROLLOC:
        switch (level)
        {
        case  0: return 100;
        case  1: return  20;
        case  2: return  20;
        case  3: return  20;
        case  4: return  19;
        case  5: return  19;
        case  6: return  19;
        case  7: return  18;
        case  8: return  18;
        case  9: return  18;
        case 10: return  17;
        case 11: return  17;
        case 12: return  17;
        case 13: return  16;
        case 14: return  16;
        case 15: return  16;
        case 16: return  15;
        case 17: return  15;
        case 18: return  15;
        case 19: return  14;
        case 20: return  14;
        case 21: return  14;
        case 22: return  13;
        case 23: return  13;
        case 24: return  13;
        case 25: return  12;
        case 26: return  12;
        case 27: return  12;
        case 28: return  11;
        case 29: return  11;
        case 30: return  11;
        case 31: return  10;
        case 32: return  10;
        case 33: return  10;
        case 34: return   9;
        default:
            WriteLogf("SYSERR: Missing level for mage thac0.");
        }
    case RACE_HUMAN:
        switch (level)
        {
        case  0: return 100;
        case  1: return  20;
        case  2: return  20;
        case  3: return  20;
        case  4: return  18;
        case  5: return  18;
        case  6: return  18;
        case  7: return  16;
        case  8: return  16;
        case  9: return  16;
        case 10: return  14;
        case 11: return  14;
        case 12: return  14;
        case 13: return  12;
        case 14: return  12;
        case 15: return  12;
        case 16: return  10;
        case 17: return  10;
        case 18: return  10;
        case 19: return   8;
        case 20: return   8;
        case 21: return   8;
        case 22: return   6;
        case 23: return   6;
        case 24: return   6;
        case 25: return   4;
        case 26: return   4;
        case 27: return   4;
        case 28: return   2;
        case 29: return   2;
        case 30: return   2;
        case 31: return   1;
        case 32: return   1;
        case 33: return   1;
        case 34: return   1;
        default:
            WriteLogf("SYSERR: Missing level for cleric thac0.");
        }
    case RACE_OGIER:
        switch (level)
        {
        case  0: return 100;
        case  1: return  20;
        case  2: return  20;
        case  3: return  19;
        case  4: return  19;
        case  5: return  18;
        case  6: return  18;
        case  7: return  17;
        case  8: return  17;
        case  9: return  16;
        case 10: return  16;
        case 11: return  15;
        case 12: return  15;
        case 13: return  14;
        case 14: return  14;
        case 15: return  13;
        case 16: return  13;
        case 17: return  12;
        case 18: return  12;
        case 19: return  11;
        case 20: return  11;
        case 21: return  10;
        case 22: return  10;
        case 23: return   9;
        case 24: return   9;
        case 25: return   8;
        case 26: return   8;
        case 27: return   7;
        case 28: return   7;
        case 29: return   6;
        case 30: return   6;
        case 31: return   5;
        case 32: return   5;
        case 33: return   4;
        case 34: return   4;
        default:
            WriteLogf("SYSERR: Missing level for thief thac0.");
        }
    case RACE_FADE:
        switch (level)
        {
        case  0: return 100;
        case  1: return  20;
        case  2: return  19;
        case  3: return  18;
        case  4: return  17;
        case  5: return  16;
        case  6: return  15;
        case  7: return  14;
        case  8: return  14;
        case  9: return  13;
        case 10: return  12;
        case 11: return  11;
        case 12: return  10;
        case 13: return   9;
        case 14: return   8;
        case 15: return   7;
        case 16: return   6;
        case 17: return   5;
        case 18: return   4;
        case 19: return   3;
        case 20: return   2;
        case 21: return   1;
        case 22: return   1;
        case 23: return   1;
        case 24: return   1;
        case 25: return   1;
        case 26: return   1;
        case 27: return   1;
        case 28: return   1;
        case 29: return   1;
        case 30: return   1;
        case 31: return   1;
        case 32: return   1;
        case 33: return   1;
        case 34: return   1;
        default:
            WriteLogf("SYSERR: Missing level for warrior thac0.");
        }
    default:
        WriteLogf("SYSERR: Unknown race in thac0 chart.");
    }

    // Will not get there unless something is wrong.
    return 100;
}


/*
 * Roll the 6 stats for a character... each stat is made of the sum of the best
 * 3 out of 4 rolls of a 6-sided die.  Each race then decides which priority
 * will be given for the best to worst stats.
 */
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
        }

        temp = rolls[0] + rolls[1] + rolls[2] + rolls[3] -
            MIN(rolls[0], MIN(rolls[1], MIN(rolls[2], rolls[3])));

        for (k = 0; k < 6; k++) {
            if (table[k] < temp) {
                temp ^= table[k];
                table[k] ^= temp;
                temp ^= table[k];
            }
        }
    }

    ch->real_abils.setStrAdd(0);

    switch (GET_RACE(ch))
    {
    case RACE_TROLLOC:
        ch->real_abils.setIntel(table[0]);
        ch->real_abils.setWis(table[1]);
        ch->real_abils.setDex(table[2]);
        ch->real_abils.setStr(table[3]);
        ch->real_abils.setCon(table[4]);
        ch->real_abils.setCha(table[5]);
        break;

    case RACE_HUMAN:
        ch->real_abils.setWis(table[0]);
        ch->real_abils.setIntel(table[1]);
        ch->real_abils.setStr(table[2]);
        ch->real_abils.setDex(table[3]);
        ch->real_abils.setCon(table[4]);
        ch->real_abils.setCha(table[5]);
        break;

    case RACE_OGIER:
        ch->real_abils.setDex(table[0]);
        ch->real_abils.setStr(table[1]);
        ch->real_abils.setCon(table[2]);
        ch->real_abils.setIntel(table[3]);
        ch->real_abils.setWis(table[4]);
        ch->real_abils.setCha(table[5]);
        break;

    case RACE_FADE:
        ch->real_abils.setStr(table[0]);
        ch->real_abils.setDex(table[1]);
        ch->real_abils.setCon(table[2]);
        ch->real_abils.setWis(table[3]);
        ch->real_abils.setIntel(table[4]);
        ch->real_abils.setCha(table[5]);
        if (ch->real_abils.str >= 18) {
            ch->real_abils.setStrAdd(rand_number(0, 100));
        }
        break;
    }
    ch->aff_abils = ch->real_abils;
}
#endif

/*
 * invalid_race is used by handler.c to determine if a piece of equipment is
 * usable by a particular race, based on the ITEM_ANTI_{race} bitvectors.
 */
int invalid_race(struct char_data *ch, struct obj_data *obj)
{
    if (OBJ_FLAGGED(obj, ITEM_ANTI_TROLLOC) && isTrolloc(ch)) {
        return TRUE;
    }

    if (OBJ_FLAGGED(obj, ITEM_ANTI_HUMAN) && isHuman(ch)) {
        return TRUE;
    }

    if (OBJ_FLAGGED(obj, ITEM_ANTI_FADE) && isFade(ch)) {
        return TRUE;
    }

    if (OBJ_FLAGGED(obj, ITEM_ANTI_OGIER) && isOgier(ch)) {
        return TRUE;
    }

    return FALSE;
}

/*
 * Safe wrapper for getting a string representation of a race
 */
const char *get_race_string(int race)
{
    if ((race <= RACE_UNKNOWN) || (race >= NUM_RACES)) {
        return "UNKNOWN";
    }
    else {
        return pc_race_types[race];
    }
}

/*
 * TODO: Replace or remove, this is just a copy of class for now.
 *
 * Default titles of male characters.
 */
const char *racetitle_male(int chrace, int level)
{
    if (level <= 0 || level > LVL_IMPL) {
        return "the Man";
    }
    if (level == LVL_IMPL) {
        return "the Implementor";
    }

    switch (chrace) {
    case RACE_TROLLOC:
        switch (level) {
        case  1: return "the Apprentice of Magic";
        case  2: return "the Spell Student";
        case  3: return "the Scholar of Magic";
        case  4: return "the Delver in Spells";
        case  5: return "the Medium of Magic";
        case  6: return "the Scribe of Magic";
        case  7: return "the Seer";
        case  8: return "the Sage";
        case  9: return "the Illusionist";
        case 10: return "the Abjurer";
        case 11: return "the Invoker";
        case 12: return "the Enchanter";
        case 13: return "the Conjurer";
        case 14: return "the Magician";
        case 15: return "the Creator";
        case 16: return "the Savant";
        case 17: return "the Magus";
        case 18: return "the Wizard";
        case 19: return "the Warlock";
        case 20: return "the Sorcerer";
        case 21: return "the Necromancer";
        case 22: return "the Thaumaturge";
        case 23: return "the Student of the Occult";
        case 24: return "the Disciple of the Uncanny";
        case 25: return "the Minor Elemental";
        case 26: return "the Greater Elemental";
        case 27: return "the Crafter of Magics";
        case 28: return "the Shaman";
        case 29: return "the Keeper of Talismans";
        case 30: return "the Archmage";
        case LVL_IMMORT: return "the Immortal Warlock";
        case LVL_GOD:    return "the Avatar of Magic";
        case LVL_GRGOD:  return "the God of Magic";
        default: return "the Mage";
        }
        break;

    case RACE_HUMAN:
        switch (level) {
        case  1: return "the Believer";
        case  2: return "the Attendant";
        case  3: return "the Acolyte";
        case  4: return "the Novice";
        case  5: return "the Missionary";
        case  6: return "the Adept";
        case  7: return "the Deacon";
        case  8: return "the Vicar";
        case  9: return "the Priest";
        case 10: return "the Minister";
        case 11: return "the Canon";
        case 12: return "the Levite";
        case 13: return "the Curate";
        case 14: return "the Monk";
        case 15: return "the Healer";
        case 16: return "the Chaplain";
        case 17: return "the Expositor";
        case 18: return "the Bishop";
        case 19: return "the Arch Bishop";
        case 20: return "the Patriarch";
            // no one ever thought up these titles 21-30
        case LVL_IMMORT: return "the Immortal Cardinal";
        case LVL_GOD:    return "the Inquisitor";
        case LVL_GRGOD:  return "the God of good and evil";
        default: return "the Cleric";
        }
        break;

    case RACE_OGIER:
        switch (level) {
        case  1: return "the Pilferer";
        case  2: return "the Footpad";
        case  3: return "the Filcher";
        case  4: return "the Pick-Pocket";
        case  5: return "the Sneak";
        case  6: return "the Pincher";
        case  7: return "the Cut-Purse";
        case  8: return "the Snatcher";
        case  9: return "the Sharper";
        case 10: return "the Rogue";
        case 11: return "the Robber";
        case 12: return "the Magsman";
        case 13: return "the Highwayman";
        case 14: return "the Burglar";
        case 15: return "the Thief";
        case 16: return "the Knifer";
        case 17: return "the Quick-Blade";
        case 18: return "the Killer";
        case 19: return "the Brigand";
        case 20: return "the Cut-Throat";
            // no one ever thought up these titles 21-30
        case LVL_IMMORT: return "the Immortal Assassin";
        case LVL_GOD:    return "the Demi God of thieves";
        case LVL_GRGOD:  return "the God of thieves and tradesmen";
        default: return "the Thief";
        }
        break;

    case RACE_FADE:
        switch (level) {
        case  1: return "the Swordpupil";
        case  2: return "the Recruit";
        case  3: return "the Sentry";
        case  4: return "the Fighter";
        case  5: return "the Soldier";
        case  6: return "the Warrior";
        case  7: return "the Veteran";
        case  8: return "the Swordsman";
        case  9: return "the Fencer";
        case 10: return "the Combatant";
        case 11: return "the Hero";
        case 12: return "the Myrmidon";
        case 13: return "the Swashbuckler";
        case 14: return "the Mercenary";
        case 15: return "the Swordmaster";
        case 16: return "the Lieutenant";
        case 17: return "the Champion";
        case 18: return "the Dragoon";
        case 19: return "the Cavalier";
        case 20: return "the Knight";
            // no one ever thought up these titles 21-30
        case LVL_IMMORT: return "the Immortal Warlord";
        case LVL_GOD:    return "the Extirpator";
        case LVL_GRGOD:  return "the God of war";
        default: return "the Warrior";
        }
        break;
    }

    // Default title for racees which do not have titles defined
    return "the Raceless";
}

/*
 * TODO: Replace or remove, this is just a copy of class for now.
 *
 * Default titles of female characters.
 */
const char *racetitle_female(int chrace, int level)
{
    if (level <= 0 || level > LVL_IMPL) {
        return "the Woman";
    }
    if (level == LVL_IMPL) {
        return "the Implementress";
    }

    switch (chrace)
    {
    case RACE_TROLLOC:
        switch (level)
        {
        case  1: return "the Apprentice of Magic";
        case  2: return "the Spell Student";
        case  3: return "the Scholar of Magic";
        case  4: return "the Delveress in Spells";
        case  5: return "the Medium of Magic";
        case  6: return "the Scribess of Magic";
        case  7: return "the Seeress";
        case  8: return "the Sage";
        case  9: return "the Illusionist";
        case 10: return "the Abjuress";
        case 11: return "the Invoker";
        case 12: return "the Enchantress";
        case 13: return "the Conjuress";
        case 14: return "the Witch";
        case 15: return "the Creator";
        case 16: return "the Savant";
        case 17: return "the Craftess";
        case 18: return "the Wizard";
        case 19: return "the War Witch";
        case 20: return "the Sorceress";
        case 21: return "the Necromancress";
        case 22: return "the Thaumaturgess";
        case 23: return "the Student of the Occult";
        case 24: return "the Disciple of the Uncanny";
        case 25: return "the Minor Elementress";
        case 26: return "the Greater Elementress";
        case 27: return "the Crafter of Magics";
        case 28: return "Shaman";
        case 29: return "the Keeper of Talismans";
        case 30: return "Archwitch";

        case LVL_IMMORT: return "the Immortal Enchantress";
        case LVL_GOD:    return "the Empress of Magic";
        case LVL_GRGOD:  return "the Goddess of Magic";
        default: return "the Witch";
        }
        break;

    case RACE_HUMAN:
        switch (level)
        {
        case  1: return "the Believer";
        case  2: return "the Attendant";
        case  3: return "the Acolyte";
        case  4: return "the Novice";
        case  5: return "the Missionary";
        case  6: return "the Adept";
        case  7: return "the Deaconess";
        case  8: return "the Vicaress";
        case  9: return "the Priestess";
        case 10: return "the Lady Minister";
        case 11: return "the Canon";
        case 12: return "the Levitess";
        case 13: return "the Curess";
        case 14: return "the Nunne";
        case 15: return "the Healess";
        case 16: return "the Chaplain";
        case 17: return "the Expositress";
        case 18: return "the Bishop";
        case 19: return "the Arch Lady of the Church";
        case 20: return "the Matriarch";
            // no one ever thought up these titles 21-30
        case LVL_IMMORT: return "the Immortal Priestess";
        case LVL_GOD:    return "the Inquisitress";
        case LVL_GRGOD:  return "the Goddess of good and evil";
        default: return "the Cleric";
        }
        break;

    case RACE_OGIER:
        switch (level)
        {
        case  1: return "the Pilferess";
        case  2: return "the Footpad";
        case  3: return "the Filcheress";
        case  4: return "the Pick-Pocket";
        case  5: return "the Sneak";
        case  6: return "the Pincheress";
        case  7: return "the Cut-Purse";
        case  8: return "the Snatcheress";
        case  9: return "the Sharpress";
        case 10: return "the Rogue";
        case 11: return "the Robber";
        case 12: return "the Magswoman";
        case 13: return "the Highwaywoman";
        case 14: return "the Burglaress";
        case 15: return "the Thief";
        case 16: return "the Knifer";
        case 17: return "the Quick-Blade";
        case 18: return "the Murderess";
        case 19: return "the Brigand";
        case 20: return "the Cut-Throat";
            // no one ever thought up these titles 21-30
        case LVL_IMMORT: return "the Immortal Assassin";
        case LVL_GOD: return "the Demi Goddess of thieves";
        case LVL_GRGOD: return "the Goddess of thieves and tradesmen";
        default: return "the Thief";
        }
        break;

    case RACE_FADE:
        switch (level)
        {
        case  1: return "the Swordpupil";
        case  2: return "the Recruit";
        case  3: return "the Sentress";
        case  4: return "the Fighter";
        case  5: return "the Soldier";
        case  6: return "the Warrior";
        case  7: return "the Veteran";
        case  8: return "the Swordswoman";
        case  9: return "the Fenceress";
        case 10: return "the Combatess";
        case 11: return "the Heroine";
        case 12: return "the Myrmidon";
        case 13: return "the Swashbuckleress";
        case 14: return "the Mercenaress";
        case 15: return "the Swordmistress";
        case 16: return "the Lieutenant";
        case 17: return "the Lady Champion";
        case 18: return "the Lady Dragoon";
        case 19: return "the Cavalier";
        case 20: return "the Lady Knight";
            // no one ever thought up these titles 21-30
        case LVL_IMMORT: return "the Immortal Lady of War";
        case LVL_GOD: return "the Queen of Destruction";
        case LVL_GRGOD: return "the Goddess of war";
        default: return "the Warrior";
        }
        break;
    }

    // Default title for racees which do not have titles defined
    return "the Raceless";
}

/*
 * Adjust stats to suit the race of the charater.
 */
void adjust_abils_for_race(struct char_data *ch) {
    // basic version.  stat generation eeds a redo anyway
    int irand = 0;

    /*
    CLASSB_ANYPC,                       //<! RACE_HUMAN,
    CLASSB_WARRIOR | CLASSB_THIEF,      //<! RACE_TROLLOC,
    CLASSB_CLERIC | CLASSB_WARRIOR,     //<! RACE_OGIER,
    CLASSB_MAGIC_USER | CLASSB_THIEF    //<! RACE_FADE,
    */
    switch (ch->player.chrace) {
    case RACE_HUMAN:
        mudlog(CMP, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), TRUE, "Adjust stats for human player '%s'", GET_NAME(ch));
        // nope, humans are middle of the road
        break;
    case RACE_TROLLOC:
        mudlog(CMP, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), TRUE, "Adjust stats for trolloc player '%s'", GET_NAME(ch));
        // trollocs are big, strong or fast and dumb
        // allow for class choice?
        switch (ch->player.chclass) {
        case CLASS_WARRIOR:
            // str and con up.
            // stronger or tougher?
            irand = rand_number(1, 3);

            // if we have a 1, clearly stronger
            // if we have a 2 and are already stronger, enhance it
            if ((irand == 1) || ((irand == 2) && (GET_STR(ch)>GET_CON(ch)))) {
                // stronger
                // str up by 2-3
                ch->real_abils.setStr(LIMIT(ch->real_abils.getStr()+rand_number(2,3), 7, 21));
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
            WriteLogf("SYSERR: adjust_abils_for_race: Trolloc character '%s' has invalid class [%u]", GET_NAME(ch), ch->player.chclass);
        }
        //ch->real_abils.setStrAdd(LIMIT(ch->real_abils.getStrAdd()+rand_number(2,3), 7, 21));  // todo - adjust str add

        // regardless, wis down quite a lot - they cannot resist anything
        ch->real_abils.setWis(LIMIT(ch->real_abils.getWis() - rand_number(3, 5), 1, 12));  // if they are lucky they MIGHT reach average human
        // regardless, int down quite a bit
        ch->real_abils.setIntel(LIMIT(ch->real_abils.getIntel() - rand_number(2, 3), 3, 14));  // if they are lucky they MIGHT reach average human
        // regardless, cha down a bit
        ch->real_abils.setCha(LIMIT(ch->real_abils.getWis() - rand_number(2, 4), 1, 13));  // if they are lucky they MIGHT reach average human
        break;

    case RACE_OGIER:
        // Ogier are strong and intelligent, but not quick
        // allow for class choice?
        switch (ch->player.chclass) {
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
            WriteLogf("SYSERR: adjust_abils_for_race: Ogier character '%s' has invalid class [%u]", GET_NAME(ch), ch->player.chclass);
        }
        //ch->real_abils.setStrAdd(LIMIT(ch->real_abils.getStrAdd()+rand_number(2,3), 7, 21));  // todo - adjust str add

        // regardless, dex down quite a lot
        ch->real_abils.setDex(LIMIT(ch->real_abils.getDex() - rand_number(2, 4), 2, 16));
        // regardless, int up a little
        ch->real_abils.setIntel(LIMIT(ch->real_abils.getIntel() + rand_number(1, 2), 12, 20));
        break;
    case RACE_FADE:
        // Fades are quick, dangerous and resistant to magical effects
        switch (ch->player.chclass) {
        case CLASS_WARRIOR:
            ch->real_abils.setDex(LIMIT(ch->real_abils.getDex() + rand_number(2, 3), 14, 21));
            ch->real_abils.setStr(LIMIT(ch->real_abils.getStr() + rand_number(1, 3), 10, 18));
            break;
        case CLASS_THIEF:
            ch->real_abils.setDex(LIMIT(ch->real_abils.getDex() + rand_number(4, 5), 17, 21));
        default:
            WriteLogf("SYSERR: adjust_abils_for_race: Fade character '%s' has invalid class [%u]", GET_NAME(ch), ch->player.chclass);
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
        WriteLogf("SYSERR: adjust_abils_for_race: Character '%s' has invalid race [%u]", GET_NAME(ch), ch->player.chrace);
        //mudlog(BRF, MAX(LVL_IMMORT, GET_INVIS_LEV(d->character)), TRUE, "Failure to AddRecentPlayer (returned FALSE).");
    }
}
