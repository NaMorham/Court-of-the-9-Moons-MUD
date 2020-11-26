/**************************************************************************
*  File: oasis_delete.c                                    Part of tbaMUD *
*  Usage: Oasis OLC deletion.                                             *
*                                                                         *
* By Levork. Copyright 1996 Harvey Gilpin. 1997-2001 George Greer.        *
* 2002 Kip Potter [Mythran].                                              *
**************************************************************************/

#include "conf.h"
#include "sysdep.h"
#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "interpreter.h"
#include "handler.h"
#include "db.h"
#include "genolc.h"
#include "oasis.h"
#include "improved-edit.h"


/*
 * Free's strings from any object, room, mobiles, or player. TRUE if successful,
 * otherwise, it returns FALSE. Type - The OLC type constant relating to the
 * data type of data.
 */
int free_strings(void *data, int type)
{
    struct room_data *room;
    struct config_data *config;
    int i;

    switch (type) {
    case OASIS_WLD:
        room = (struct room_data *) data;

        // Free Descriptions
        if (room->name) {
            free(room->name);
            room->name = NULL;
        }
        if (room->description) {
            free(room->description);
            room->description = NULL;
        }
        if (room->ex_description) {
            free_ex_descriptions(room->ex_description);
            room->ex_description = NULL;
        }

        // Return the return value of free_strings().
        return (free_strings(room, OASIS_EXI));

    case OASIS_EXI:
        room = (struct room_data *) data;

        for (i = 0; i < NUM_OF_DIRS; i++) {
            if (room->dir_option[i]) {
                if (room->dir_option[i]->general_description) {
                    free(room->dir_option[i]->general_description);
                    room->dir_option[i]->general_description = NULL;
                }
                if (room->dir_option[i]->keyword) {
                    free(room->dir_option[i]->keyword);
                    room->dir_option[i]->keyword = NULL;
                }
                free(room->dir_option[i]);
                room->dir_option[i] = NULL;
            }
        }

        return (TRUE);

    case OASIS_MOB:
    case OASIS_OBJ:
        return (FALSE);     // For now...

    case OASIS_CFG:
        config = (struct config_data *) data;

        if (config->play.OK) {
            free(config->play.OK);
            config->play.OK = NULL;
        }
        if (config->play.NOPERSON) {
            free(config->play.NOPERSON);
            config->play.NOPERSON = NULL;
        }
        if (config->play.NOEFFECT) {
            free(config->play.NOEFFECT);
            config->play.NOEFFECT = NULL;
        }
        if (config->operation.DFLT_IP) {
            free(config->operation.DFLT_IP);
            config->operation.DFLT_IP = NULL;
        }
        if (config->operation.DFLT_DIR) {
            free(config->operation.DFLT_DIR);
            config->operation.DFLT_DIR = NULL;
        }
        if (config->operation.LOGNAME) {
            free(config->operation.LOGNAME);
            config->operation.LOGNAME = NULL;
        }
        if (config->operation.MENU) {
            free(config->operation.MENU);
            config->operation.MENU = NULL;
        }
        if (config->operation.WELC_MESSG) {
            free(config->operation.WELC_MESSG);
            config->operation.WELC_MESSG = NULL;
        }
        if (config->operation.START_MESSG) {
            free(config->operation.START_MESSG);
            config->operation.START_MESSG = NULL;
        }
        return (TRUE);

    default:
        mudlog(BRF, LVL_GOD, TRUE, "SYSERR: oasis_delete.c: free_strings: Invalid type handled (Type %d).", type);
        return (FALSE);
    }
}
