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
#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#include "mudtypes.h"

/**
 * Intended use of this macro is to allow external packages to work with a
 * variety of versions without modifications.  For instance, an IS_CORPSE()
 * macro was introduced in pl13.  Any future code add-ons could take into
 * account the version and supply their own definition for the macro if used
 * on an older version. You are supposed to compare this with the macro
 * TBAMUD_VERSION() in utils.h.
 * It is read as Major/Minor/Patchlevel - MMmmPP
 */
#define _TBAMUD    0x030620

/**
 * If you want equipment to be automatically equipped to the same place
 * it was when players rented, set the define below to 1 because
 * TRUE/FALSE aren't defined yet.
 */
#define USE_AUTOEQ    1

/* preamble */

#include "mudtypes.h"
#include "CharacterAttributes.h"

/**
 * Function macro for the mob, obj and room special functions
 */
#define SPECIAL(name) \
    int (name)(struct char_data *ch, void *me, int cmd, char *argument)

// room-related defines
/*
 * The cardinal directions: used as index to room_data.dir_option[]
 */
enum eDirections
{
    NORTH = 0,    //!< The direction north
    EAST,        //!< The direction east
    SOUTH,        //!< The direction south
    WEST,        //!< The direction west
    UP,            //!< The direction up
    DOWN,        //!< The direction down

    // Total number of directions available to move in. BEFORE CHANGING THIS, make
    // sure you change every other direction and movement based item that this will
    // impact.
    NUM_OF_DIRS        // 6
};

// Room flags: used in room_data.room_flags
// WARNING: In the world files, NEVER set the bits marked "R" ("Reserved")
enum eRoomTypes
{
    ROOM_DARK = 0,        //!< Dark room, light needed to see
    ROOM_DEATH,            //!< Death trap, instant death
    ROOM_NOMOB,            //!< MOBs not allowed in room
    ROOM_INDOORS,        //!< Indoors, no weather
    ROOM_PEACEFUL,        //!< Violence not allowed
    ROOM_SOUNDPROOF,    //!< Shouts, gossip blocked
    ROOM_NOTRACK,        //!< Track won't go through
    ROOM_NOMAGIC,        //!< Magic not allowed
    ROOM_TUNNEL,        //!< Room for only 1 person
    ROOM_PRIVATE,        //!< Can't teleport in
    ROOM_GODROOM,        //!< LVL_GOD+ only allowed
    ROOM_HOUSE,            //!< (R) Room is a house
    ROOM_HOUSE_CRASH,    //!< (R) House needs saving
    ROOM_ATRIUM,        //!< (R) The door to a house
    ROOM_OLC,            //!< (R) Modifyable/!compress
    ROOM_BFS_MARK,        //!< (R) breath-first srch mrk
    ROOM_WORLDMAP,        //!< World-map style maps here

    // Must be last
    NUM_ROOM_FLAGS        //!< The total number of Room Flags
};

// Zone info: Used in zone_data.zone_flags
#define ZONE_CLOSED       0  //!< Zone is closed - players cannot enter
#define ZONE_NOIMMORT     1  //!< Immortals (below LVL_GRGOD) cannot enter this zone
#define ZONE_QUEST        2  //!< This zone is a quest zone (not implemented)
#define ZONE_GRID         3  //!< Zone is 'on the grid', connected, show on 'areas'
#define ZONE_NOBUILD      4  //!< Building is not allowed in the zone
#define ZONE_NOASTRAL     5  //!< No teleportation magic will work to or from this zone
/** The total number of Zone Flags */
#define NUM_ZONE_FLAGS    6

// Exit info: used in room_data.dir_option.exit_info
#define EX_ISDOOR     (1 << 0)   //!< Exit is a door
#define EX_CLOSED     (1 << 1)   //!< The door is closed
#define EX_LOCKED     (1 << 2)   //!< The door is locked
#define EX_PICKPROOF  (1 << 3)   //!< Lock can't be picked
#define EX_BREAKABLE  (1 << 4)   //!< The door can be smashed
#define EX_BROKEN     (1 << 5)   //!< The exit is broken
#define EX_HIDDEN     (1 << 6)   //!< The exit is hidden

// Sector types: used in room_data.sector_type
enum eSectorType
{
    SECT_INSIDE = 0,    //!< Indoors, connected to SECT macro.
    SECT_CITY,            //!< In a city
    SECT_FIELD,            //!< In a field
    SECT_FOREST,        //!< In a forest
    SECT_HILLS,            //!< In the hills
    SECT_MOUNTAIN,        //!< On a mountain
    SECT_WATER_SWIM,    //!< Swimmable water
    SECT_WATER_NOSWIM,    //!< Water - need a boat
    SECT_FLYING,        //!< Flying
    SECT_UNDERWATER,    //!< Underwater
    SECT_TRACK,         //!< On a dirt path or track

    // Must be last
    NUM_ROOM_SECTORS    //<! The total number of room Sector Types
};

// char and mob-related defines

// History
#define HIST_ALL       0 //!< Index to history of all channels
#define HIST_SAY       1 //!< Index to history of all 'say'
#define HIST_GOSSIP    2 //!< Index to history of all 'gossip'
#define HIST_WIZNET    3 //!< Index to history of all 'wiznet'
#define HIST_TELL      4 //!< Index to history of all 'tell'
#define HIST_SHOUT     5 //!< Index to history of all 'shout'
#define HIST_GRATS     6 //!< Index to history of all 'grats'
#define HIST_HOLLER    7 //!< Index to history of all 'holler'
#define HIST_AUCTION   8 //!< Index to history of all 'auction'

#define NUM_HIST       9 //!< Total number of history indexes

#define HISTORY_SIZE   5 //!< Number of last commands kept in each history

// PC classes
#define CLASS_UNDEFINED      (-1) //!< PC Class undefined
#define CLASS_MAGIC_USER  0    //!< PC Class Magic User
#define CLASS_CLERIC      1    //!< PC Class Cleric
#define CLASS_THIEF       2    //!< PC Class Thief
#define CLASS_WARRIOR     3    //!< PC Class Warrior
//* Total number of available PC Classes
#define NUM_CLASSES      4

// NPC classes (currently unused - feel free to implement!)
#define CLASS_OTHER       0    //!< NPC Class Other (or undefined)
#define CLASS_UNDEAD      1    //!< NPC Class Undead
#define CLASS_HUMANOID    2    //!< NPC Class Humanoid
#define CLASS_ANIMAL      3    //!< NPC Class Animal
#define CLASS_DRAGON      4    //!< NPC Class Dragon
#define CLASS_GIANT       5    //!< NPC Class Giant

enum eRaces
{
    RACE_UNKNOWN = -1,
    RACE_HUMAN,
    RACE_TROLLOC,
    RACE_OGIER,
    RACE_FADE,

    NUM_RACES    // Must be last
};

// Sex
#define SEX_NEUTRAL   0        //!< Neutral Sex (Hermaphrodite)
#define SEX_MALE      1        //!< Male Sex (XY Chromosome)
#define SEX_FEMALE    2        //!< Female Sex (XX Chromosome)

#define NUM_GENDERS   3        //<! Total number of Genders

// Positions
enum ePositions
{
    POS_DEAD = 0,    //!< Position = dead
    POS_MORTALLYW,    //!< Position = mortally wounded
    POS_INCAP,        //!< Position = incapacitated
    POS_STUNNED,    //!< Position = stunned
    POS_SLEEPING,    //!< Position = sleeping
    POS_RESTING,    //!< Position = resting
    POS_SITTING,    //!< Position = sitting
    POS_FIGHTING,    //!< Position = fighting
    POS_STANDING,    //!< Position = standing

    NUM_POSITIONS    //!< Total number of positions
};

// Player flags: used by char_data.char_specials.act
enum ePlayerFlags
{
    PLR_KILLER = 0,        //!< Player is a player-killer
    PLR_THIEF,            //!< Player is a player-thief
    PLR_FROZEN,            //!< Player is frozen
    PLR_DONTSET,        //!< Don't EVER set (ISNPC bit, set by mud)
    PLR_WRITING,        //!< Player writing (board/mail/olc)
    PLR_MAILING,        //!< Player is writing mail
    PLR_CRASH,            //!< Player needs to be crash-saved
    PLR_SITEOK,            //!< Player has been site-cleared
    PLR_NOSHOUT,        //!< Player not allowed to shout/goss
    PLR_NOTITLE,        //!< Player not allowed to set title
    PLR_DELETED,        //!< Player deleted - space reusable
    PLR_LOADROOM,        //!< Player uses nonstandard loadroom
    PLR_NOWIZLIST,        //!< Player shouldn't be on wizlist
    PLR_NODELETE,        //!< Player shouldn't be deleted
    PLR_INVSTART,        //!< Player should enter game wizinvis
    PLR_CRYO,            //!< Player is cryo-saved (purge prog)
    PLR_NOTDEADYET,        //!< (R) Player being extracted
    PLR_BUG,            //!< Player is writing a bug
    PLR_IDEA,           //!< Player is writing an idea
    PLR_TYPO,           //!< Player is writing a typo

    // Must be last
    NUM_PLAYER_FLAGS     //!< Number of player flags in use
};

enum eMobFlags
{
    // Mobile flags: used by char_data.char_specials.act
    MOB_SPEC = 0,        //!< Mob has a callable spec-proc
    MOB_SENTINEL,        //!< Mob should not move
    MOB_SCAVENGER,        //!< Mob picks up stuff on the ground
    MOB_ISNPC,            //!< (R) Automatically set on all Mobs
    MOB_AWARE,            //!< Mob can't be backstabbed
    MOB_AGGRESSIVE,        //!< Mob auto-attacks everybody nearby
    MOB_STAY_ZONE,        //!< Mob shouldn't wander out of zone
    MOB_WIMPY,            //!< Mob flees if severely injured
    MOB_AGGR_EVIL,        //!< Auto-attack any evil PC's
    MOB_AGGR_GOOD,        //!< Auto-attack any good PC's
    MOB_AGGR_NEUTRAL,    //!< Auto-attack any neutral PC's
    MOB_MEMORY,            //!< remember attackers if attacked
    MOB_HELPER,            //!< attack PCs fighting other NPCs
    MOB_NOCHARM,        //!< Mob can't be charmed
    MOB_NOSUMMON,        //!< Mob can't be summoned
    MOB_NOSLEEP,        //!< Mob can't be slept
    MOB_NOBASH,            //!< Mob can't be bashed (e.g. trees)
    MOB_NOBLIND,        //!< Mob can't be blinded
    MOB_NOTDEADYET,        //!< (R) Mob being extracted

    // Must be last
    NUM_MOB_FLAGS    //<! Total number of Mob Flags; it should be 1 less than MOB_NOT_DEADYET
};

// Preference flags: used by char_data.player_specials.pref
enum ePreferenceFlags
{
    PRF_BRIEF = 0,        //!< Room descs won't normally be shown
    PRF_COMPACT,        //!< No extra CRLF pair before prompts
    PRF_NOSHOUT,        //!< Can't hear shouts
    PRF_NOTELL,            //!< Can't receive tells
    PRF_DISPHP,            //!< Display hit points in prompt
    PRF_DISPMANA,        //!< Display mana points in prompt
    PRF_DISPMOVE,        //!< Display move points in prompt
    PRF_AUTOEXIT,        //!< Display exits in a room
    PRF_NOHASSLE,        //!< Aggr mobs won't attack
    PRF_QUEST,            //!< On quest
    PRF_SUMMONABLE,        //!< Can be summoned
    PRF_NOREPEAT,        //!< No repetition of comm commands
    PRF_HOLYLIGHT,        //!< Can see in dark
    PRF_COLOR_1,        //!< Color (low bit)
    PRF_COLOR_2,        //!< Color (high bit)
    PRF_NOWIZ,            //!< Can't hear wizline
    PRF_LOG1,            //!< On-line System Log (low bit)
    PRF_LOG2,            //!< On-line System Log (high bit)
    PRF_NOAUCT,            //!< Can't hear auction channel
    PRF_NOGOSS,            //!< Can't hear gossip channel
    PRF_NOGRATZ,        //!< Can't hear grats channel
    PRF_SHOWVNUMS,        //!< Can see VNUMs
    PRF_DISPAUTO,        //!< Show prompt HP, MP, MV when < 25%
    PRF_CLS,            //!< Clear screen in OLC
    PRF_BUILDWALK,        //!< Build new rooms while walking
    PRF_AFK,            //!< AFK flag
    PRF_AUTOLOOT,        //!< Loot everything from a corpse
    PRF_AUTOGOLD,        //!< Loot gold from a corpse
    PRF_AUTOSPLIT,        //!< Split gold with group
    PRF_AUTOSAC,        //!< Sacrifice a corpse
    PRF_AUTOASSIST,        //!< Auto-assist toggle
    PRF_AUTOMAP,        //!< Show map at the side of room descs
    PRF_AUTOKEY,        //!< Automatically unlock locked doors when opening
    PRF_AUTODOOR,       //!< Use the next available door

    // Must be last
    NUM_PRF_FLAGS        //!< Total number of available PRF flags
};

// Affect bits: used in char_data.char_specials.saved.affected_by
// WARNING: In the world files, NEVER set the bits marked "R" ("Reserved")
enum eAffectBits
{
    AFF_DONTUSE = 0,    //!< DON'T USE!
    AFF_BLIND,            //!< (R) Char is blind
    AFF_INVISIBLE,        //!< Char is invisible
    AFF_DETECT_ALIGN,    //!< Char is sensitive to align
    AFF_DETECT_INVIS,    //!< Char can see invis chars
    AFF_DETECT_MAGIC,    //!< Char is sensitive to magic
    AFF_SENSE_LIFE,        //!< Char can sense hidden life
    AFF_WATERWALK,        //!< Char can walk on water
    AFF_SANCTUARY,        //!< Char protected by sanct
    AFF_GROUP,            //!< (R) Char is grouped
    AFF_CURSE,            //!< Char is cursed
    AFF_INFRAVISION,    //!< Char can see in dark
    AFF_POISON,            //!< (R) Char is poisoned
    AFF_PROTECT_EVIL,    //!< Char protected from evil
    AFF_PROTECT_GOOD,    //!< Char protected from good
    AFF_SLEEP,            //!< (R) Char magically asleep
    AFF_NOTRACK,        //!< Char can't be tracked
    AFF_FLYING,            //!< Char is flying
    AFF_SCUBA,            //!< Room for future expansion
    AFF_SNEAK,            //!< Char can move quietly
    AFF_HIDE,            //!< Char is hidden
    AFF_FREE,            //!< Room for future expansion
    AFF_CHARM,            //!< Char is charmed

    // Must be last
    NUM_AFF_FLAGS        //!< Total number of affect flags not including the don't use flag.
};

// Modes of connectedness: used by descriptor_data.state
enum eConnectionState
{
    CON_PLAYING = 0,    //!< Playing - Nominal state
    CON_CLOSE,            //!< User disconnect, remove character.
    CON_GET_NAME,        //!< Login with name
    CON_NAME_CNFRM,        //!< New character, confirm name
    CON_PASSWORD,        //!< Login with password
    CON_NEWPASSWD,        //!< New character, create password
    CON_CNFPASSWD,        //!< New character, confirm password
    CON_QSEX,            //!< Choose character sex
    CON_QCLASS,            //!< Choose character class
    CON_RMOTD,            //!< Reading the message of the day
    CON_MENU,            //!< At the main menu
    CON_PLR_DESC,        //!< Enter a new character description prompt
    CON_CHPWD_GETOLD,    //!< Changing passwd: Get old
    CON_CHPWD_GETNEW,    //!< Changing passwd: Get new
    CON_CHPWD_VRFY,        //!< Changing passwd: Verify new password
    CON_DELCNF1,        //!< Character Delete: Confirmation 1
    CON_DELCNF2,        //!< Character Delete: Confirmation 2
    CON_DISCONNECT,        //!< In-game link loss (leave character)
    CON_OEDIT,            //!< OLC mode - object editor
    CON_REDIT,            //!< OLC mode - room editor
    CON_ZEDIT,            //!< OLC mode - zone info editor
    CON_MEDIT,            //!< OLC mode - mobile editor
    CON_SEDIT,            //!< OLC mode - shop editor
    CON_TEDIT,            //!< OLC mode - text editor
    CON_CEDIT,            //!< OLC mode - conf editor
    CON_AEDIT,            //!< OLC mode - social (action) edit
    CON_TRIGEDIT,        //!< OLC mode - trigger edit
    CON_HEDIT,            //!< OLC mode - help edit
    CON_QEDIT,            //!< OLC mode - quest edit
    CON_PREFEDIT,       //!< OLC mode - preference edit
    CON_IBTEDIT,        //!< OLC mode - idea/bug/typo edit

    // Must be last
    NUM_CON_STATES        //!< Number of connection states
};

// OLC States range - used by IS_IN_OLC and IS_PLAYING
#define FIRST_OLC_STATE CON_OEDIT     //!< The first CON_ state that is an OLC
#define LAST_OLC_STATE  CON_PREFEDIT  //!< The last CON_ state that is an OLC

// Character equipment positions: used as index for char_data.equipment[]
/*
 * NOTE: Don't confuse these constants with the ITEM_ bitvectors
 * which control the valid places you can wear a piece of equipment.
 * For example, there are two neck positions on the player, and items
 * only get the generic neck type.
 */
enum eWearPositions
{
    WEAR_LIGHT = 0,        //!< Equipment Location Light
    WEAR_FINGER_R,        //!< Equipment Location Right Finger
    WEAR_FINGER_L,        //!< Equipment Location Left Finger
    WEAR_NECK_1,        //!< Equipment Location Neck #1
    WEAR_NECK_2,        //!< Equipment Location Neck #2
    WEAR_BODY,            //!< Equipment Location Body
    WEAR_HEAD,            //!< Equipment Location Head
    WEAR_LEGS,            //!< Equipment Location Legs
    WEAR_FEET,            //!< Equipment Location Feet
    WEAR_HANDS,            //!< Equipment Location Hands
    WEAR_ARMS,            //!< Equipment Location Arms
    WEAR_SHIELD,        //!< Equipment Location Shield
    WEAR_ABOUT,            //!< Equipment Location about body (like a cape)
    WEAR_WAIST,            //!< Equipment Location Waist
    WEAR_WRIST_R,        //!< Equipment Location Right Wrist
    WEAR_WRIST_L,        //!< Equipment Location Left Wrist
    WEAR_WIELD,            //!< Equipment Location Weapon
    WEAR_HOLD,            //!< Equipment Location held in offhand

    // Must be last
    NUM_WEARS,            //!< Total number of available equipment lcoations
};

// object-related defines
/**
 * Item types: used by obj_data.obj_flags.type_flag
 */
enum eItemTypes
{
    ITEM_LIGHT = 1,        //!< Item is a light source
    ITEM_SCROLL,        //!< Item is a scroll
    ITEM_WAND,            //!< Item is a wand
    ITEM_STAFF,            //!< Item is a staff
    ITEM_WEAPON,        //!< Item is a weapon
    ITEM_FURNITURE,        //!< Sittable Furniture
    ITEM_FREE,            //!< Unimplemented
    ITEM_TREASURE,        //!< Item is a treasure, not gold
    ITEM_ARMOR,            //!< Item is armor
    ITEM_POTION,        //!< (10) Item is a potion
    ITEM_WORN,            //!< Unimplemented
    ITEM_OTHER,            //!< Misc object
    ITEM_TRASH,            //!< Trash - shopkeepers won't buy
    ITEM_FREE2,            //!< Unimplemented
    ITEM_CONTAINER,        //!< Item is a container
    ITEM_NOTE,            //!< Item is note
    ITEM_DRINKCON,        //!< Item is a drink container
    ITEM_KEY,            //!< Item is a key
    ITEM_FOOD,            //!< Item is food
    ITEM_MONEY,            //!< (20) Item is money (gold)
    ITEM_PEN,            //!< Item is a pen
    ITEM_BOAT,            //!< Item is a boat
    ITEM_FOUNTAIN,        //!< Item is a fountain

    // @todo Should this be 23? */
    NUM_ITEM_TYPES        //!< Total number of item types. @todo Should this be 23?
};

/**
 * Take/Wear flags: used by obj_data.obj_flags.wear_flags
 */
enum eWearFlags
{
    ITEM_WEAR_TAKE = 0,        //!< Item can be taken
    ITEM_WEAR_FINGER,        //!< Item can be worn on finger
    ITEM_WEAR_NECK,            //!< Item can be worn around neck
    ITEM_WEAR_BODY,            //!< Item can be worn on body
    ITEM_WEAR_HEAD,            //!< Item can be worn on head
    ITEM_WEAR_LEGS,            //!< Item can be worn on legs
    ITEM_WEAR_FEET,            //!< Item can be worn on feet
    ITEM_WEAR_HANDS,        //!< Item can be worn on hands
    ITEM_WEAR_ARMS,            //!< Item can be worn on arms
    ITEM_WEAR_SHIELD,        //!< Item can be used as a shield
    ITEM_WEAR_ABOUT,        //!< Item can be worn about body
    ITEM_WEAR_WAIST,        //!< Item can be worn around waist
    ITEM_WEAR_WRIST,        //!< Item can be worn on wrist
    ITEM_WEAR_WIELD,        //!< Item can be wielded
    ITEM_WEAR_HOLD,            //!< Item can be held

    // Must be last
    NUM_ITEM_WEARS            //!< Total number of item wears
};

/* Extra object flags: used by obj_data.obj_flags.extra_flags */
enum eObjectFlags
{
    ITEM_GLOW = 0,            //!< Item is glowing
    ITEM_HUM,                //!< Item is humming
    ITEM_NORENT,            //!< Item cannot be rented
    ITEM_NODONATE,            //!< Item cannot be donated
    ITEM_NOINVIS,            //!< Item cannot be made invis
    ITEM_INVISIBLE,            //!< Item is invisible
    ITEM_MAGIC,                //!< Item is magical
    ITEM_NODROP,            //!< Item is cursed: can't drop
    ITEM_BLESS,                //!< Item is blessed
    ITEM_ANTI_GOOD,            //!< Not usable by good people
    ITEM_ANTI_EVIL,            //!< Not usable by evil people
    ITEM_ANTI_NEUTRAL,        //!< Not usable by neutral people
    ITEM_ANTI_MAGIC_USER,    //!< Not usable by mages
    ITEM_ANTI_CLERIC,        //!< Not usable by clerics
    ITEM_ANTI_THIEF,        //!< Not usable by thieves
    ITEM_ANTI_WARRIOR,        //!< Not usable by warriors
    ITEM_NOSELL,            //!< Shopkeepers won't touch it
    ITEM_QUEST,                //!< Item is a quest item
    ITEM_ANTI_HUMAN,        //!< Not usable by humans
    ITEM_ANTI_TROLLOC,        //!< Not usable by trollocs
    ITEM_ANTI_OGIER,        //!< Not usable by ogier
    ITEM_ANTI_FADE,            //!< Not usable by fades

    // Must be last
    NUM_ITEM_FLAGS            //!< Total number of item flags
};

/* Modifier constants used with obj affects ('A' fields) */
enum eApplyModifers
{
    APPLY_NONE = 0,            //!< No effect
    APPLY_STR,                //!< Apply to strength
    APPLY_DEX,                //!< Apply to dexterity
    APPLY_INT,                //!< Apply to intelligence
    APPLY_WIS,                //!< Apply to wisdom
    APPLY_CON,                //!< Apply to constitution
    APPLY_CHA,                //!< Apply to charisma
    APPLY_CLASS,            //!< Reserved
    APPLY_RACE,                //!< Reserved
    APPLY_LEVEL,            //!< Reserved
    APPLY_AGE,                //!< Apply to age
    APPLY_CHAR_WEIGHT,        //!< Apply to weight
    APPLY_CHAR_HEIGHT,        //!< Apply to height
    APPLY_MANA,                //!< Apply to max mana
    APPLY_HIT,                 //!< Apply to max hit points
    APPLY_MOVE,                //!< Apply to max move points
    APPLY_GOLD,                //!< Reserved
    APPLY_EXP,                //!< Reserved
    APPLY_AC,                //!< Apply to Armor Class
    APPLY_HITROLL,            //!< Apply to hitroll
    APPLY_DAMROLL,            //!< Apply to damage roll
    APPLY_SAVING_PARA,        //!< Apply to save throw: paralysis
    APPLY_SAVING_ROD,        //!< Apply to save throw: rods
    APPLY_SAVING_PETRI,        //!< Apply to save throw: petrif
    APPLY_SAVING_BREATH,    //!< Apply to save throw: breath
    APPLY_SAVING_SPELL,        //!< Apply to save throw: spells

    // Must be last
    NUM_APPLIES                //!< Total number of applies
};

// Equals the total number of SAVING_* defines in spells.h
#define NUM_OF_SAVING_THROWS  5

// Container flags - value[1]
#define CONT_CLOSEABLE      (1 << 0)    //!< Container can be closed
#define CONT_PICKPROOF      (1 << 1)    //!< Container is pickproof
#define CONT_CLOSED         (1 << 2)    //!< Container is closed
#define CONT_LOCKED         (1 << 3)    //!< Container is locked

/* Some different kind of liquids for use in values of drink containers */
enum eLiquidTypes
{
    LIQ_WATER = 0,        //!< Liquid type water
    LIQ_BEER,            //!< Liquid type beer
    LIQ_WINE,            //!< Liquid type wine
    LIQ_ALE,            //!< Liquid type ale
    LIQ_DARKALE,        //!< Liquid type darkale
    LIQ_WHISKY,            //!< Liquid type whisky
    LIQ_LEMONADE,        //!< Liquid type lemonade
    LIQ_FIREBRT,        //!< Liquid type firebrt
    LIQ_LOCALSPC,        //!< Liquid type localspc
    LIQ_SLIME,            //!< Liquid type slime
    LIQ_MILK,            //!< Liquid type milk
    LIQ_TEA,            //!< Liquid type tea
    LIQ_COFFE,            //!< Liquid type coffee
    LIQ_BLOOD,            //!< Liquid type blood
    LIQ_SALTWATER,        //!< Liquid type saltwater
    LIQ_CLEARWATER,        //!< Liquid type clearwater

    // Must be last
    NUM_LIQ_TYPES        //!< Total number of liquid types
};

// other miscellaneous defines
/**
 * Player conditions
 */
#define DRUNK        0  //!< Player drunk condition
#define HUNGER       1  //!< Player hunger condition
#define THIRST       2  //!< Player thirst condition

/**
 * Sun state for weather_data
 */
#define SUN_DARK    0  //!< Night time
#define SUN_RISE    1  //!< Dawn
#define SUN_LIGHT    2  //!< Day time
#define SUN_SET        3  //!< Dusk

/**
 * Sky conditions for weather_data
 */
#define SKY_CLOUDLESS  0  //!< Weather = No clouds
#define SKY_CLOUDY     1  //!< Weather = Cloudy
#define SKY_RAINING    2  //!< Weather = Rain
#define SKY_LIGHTNING  3  //!< Weather = Lightning storm

/**
 * Rent codes
 */
#define RENT_UNDEF      0 //!< Character inv save status = undefined
#define RENT_CRASH      1 //!< Character inv save status = game crash
#define RENT_RENTED     2 //!< Character inv save status = rented
#define RENT_CRYO       3 //!< Character inv save status = cryogenics
#define RENT_FORCED     4 //!< Character inv save status = forced rent
#define RENT_TIMEDOUT   5 //!< Character inv save status = timed out

/**
 * Settings for Bit Vectors
 */
#define RF_ARRAY_MAX    4  //!< # Bytes in Bit vector - Room flags
#define PM_ARRAY_MAX    4  //!< # Bytes in Bit vector - Act and Player flags
#define PR_ARRAY_MAX    4  //!< # Bytes in Bit vector - Player Pref Flags
#define AF_ARRAY_MAX    4  //!< # Bytes in Bit vector - Affect flags
#define TW_ARRAY_MAX    4  //!< # Bytes in Bit vector - Obj Wear Locations
#define EF_ARRAY_MAX    4  //!< # Bytes in Bit vector - Obj Extra Flags
#define ZN_ARRAY_MAX    4  //!< # Bytes in Bit vector - Zone Flags

// other #defined constants
/**
 * **DO**NOT** blindly change the number of levels in your MUD merely by
 * changing these numbers and without changing the rest of the code to match.
 * Other changes throughout the code are required.  See coding.doc for details.
 *
 * LVL_IMPL should always be the HIGHEST possible immortal level, and
 * LVL_IMMORT should always be the LOWEST immortal level.  The number of
 * mortal levels will always be LVL_IMMORT - 1.
 */
#define LVL_IMPL    34  //!< Level of Implementors
#define LVL_GRGOD   33  //!< Level of Greater Gods
#define LVL_GOD     32  //!< Level of Gods
#define LVL_IMMORT    31  //!< Level of Immortals

/**
 * Minimum level to build and to run the saveall command
 */
#define LVL_BUILDER    LVL_IMMORT

/**
 * Arbitrary number that won't be in a string
 */
#define MAGIC_NUMBER    (0x06)

/**
 * OPT_USEC determines how many commands will be processed by the MUD per
 * second and how frequently it does socket I/O.  A low setting will cause
 * actions to be executed more frequently but will increase overhead due to
 * more cycling to check.  A high setting (e.g. 1 Hz) may upset your players
 * as actions (such as large speedwalking chains) take longer to be executed.
 * You shouldn't need to adjust this.
 * This will equate to 10 passes per second.
 * @see PASSES_PER_SEC
 * @see RL_SEC
 */
#define OPT_USEC    100000
/**
 * How many heartbeats equate to one real second.
 * @see OPT_USEC
 * @see RL_SEC
 */
#define PASSES_PER_SEC    (1000000 / OPT_USEC)
/**
 * Used with other macros to define at how many heartbeats a control loop
 * gets executed. Helps to translate pulse counts to real seconds for
 * human comprehension.
 * @see PASSES_PER_SEC
 */
#define RL_SEC        * PASSES_PER_SEC

/** Controls when a zone update will occur. */
#define PULSE_ZONE      (10 RL_SEC)
/** Controls when mobile (NPC) actions and updates will occur. */
#define PULSE_MOBILE    (10 RL_SEC)
/** Controls the time between turns of combat. */
#define PULSE_VIOLENCE  ( 2 RL_SEC)
/**
 * Controls when characters and houses (if implemented) will be autosaved.
 * @see CONFIG_AUTO_SAVE
 */
#define PULSE_AUTOSAVE  (60 RL_SEC)
/** Controls when checks are made for idle name and password CON_ states */
#define PULSE_IDLEPWD   (15 RL_SEC)
/** Currently unused. */
#define PULSE_SANITY    (30 RL_SEC)
/**
 * How often to log # connected sockets and # active players.
 * Currently set for 5 minutes.
 */
#define PULSE_USAGE     (5 * 60 RL_SEC)
/**
 * Controls when to save the current ingame MUD time to disk.
 * This should be set >= SECS_PER_MUD_HOUR
 */
#define PULSE_TIMESAVE    (30 * 60 RL_SEC)

// Variables for the output buffering system
#define MAX_SOCK_BUF       (24 * 1024) //!< Size of kernel's sock buf
#define MAX_PROMPT_LENGTH  96          //!< Max length of prompt
#define GARBAGE_SPACE      32          //!< Space for **OVERFLOW** etc
#define SMALL_BUFSIZE      1024        //!< Static output buffer size
/** Max amount of output that can be buffered */
#define LARGE_BUFSIZE      (MAX_SOCK_BUF - GARBAGE_SPACE - MAX_PROMPT_LENGTH)

#define MAX_STRING_LENGTH     49152  //!< Max length of string, as defined
#define MAX_INPUT_LENGTH      512    //!< Max length per *line* of input
#define MAX_RAW_INPUT_LENGTH  1024   //!< Max size of *raw* input
#define MAX_MESSAGES          60     //!< Max Different attack message types
#define MAX_NAME_LENGTH       20     //!< Max PC/NPC name length
#define MAX_PWD_LENGTH        30     //!< Max PC password length
#define MAX_TITLE_LENGTH      80     //!< Max PC title length
#define HOST_LENGTH           40     //!< Max hostname resolution length
#define PLR_DESC_LENGTH       4096   //!< Max length for PC description
#define MAX_SKILLS            200    //!< Max number of skills/spells
#define MAX_AFFECT            32     //!< Max number of player affections
#define MAX_OBJ_AFFECT        6      //!< Max object affects
#define MAX_NOTE_LENGTH       4000   //!< Max length of text on a note obj
#define MAX_LAST_ENTRIES      6000   //!< Max log entries??
#define MAX_HELP_KEYWORDS     256    //!< Max length of help keyword string
#define MAX_HELP_ENTRY        MAX_STRING_LENGTH //!< Max size of help entry
#define MAX_COMPLETED_QUESTS  1024   //!< Maximum number of completed quests allowed

/**
 * Define the largest set of commands for a trigger.
 * 16k should be plenty and then some.
 */
#define MAX_CMD_LENGTH 16384

/* Various virtual (human-reference) number types. */
typedef IDXTYPE room_vnum;    //!< vnum specifically for room
typedef IDXTYPE obj_vnum;    //!< vnum specifically for object
typedef IDXTYPE mob_vnum;    //!< vnum specifically for mob (NPC)
typedef IDXTYPE zone_vnum;    //!< vnum specifically for zone
typedef IDXTYPE shop_vnum;    //!< vnum specifically for shop
typedef IDXTYPE trig_vnum;    //!< vnum specifically for triggers
typedef IDXTYPE qst_vnum;    //!< vnum specifically for quests

/* Various real (array-reference) number types. */
typedef IDXTYPE room_rnum;    //!< references an instance of a room
typedef IDXTYPE obj_rnum;    //!< references an instance of a obj
typedef IDXTYPE mob_rnum;    //!< references an instance of a mob (NPC)
typedef IDXTYPE zone_rnum;    //!< references an instance of a zone
typedef IDXTYPE shop_rnum;    //!< references an instance of a shop
typedef IDXTYPE trig_rnum;    //!< references an instance of a trigger
typedef IDXTYPE qst_rnum;    //!< references an instance of a quest

/**
 * Bitvector type for 32 bit unsigned long bitvectors. 'unsigned long long'
 * will give you at least 64 bits if you have GCC. You'll have to search
 * throughout the code for "bitvector_t" and change them yourself if you'd
 * like this extra flexibility.
 */
typedef unsigned long int bitvector_t;

/**
 * Extra description: used in objects, mobiles, and rooms. For example,
 * a 'look hair' might pull up an extra description (if available) for
 * the mob, object or room.
 * Multiple extra descriptions on the same object are implemented as a
 * linked list.
 */
struct extra_descr_data
{
    char *keyword;                    //!< Keyword for look/examine
    char *description;                //!< What is shown when this keyword is 'seen'
    struct extra_descr_data *next;    //!< Next description for this mob/obj/room
};

// object-related structures
/**
 * Number of elements in the object value array. Raising this will provide
 * more configurability per object type, and shouldn't break anything.
 * DO NOT LOWER from the default value of 4.
 */
#define NUM_OBJ_VAL_POSITIONS 4

/**
 * Object flags used in obj_data. These represent the instance values for
 * a real object, values that can change during gameplay.
 */
struct obj_flag_data
{
    int value[NUM_OBJ_VAL_POSITIONS]; ///!< Values of the item (see list)
    byte type_flag;                   ///!< Type of item
    int level;                        ///!< Minimum level to use object
    int wear_flags[TW_ARRAY_MAX];     ///!< Where you can wear it, if wearable
    int extra_flags[EF_ARRAY_MAX];    ///!< If it hums, glows, etc.
    int weight;                       ///!< Weight of the object
    int cost;                         ///!< Value when sold
    int cost_per_day;                 ///!< Rent cost per real day
    int timer;                        ///!< Timer for object
    int bitvector[AF_ARRAY_MAX];      ///!< Affects characters
};

/**
 * Used in obj_file_elem. DO NOT CHANGE if you are using binary object files
 * and already have a player base and don't want to do a player wipe.
 */
struct obj_affected_type
{
    byte location;  ///!< Which ability to change (APPLY_XXX)
    sbyte modifier; ///!< How much it changes by
};

/**
 * The Object structure.
 */
struct obj_data
{
    obj_rnum item_number;                ///!< The unique id of this object instance.
    room_rnum in_room;                    ///!< What room is the object lying in, or -1?

    struct obj_flag_data obj_flags;        ///!< Object information
    struct obj_affected_type affected[MAX_OBJ_AFFECT]; ///!< affects

    char *name;                            ///!< Keyword reference(s) for object.
    char *description;                    ///!< Shown when the object is lying in a room.
    char *short_description;            ///!< Shown when worn, carried, in a container
    char *action_description;            ///!< Displays when (if) the object is used
    struct extra_descr_data *ex_description; ///!< List of extra descriptions
    struct char_data *carried_by;        ///!< Points to PC/NPC carrying, or NULL
    struct char_data *worn_by;            ///!< Points to PC/NPC wearing, or NULL
    sh_int worn_on;                        ///!< If the object can be worn, where can it be worn?

    struct obj_data *in_obj;            ///!< Points to carrying object, or NULL
    struct obj_data *contains;            ///!< List of objects being carried, or NULL

    long id;                            ///!< used by DG triggers - unique id
    struct trig_proto_list *proto_script; ///!< list of default triggers
    struct script_data *script;         ///!< script info for the object

    struct obj_data *next_content;        ///!< For 'contains' lists
    struct obj_data *next;                ///!< For the object list
    struct char_data *sitting_here;        ///!< For furniture, who is sitting in it
};

/**
 * Instance info for an object that gets saved to disk.
 * DO NOT CHANGE if you are using binary object files
 * and already have a player base and don't want to do a player wipe.
 */
struct obj_file_elem
{
    obj_vnum item_number;                ///!< The prototype, non-unique info for this object.

#if USE_AUTOEQ
    sh_int location;                    ///!< If re-equipping objects on load, wear object here
#endif
    int value[NUM_OBJ_VAL_POSITIONS];    ///!< Current object values
    int extra_flags[EF_ARRAY_MAX];        ///!< Object extra flags
    int weight;                            ///!< Object weight
    int timer;                            ///!< Current object timer setting
    int bitvector[AF_ARRAY_MAX];        ///!< Object affects
    struct obj_affected_type affected[MAX_OBJ_AFFECT]; ///!< Affects to mobs
};

/**
 * Header block for rent files.
 * DO NOT CHANGE the structure if you are using binary object files
 * and already have a player base and don't want to do a player wipe.
 * If you are using binary player files, feel free to turn the spare
 * variables into something more meaningful, as long as you keep the
 * int datatype.
 * NOTE: This is *not* used with the ascii playerfiles.
 * NOTE 2: This structure appears to be unused in this codebase?
 */
struct rent_info
{
    int time;
    int rentcode;          ///!< How this character rented
    int net_cost_per_diem; ///!< ? Appears to be unused ?
    int gold;              ///!< ? Appears to be unused ?
    int account;           ///!< ? Appears to be unused ?
    int nitems;            ///!< ? Appears to be unused ?
    int spare0;
    int spare1;
    int spare2;
    int spare3;
    int spare4;
    int spare5;
    int spare6;
    int spare7;
};

// room-related structures

/**
 * Direction (north, south, east...) information for rooms.
 */
struct room_direction_data
{
    char *general_description;            ///!< Show to char looking in this direction.

    char *keyword;                        ///!< for interacting (open/close) this direction

    sh_int /*bitvector_t*/ exit_info;    ///!< Door, and what type?
    obj_vnum key;                        ///!< Key's vnum (-1 for no key)
    room_rnum to_room;                    ///!< Where direction leads, or NOWHERE if not defined
};

/**
 * The Room Structure.
 */
struct room_data
{
    room_vnum number;                ///!< Rooms number (vnum)
    zone_rnum zone;                    ///!< Room zone (for resetting)
    int sector_type;                ///!< sector type (move/hide)
    int room_flags[RF_ARRAY_MAX];    ///!< INDOORS, DARK, etc
    char *name;                        ///!< Room name
    char *description;                ///!< Shown when entered, looked at
    struct extra_descr_data *ex_description; ///!< Additional things to look at
    struct room_direction_data *dir_option[NUM_OF_DIRS]; ///!< Directions
    byte light;                        ///!< Number of lightsources in room
    SPECIAL(*func);                    ///!< Points to special function attached to room
    struct trig_proto_list *proto_script; ///!< list of default triggers
    struct script_data *script;        ///!< script info for the room
    struct obj_data *contents;        ///!< List of items in room
    struct char_data *people;        ///!< List of NPCs / PCs in room
};

// char-related structures
/**
 * Memory structure used by NPCs to remember specific PCs.
 */
struct memory_rec_struct
{
    long id;                        ///!< The PC id to remember.
    struct memory_rec_struct *next; ///!< Next PC to remember
};

/**
 * memory_rec_struct typedef
 */
typedef struct memory_rec_struct memory_rec;

/**
 * This structure is purely intended to be an easy way to transfer and return
 * information about time (real or mudwise).
 */
struct time_info_data
{
    int hours;   ///!< numeric hour
    int day;     ///!< numeric day
    int month;   ///!< numeric month
    sh_int year; ///!< numeric year
};

/**
 * Player specific time information.
 */
struct time_data
{
    time_t birth; ///!< Represents the PCs birthday, used to calculate age.
    time_t logon; ///!< Time of the last logon, used to calculate time played
    int played;   ///!< This is the total accumulated time played in secs
};

/**
 * The pclean_criteria_data is set up in config.c and used in db.c to determine
 * the conditions which will cause a player character to be deleted from disk
 * if the automagic pwipe system is enabled (see config.c).
 */
struct pclean_criteria_data
{
    int level; ///!< PC level and below to check for deletion
    int days;  ///!< time limit in days, for this level of PC
};

/**
 * General info used by PC's and NPC's.
 */
struct char_player_data
{
    char passwd[MAX_PWD_LENGTH+1]; ///!< PC's password
    char *name;                    ///!< PC / NPC name
    char *short_descr;             ///!< NPC 'actions'
    char *long_descr;              ///!< PC / NPC look description
    char *description;             ///!< NPC Extra descriptions
    char *title;                   ///!< PC / NPC title
    byte sex;                      ///!< PC / NPC sex
    byte chclass;                  ///!< PC / NPC class
    byte level;                    ///!< PC / NPC level
    struct time_data time;         ///!< PC AGE in days
    ubyte weight;                  ///!< PC / NPC weight
    ubyte height;                  ///!< PC / NPC height
};

/**
 * Character abilities. Different instances of this structure are used for
 * both inherent and current ability scores (like when poison affects the
 * player strength).
 */
struct char_ability_data
{
    sbyte str;     //!< Strength.
    sbyte str_add; //!< Strength multiplier if str = 18. Usually from 0 to 100
    sbyte intel;   //!< Intelligence
    sbyte wis;     //!< Wisdom
    sbyte dex;     //!< Dexterity
    sbyte con;     //!< Constitution
    sbyte cha;     //!< Charisma
};

/**
 * Character 'points', or health statistics.
 */
struct char_point_data
{
    sh_int mana;     ///!< Current mana level
    sh_int max_mana; ///!< Max mana level
    sh_int hit;      ///!< Curent hit point, or health, level
    sh_int max_hit;  ///!< Max hit point, or health, level
    sh_int move;     ///!< Current move point, or stamina, level
    sh_int max_move; ///!< Max move point, or stamina, level

    // Current armor class. Internal use goes from -100 (totally armored) to
    // 100 (totally naked). Externally expressed as -10 (totally armored) to
    // 10 (totally naked). Currently follows the old and decrepit Advanced
    // Dungeons and Dragons method of dealing with character defense, or
    // Armor class.
    sh_int armor;
    int gold;        ///!< Current gold carried on character
    int bank_gold;   ///!< Gold the char has in a bank account
    int exp;         ///!< The experience points, or value, of the character.

    sbyte hitroll;   ///!< Any bonus or penalty to the hit roll
    sbyte damroll;   ///!< Any bonus or penalty to the damage roll
};

/**
 * char_special_data_saved: specials which both a PC and an NPC have in
 * common, but which must be saved to the players file for PC's.
 */
struct char_special_data_saved
{
    int alignment;                    ///!< -1000 (evil) to 1000 (good) range.
    long idnum;                        ///!< PC's idnum; -1 for mobiles.
    int act[PM_ARRAY_MAX];            ///!< act flags for NPC's; player flag for PC's
    int affected_by[AF_ARRAY_MAX];    ///!< Bitvector for spells/skills affected by
    sh_int apply_saving_throw[5];    ///!< Saving throw (Bonuses)
};

/**
 * Special playing constants shared by PCs and NPCs which aren't in pfile
 */
struct char_special_data
{
    struct char_data *fighting;                ///!< Target of fight; else NULL
    struct char_data *hunting;                ///!< Target of NPC hunt; else NULL
    struct obj_data *furniture;                ///!< Object being sat on/in; else NULL
    struct char_data *next_in_furniture;    ///!< Next person sitting, else NULL

    byte position;                            ///!< Standing, fighting, sleeping, etc.

    int carry_weight;                        ///!< Carried weight
    byte carry_items;                        ///!< Number of items carried
    int timer;                                ///!< Timer for update

    struct char_special_data_saved saved;    ///!< Constants saved for PCs.
};

/**
 * Data only needed by PCs, and needs to be saved to disk.
 */
struct player_special_data_saved
{
    byte skills[MAX_SKILLS+1];        ///!< Character skills.
    int wimp_level;                    ///!< Below this # of hit points, flee!
    byte freeze_level;                ///!< Level of god who froze char, if any
    sh_int invis_level;                ///!< level of invisibility
    room_vnum load_room;            ///!< Which room to load PC into
    int pref[PR_ARRAY_MAX];            ///!< preference flags
    ubyte bad_pws;                    ///!< number of bad login attempts
    sbyte conditions[3];            ///!< Drunk, hunger, and thirst
    struct txt_block *comm_hist[NUM_HIST]; ///!< Communication history
    ubyte page_length;                ///!< Max number of rows of text to send at once
    ubyte screen_width;                ///!< How wide the display page is
    int spells_to_learn;            ///!< Remaining number of practice sessions
    int olc_zone;                    ///!< Current olc permissions
    int questpoints;                ///!< Number of quest points earned
    qst_vnum *completed_quests;        ///!< Quests completed
    int    num_completed_quests;    ///!< Number completed
    int    current_quest;            ///!< vnum of current quest
    int    quest_time;                ///!< time left on current quest
    int    quest_counter;            ///!< Count of targets left to get
    time_t   lastmotd;                ///!< Last time player read motd
    time_t   lastnews;                ///!< Last time player read news
};

/**
 * Specials needed only by PCs, not NPCs.  Space for this structure is
 * not allocated in memory for NPCs, but it is for PCs and the portion
 * of it labelled 'saved' is saved in the players file.
 */
struct player_special_data
{
    struct player_special_data_saved saved; ///!< Information to be saved.

    char *poofin;                    ///!< Description displayed to room on arrival of a god.
    char *poofout;                    ///!< Description displayed to room at a god's exit.
    struct alias_data *aliases;        ///!< Command aliases
    long last_tell;                    ///!< idnum of PC who last told this PC, used to reply
    void *last_olc_targ;            ///!< ? Currently Unused ?
    int last_olc_mode;                ///!< ? Currently Unused ?
    char *host;                        ///!< Resolved hostname, or ip, for player.
};

/**
 * Special data used by NPCs, not PCs
 */
struct mob_special_data
{
    memory_rec *memory; ///!< List of PCs to remember
    byte attack_type;   ///!< The primary attack type (bite, sting, hit, etc.)
    byte default_pos;   ///!< Default position (standing, sleeping, etc.)
    byte damnodice;     ///!< The number of dice to roll for damage
    byte damsizedice;   ///!< The size of each die rolled for damage.
};

/**
 * An affect structure.
 */
struct affected_type
{
    sh_int type;                    ///!< The type of spell that caused this
    sh_int duration;                ///!< For how long its effects will last
    sbyte modifier;                    ///!< Added/subtracted to/from apropriate ability
    byte location;                    ///!< Tells which ability to change(APPLY_XXX).
    long /*bitvector_t*/bitvector;    ///!< Tells which bits to set (AFF_XXX).

    struct affected_type *next;        ///!< The next affect in the list of affects.
};

/**
 * The list element that makes up a list of characters following this
 * character.
 */
struct follow_type
{
    struct char_data *follower; ///!< Character directly following.
    struct follow_type *next;   ///!< Next character following.
};

/**
 * Master structure for PCs and NPCs.
 */
struct char_data
{
    int pfilepos;                            ///!< PC playerfile pos and id number
    mob_rnum nr;                            ///!< NPC real instance number
    room_rnum in_room;                        ///!< Current location (real room number)
    room_rnum was_in_room;                    ///!< Previous location for linkdead people
    int wait;                                ///!< wait for how many loops before taking action.

    struct char_player_data player;            ///!< General PC/NPC data
    Attributes real_abils;                    ///!< Abilities without modifiers
    Attributes aff_abils;                    ///!< Abilities with modifiers
    struct char_point_data points;            ///!< Point/statistics
    struct char_special_data char_specials;    ///!< PC/NPC specials
    struct player_special_data *player_specials; ///!< PC specials
    struct mob_special_data mob_specials;    ///!< NPC specials

    struct affected_type *affected;            ///!< affected by what spells
    struct obj_data *equipment[NUM_WEARS];    ///!< Equipment array

    struct obj_data *carrying;                ///!< List head for objects in inventory
    struct descriptor_data *desc;            ///!< Descriptor/connection info; NPCs = NULL

    long id;                                ///!< used by DG triggers - unique id
    struct trig_proto_list *proto_script;    ///!< list of default triggers
    struct script_data *script;                ///!< script info for the object
    struct script_memory *memory;            ///!< for mob memory triggers

    struct char_data *next_in_room;            ///!< Next PC in the room
    struct char_data *next;                    ///!< Next char_data in the room
    struct char_data *next_fighting;        ///!< Next in line to fight

    struct follow_type *followers;            ///!< List of characters following
    struct char_data *master;                ///!< List of character being followed

    long pref;                                ///!< unique session id
};

/**
 * descriptor-related structures
 */
struct txt_block
{
    char *text;             ///!< ?
    int aliased;            ///!< ?
    struct txt_block *next; ///!< ?
};

/**
 * ?
 */
struct txt_q
{
    struct txt_block *head; ///!< ?
    struct txt_block *tail; ///!< ?
};

/**
 * Master structure players. Holds the real players connection to the mud.
 * An analogy is the char_data is the body of the character, the descriptor_data
 * is the soul.
 */
struct descriptor_data
{
    socket_t descriptor;                ///!< file descriptor for socket
    char host[HOST_LENGTH+1];            ///!< hostname
    byte bad_pws;                        ///!< number of bad pw attemps this login
    byte idle_tics;                        ///!< tics idle at password prompt
    int connected;                        ///!< mode of 'connectedness'
    int desc_num;                        ///!< unique num assigned to desc
    time_t login_time;                    ///!< when the person connected
    char *showstr_head;                    ///!< for keeping track of an internal str
    char **showstr_vector;                ///!< for paging through texts
    int showstr_count;                    ///!< number of pages to page through
    int showstr_page;                    ///!< which page are we currently showing?
    char **str;                            ///!< for the modify-str system
    char *backstr;                        ///!< backup string for modify-str system
    size_t max_str;                        ///!< maximum size of string in modify-str
    long mail_to;                        ///!< name for mail system
    int has_prompt;                        ///!< is the user at a prompt?
    char inbuf[MAX_RAW_INPUT_LENGTH];    ///!< buffer for raw input
    char last_input[MAX_INPUT_LENGTH];    ///!< the last input
    char small_outbuf[SMALL_BUFSIZE];    ///!< standard output buffer
    char *output;                        ///!< ptr to the current output buffer
    char **history;                        ///!< History of commands, for ! mostly.
    int history_pos;                    ///!< Circular array position.
    int bufptr;                            ///!< ptr to end of current output
    int bufspace;                        ///!< space left in the output buffer
    struct txt_block *large_outbuf;        ///!< ptr to large buffer, if we need it
    struct txt_q input;                    ///!< q of unprocessed input
    struct char_data *character;        ///!< linked to char
    struct char_data *original;            ///!< original char if switched
    struct descriptor_data *snooping;    ///!< Who is this char snooping
    struct descriptor_data *snoop_by;    ///!< And who is snooping this char
    struct descriptor_data *next;        ///!< link to next descriptor
    struct oasis_olc_data *olc;            ///!< OLC info
};

// other miscellaneous structures
/**
 * Fight message display. This structure is used to hold the information to
 * be displayed for every different violent hit type.
 */
struct msg_type
{
    char *attacker_msg; ///!< Message displayed to attecker.
    char *victim_msg;   ///!< Message displayed to victim.
    char *room_msg;     ///!< Message displayed to rest of players in room.
};

/**
 * An entire message structure for a type of hit or spell or skill.
 */
struct message_type
{
    struct msg_type die_msg;   ///!< Messages for death strikes.
    struct msg_type miss_msg;  ///!< Messages for missed strikes.
    struct msg_type hit_msg;   ///!< Messages for a succesful strike.
    struct msg_type god_msg;   ///!< Messages when trying to hit a god.
    struct message_type *next; ///!< Next set of messages.
};

/**
 * Head of list of messages for an attack type.
 */
struct message_list
{
    int a_type;               ///!< The id of this attack type.
    int number_of_attacks;    ///!< How many attack messages to chose from.
    struct message_type *msg; ///!< List of messages.
};

/**
 * Social message data structure.
 */
struct social_messg
{
    int act_nr;                    ///!< The social id.
    char *command;                ///!< The command to activate (smile, wave, etc.)
    char *sort_as;                ///!< Priority of social sorted by this.
    int hide;                    ///!< If true, and target can't see actor, target doesn't see
    int min_victim_position;    ///!< Required Position of victim
    int min_char_position;        ///!< Required Position of char
    int min_level_char;            ///!< Minimum PC level required to use this social.

    // No argument was supplied
    char *char_no_arg;            ///!< Displayed to char when no argument is supplied
    char *others_no_arg;        ///!< Displayed to others when no arg is supplied

    // An argument was there, and a victim was found
    char *char_found;            ///!< Display to char when arg is supplied
    char *others_found;            ///!< Display to others when arg is supplied
    char *vict_found;            ///!< Display to target arg

    // An argument was there, as well as a body part, and a victim was found
    char *char_body_found;        ///!< Display to actor
    char *others_body_found;    ///!< Display to others
    char *vict_body_found;        ///!< Display to target argument

    // An argument was there, but no victim was found
    char *not_found;            ///!< Display when no victim is found

    // The victim turned out to be the character
    char *char_auto;            ///!< Display when self is supplied
    char *others_auto;            ///!< Display to others when self is supplied

    // If the char cant be found search the char's inven and do these:
    char *char_obj_found;        ///!< Social performed on object, display to char
    char *others_obj_found;        ///!< Social performed on object, display to others
};

/**
 * Describes bonuses, or negatives, applied to thieves skills. In practice
 * this list is tied to the character's dexterity attribute.
 */
struct dex_skill_type
{
    sh_int p_pocket; ///!< Alters the success rate of pick pockets
    sh_int p_locks;  ///!< Alters the success of pick locks
    sh_int traps;    ///!< Historically alters the success of trap finding.
    sh_int sneak;    ///!< Alters the success of sneaking without being detected
    sh_int hide;     ///!< Alters the success of hiding out of sight
};

/**
 * Describes the bonuses applied for a specific value of a character's
 * strength attribute.
 */
struct dex_app_type
{
    sh_int reaction; ///!< Historically affects reaction savings throws.
    sh_int miss_att; ///!< Historically affects missile attacks
    sh_int defensive; ///!< Alters character's inherent armor class
};

/**
 * Describes the bonuses applied for a specific value of a character's
 * strength attribute.
 */
struct str_app_type
{
    sh_int tohit;    ///!< To Hit (THAC0) Bonus/Penalty
    sh_int todam;    ///!< Damage Bonus/Penalty
    sh_int carry_w;    ///!< Maximum weight that can be carrried
    sh_int wield_w;    ///!< Maximum weight that can be wielded
};

/**
 * Describes the bonuses applied for a specific value of a character's
 * wisdom attribute.
 */
struct wis_app_type
{
    byte bonus;        ///!< how many practices player gains per lev
};

/**
 * Describes the bonuses applied for a specific value of a character's
 * intelligence attribute.
 */
struct int_app_type
{
    byte learn;        ///!< how many % a player learns a spell/skill
};

/**
 * Describes the bonuses applied for a specific value of a
 * character's constitution attribute.
 */
struct con_app_type
{
    sh_int hitp;    ///!< Added to a character's new MAXHP at each new level.
    sh_int shock;    ///!< Historically affects resurrection chances.
};

/**
 * Stores, and used to deliver, the current weather information
 * in the mud world.
 */
struct weather_data
{
    int pressure;    ///!< How is the pressure ( Mb )?
    int change;        ///!< How fast and what way does it change?
    int sky;        ///!< How is the sky?
    int sunlight;    ///!< And how much sun?
};

/**
 * Element in monster and object index-tables.
 * NOTE: Assumes sizeof(mob_vnum) >= sizeof(obj_vnum)
 */
struct index_data
{
    mob_vnum vnum;                ///!< virtual number of this mob/obj
    int number;                    ///!< number of existing units of this mob/obj
    // Point to any SPECIAL function assoicated with mob/obj.
    // Note: These are not trigger scripts. They are functions hard coded in
    // the source code.
    SPECIAL(*func);

    char *farg;                    ///!< String argument for special function.
    struct trig_data *proto;    ///!< Points to the trigger prototype.
};

/**
 * Master linked list for the mob/object prototype trigger lists.
 */
struct trig_proto_list
{
    int vnum;                        ///!< vnum of the trigger
    struct trig_proto_list *next;    ///!< next trigger
};

struct guild_info_type
{
    int pc_class;
    room_vnum guild_room;
    int direction;
};

// Config structs

/**
* The game configuration structure used for configurating the game play
* variables.
*/
struct game_data
{
    int pk_allowed;                ///!< Is player killing allowed?
    int pt_allowed;                ///!< Is player thieving allowed?
    int level_can_shout;        ///!< Level player must be to shout.
    int holler_move_cost;        ///!< Cost to holler in move points.
    int tunnel_size;            ///!< Number of people allowed in a tunnel.
    int max_exp_gain;            ///!< Maximum experience gainable per kill.
    int max_exp_loss;            ///!< Maximum experience losable per death.
    int max_npc_corpse_time;    ///!< Num tics before NPC corpses decompose
    int max_pc_corpse_time;        ///!< Num tics before PC corpse decomposes.
    int idle_void;                ///!< Num tics before PC sent to void(idle)
    int idle_rent_time;            ///!< Num tics before PC is autorented.
    int idle_max_level;            ///!< Level of players immune to idle.
    int dts_are_dumps;            ///!< Should items in dt's be junked?
    int load_into_inventory;    ///!< Objects load in immortals inventory.
    int track_through_doors;    ///!< Track through doors while closed?
    int no_mort_to_immort;        ///!< Prevent mortals leveling to imms?
    int disp_closed_doors;        ///!< Display closed doors in autoexit?
    int map_option;                ///!< MAP_ON, MAP_OFF or MAP_IMM_ONLY
    int map_size;                ///!< Default size for map command
    int minimap_size;            ///!< Default size for mini-map (automap)
    int script_players;       //!< Is attaching scripts to players allowed?

    char *OK;                    ///!< When player receives 'Okay.' text.
    char *NOPERSON;                ///!< 'No one by that name here.'
    char *NOEFFECT;                ///!< 'Nothing seems to happen.'
};

/**
 * The rent and crashsave options.
 */
struct crash_save_data
{
    int free_rent;    ///!< Should the MUD allow rent for free?
    int max_obj_save;            ///!< Max items players can rent.
    int min_rent_cost;        ///!< surcharge on top of item costs.
    int auto_save;            ///!< Does the game automatically save ppl?
    int autosave_time;        ///!< if auto_save=TRUE, how often?
    int crash_file_timeout;    ///!< Life of crashfiles and idlesaves.
    int rent_file_timeout;    ///!< Lifetime of normal rent files in days
};

/**
 * Important room numbers. This structure stores vnums, not real array
 * numbers.
 */
struct room_numbers
{
    room_vnum mortal_start_room;    ///!< vnum of room that mortals enter at.
    room_vnum immort_start_room;    ///!< vnum of room that immorts enter at.
    room_vnum frozen_start_room;    ///!< vnum of room that frozen ppl enter.
    room_vnum donation_room_1;    ///!< vnum of donation room #1.
    room_vnum donation_room_2;    ///!< vnum of donation room #2.
    room_vnum donation_room_3;    ///!< vnum of donation room #3.
};

/**
 * Operational game variables.
 */
struct game_operation
{
    ush_int DFLT_PORT;        ///!< The default port to run the game.
    char *DFLT_IP;            ///!< Bind to all interfaces.
    char *DFLT_DIR;            ///!< The default directory (lib).
    char *LOGNAME;            ///!< The file to log messages to.
    int max_playing;            ///!< Maximum number of players allowed.
    int max_filesize;            ///!< Maximum size of misc files.
    int max_bad_pws;            ///!< Maximum number of pword attempts.
    int siteok_everyone;        ///!< Everyone from all sites are SITEOK.
    int nameserver_is_slow;    ///!< Is the nameserver slow or fast?
    int use_new_socials;        ///!< Use new or old socials file ?
    int auto_save_olc;        ///!< Does OLC save to disk right away ?
    char *MENU;                ///!< The MAIN MENU.
    char *WELC_MESSG;            ///!< The welcome message.
    char *START_MESSG;        ///!< The start msg for new characters.
    int medit_advanced;       //!< Does the medit OLC show the advanced stats menu ?
};

/**
 * The Autowizard options.
 */
struct autowiz_data
{
    int use_autowiz;        ///!< Use the autowiz feature?
    int min_wizlist_lev;    ///!< Minimun level to show on wizlist.
};

/**
 * Main Game Configuration Structure.
 * Global variables that can be changed within the game are held within this
 * structure. During gameplay, elements within this structure can be altered,
 * thus affecting the gameplay immediately, and avoiding the need to recompile
 * the code.
 * If changes are made to values of the elements of this structure during game
 * play, the information will be saved to disk.
 */
struct config_data
{
    char *CONFFILE;                        ///!< Path to on-disk file where the config_data structure gets written.
    struct game_data play;                ///!< In-game specific global settings, such as allowing player killing.
    struct crash_save_data csd;            ///!< How is renting, crash files, and object saving handled?
    struct room_numbers room_nums;        ///!< Special designated rooms, like start rooms, and donation rooms.
    struct game_operation operation;    ///!< Basic operational settings, like max file sizes and max players.
    struct autowiz_data autowiz;        ///!< Autowiz specific settings, like turning it on and minimum level
};

#ifdef MEMORY_DEBUG
#include "zmalloc.h"
#endif

#endif // _STRUCTS_H_
