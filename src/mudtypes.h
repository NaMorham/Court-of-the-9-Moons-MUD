/**
 * @file structs.h
 * Core structures used within the core mud code.
 *
 * Part of the core tbaMUD source code distribution, which is a derivative
 * of, and continuation of, CircleMUD.
 *
 * All rights reserved.  See license for complete information.
 * Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University
 * CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.
 */
#ifndef __CIRCLE__MUDTYPES_H__
#define __CIRCLE__MUDTYPES_H__

#include <limits.h>

#include "conf.h"
#include "sysdep.h"

/*
 * Type Definitions
 */
typedef signed char sbyte;          //!< 1 byte; vals = -127 to 127
typedef unsigned char ubyte;        //!< 1 byte; vals = 0 to 255
typedef signed short int sh_int;    //!< 2 bytes; vals = -32,768 to 32,767
typedef unsigned short int ush_int; //!< 2 bytes; vals = 0 to 65,535

#if !defined(__cplusplus)    // Anyone know a portable method?
typedef char                bool;       ///< Technically 1 signed byte; vals should only = TRUE or FALSE.
#endif

#if !defined(CIRCLE_WINDOWS) || defined(LCC_WIN32)    // Hm, sysdep.h?
typedef signed char byte;            //!< Technically 1 signed byte; vals should only = TRUE or FALSE.
#endif

// preamble
/**
 * As of bpl20, it should be safe to use unsigned data types for the various
 * virtual and real number data types.  There really isn't a reason to use
 * signed anymore so use the unsigned types and get 65,535 objects instead of
 * 32,768. NOTE: This will likely be unconditionally unsigned later.
 * 0 = use signed indexes; 1 = use unsigned indexes
 */
#define CIRCLE_UNSIGNED_INDEX    1

#if CIRCLE_UNSIGNED_INDEX
typedef ush_int IDXTYPE;                //!< Index types are unsigned short ints
const IDXTYPE IDXTYPE_MAX = USHRT_MAX;    //!< Used for compatibility checks.
const IDXTYPE IDXTYPE_MIN = 0;            //!< Used for compatibility checks.
const IDXTYPE NOWHERE = ((IDXTYPE)~0);    //!< Sets to ush_int_MAX, or 65,535
const IDXTYPE NOTHING = ((IDXTYPE)~0);    //!< Sets to ush_int_MAX, or 65,535
const IDXTYPE NOBODY = ((IDXTYPE)~0);    //!< Sets to ush_int_MAX, or 65,535
const IDXTYPE NOFLAG = ((IDXTYPE)~0);    //!< Sets to ush_int_MAX, or 65,535
#else
typedef    sh_int IDXTYPE;                    //!< Index types are unsigned short ints
const IDXTYPE IDXTYPE_MAX = SHRT_MAX;    //!< Used for compatibility checks.
const IDXTYPE IDXTYPE_MIN = SHRT_MIN;    //!< Used for compatibility checks.
const IDXTYPE NOWHERE = ((IDXTYPE)-1);    //!< nil reference for rooms
const IDXTYPE NOTHING = ((IDXTYPE)-1);    //!< nil reference for objects
const IDXTYPE NOBODY = ((IDXTYPE)-1);    //!< nil reference for mobiles
const IDXTYPE NOFLAG = ((IDXTYPE)-1);    //!< nil reference for flags
#endif

#endif  // __CIRCLE__MUDTYPES_H__
