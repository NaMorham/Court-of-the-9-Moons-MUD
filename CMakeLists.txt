cmake_minimum_required(VERSION 2.8)

project(ninemoons)

#message(STATUS "DBG: CMAKE_CACHEFILE_DIR = " ${CMAKE_CACHEFILE_DIR})
#message(STATUS "DBG: CMAKE_BINARY_DIR = " ${CMAKE_BINARY_DIR})

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    if(NOT CMAKE_CACHEFILE_DIR)
        set(CMAKE_CACHEFILE_DIR ${CMAKE_BINARY_DIR})
    endif(NOT CMAKE_CACHEFILE_DIR)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_CACHEFILE_DIR}/dist/ CACHE PATH "default install dir" FORCE)
endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

#message(STATUS "DBG: CMAKE_INSTALL_PREFIX = " ${CMAKE_INSTALL_PREFIX})

#include(cmake_vars.log.cmake)

set(CMAKE_USE_RELATIVE_PATHS "ON")
set(NO_UNICODE "ON")
#set(CMAKE_CXX_FLAGS "-std=c++11")

set(HDR_FILES
    src/act.h
    src/asciimap.h
    src/ban.h
    src/boards.h
    src/bsd-snprintf.h
    src/CharacterAttributes.h
    src/class.h
    src/comm.h
    src/conf.h
    src/config.h
    src/constants.h
    src/db.h
    src/dg_event.h
    src/dg_olc.h
    src/dg_scripts.h
    src/fight.h
    src/genmob.h
    src/genobj.h
    src/genolc.h
    src/genshp.h
    src/genwld.h
    src/genzon.h
    src/graph.h
    src/handler.h
    src/hedit.h
    src/house.h
    src/ibt.h
    src/improved-edit.h
    src/interpreter.h
    src/mail.h
    src/modify.h
    src/mudtypes.h
    src/oasis.h
    src/pfdefaults.h
    src/prefedit.h
    src/quest.h
    src/race.h
    src/screen.h
    src/shop.h
    src/spec_procs.h
    src/spells.h
    src/structs.h
    src/sysdep.h
    src/telnet.h
    src/trackdata.h
    src/utils.h
    src/zmalloc.h
)

set(CPP_FILES
    src/CharacterAttributes.cpp
    src/act.comm.cpp
    src/act.informative.cpp
    src/act.item.cpp
    src/act.movement.cpp
    src/act.offensive.cpp
    src/act.other.cpp
    src/act.social.cpp
    src/act.wizard.cpp
    src/aedit.cpp
    src/asciimap.cpp
    src/ban.cpp
    src/boards.cpp
    src/bsd-snprintf.cpp
    src/castle.cpp
    src/cedit.cpp
    src/class.cpp
    src/comm.cpp
    src/config.cpp
    src/constants.cpp
    src/db.cpp
    src/dg_comm.cpp
    src/dg_db_scripts.cpp
    src/dg_event.cpp
    src/dg_handler.cpp
    src/dg_misc.cpp
    src/dg_mobcmd.cpp
    src/dg_objcmd.cpp
    src/dg_olc.cpp
    src/dg_scripts.cpp
    src/dg_triggers.cpp
    src/dg_variables.cpp
    src/dg_wldcmd.cpp
    src/fight.cpp
    src/genmob.cpp
    src/genobj.cpp
    src/genolc.cpp
    src/genqst.cpp
    src/genshp.cpp
    src/genwld.cpp
    src/genzon.cpp
    src/graph.cpp
    src/handler.cpp
    src/hedit.cpp
    src/house.cpp
    src/ibt.cpp
    src/improved-edit.cpp
    src/interpreter.cpp
    src/limits.cpp
    src/magic.cpp
    src/mail.cpp
    src/medit.cpp
    src/mobact.cpp
    src/modify.cpp
    src/oasis.cpp
    src/oasis_copy.cpp
    src/oasis_delete.cpp
    src/oasis_list.cpp
    src/objsave.cpp
    src/oedit.cpp
    src/players.cpp
    src/prefedit.cpp
    src/qedit.cpp
    src/quest.cpp
    src/race.cpp
    src/random.cpp
    src/redit.cpp
    src/sedit.cpp
    src/shop.cpp
    src/spec_assign.cpp
    src/spec_procs.cpp
    src/spell_parser.cpp
    src/spells.cpp
    src/tedit.cpp
    src/trackdata.cpp
    src/utils.cpp
    src/weather.cpp
    src/zedit.cpp
    src/zmalloc.cpp
)

include_directories(
    src/
)

add_executable( ${PROJECT_NAME} ${CPP_FILES} ${HDR_FILES} )

install(TARGETS ${PROJECT_NAME} DESTINATION bin )


#
# TODO: Utils
#



#
# TODO: Tests would be good too
#

#set(TEST_FILES
#    test/place_only.t
#    test/back_to_start.t
#    test/no_commands.t
#    test/place_and_move.t
#    test/place_move_turn.t
#)

#install(FILES ${TEST_FILES} DESTINATION bin )

#enable_testing()

#add_test(PlaceOnly cram -v ${CMAKE_CACHEFILE_DIR}/dist/bin/place_only.t)
#add_test(PlaceAndMove cram -v ${CMAKE_CACHEFILE_DIR}/dist/bin/place_and_move.t)
#add_test(PlaceMoveTurn cram -v ${CMAKE_CACHEFILE_DIR}/dist/bin/place_move_turn.t)
#add_test(NoCommands cram -v ${CMAKE_CACHEFILE_DIR}/dist/bin/no_commands.t)
#add_test(BackToStart cram -v ${CMAKE_CACHEFILE_DIR}/dist/bin/back_to_start.t)
