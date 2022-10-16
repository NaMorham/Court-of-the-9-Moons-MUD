# Generated automatically from Makefile.in by configure.
# tbaMUD Makefile.in - Makefile template used by 'configure'
# Clean-up provided by seqwith.

# C compiler to use
CC = g++

# Any special flags you want to pass to the compiler
MYFLAGS = -Wall -Wno-char-subscripts -Wno-unused-but-set-variable

#flags for profiling (see hacker.doc for more information)
PROFILE = 

##############################################################################
# Do Not Modify Anything Below This Line (unless you know what you're doing) #
##############################################################################

BINDIR = ../bin

CFLAGS = -g -O2 $(MYFLAGS) $(PROFILE)

LIBS =  -lcrypt 

SRCFILES := act.comm.cpp act.informative.cpp act.item.cpp act.movement.cpp act.offensive.cpp act.other.cpp act.social.cpp act.wizard.cpp aedit.cpp asciimap.cpp ban.cpp boards.cpp bsd-snprintf.cpp castle.cpp cedit.cpp class.cpp CharacterAttributes.cpp comm.cpp config.cpp constants.cpp db.cpp dg_comm.cpp dg_db_scripts.cpp dg_event.cpp dg_handler.cpp dg_misc.cpp dg_mobcmd.cpp dg_objcmd.cpp dg_olc.cpp dg_scripts.cpp dg_triggers.cpp dg_variables.cpp dg_wldcmd.cpp fight.cpp genmob.cpp genobj.cpp genolc.cpp genqst.cpp genshp.cpp genwld.cpp genzon.cpp graph.cpp handler.cpp hedit.cpp house.cpp ibt.cpp improved-edit.cpp interpreter.cpp limits.cpp lists.cpp magic.cpp mail.cpp medit.cpp mobact.cpp modify.cpp msgedit.cpp mud_event.cpp oasis.cpp oasis_copy.cpp oasis_delete.cpp oasis_list.cpp objsave.cpp oedit.cpp players.cpp prefedit.cpp protocol.cpp qedit.cpp quest.cpp race.cpp random.cpp redit.cpp sedit.cpp shop.cpp spec_assign.cpp spec_procs.cpp spell_parser.cpp spells.cpp tedit.cpp utils.cpp weather.cpp zedit.cpp zmalloc.cpp
OBJFILES := act.comm.o act.informative.o act.item.o act.movement.o act.offensive.o act.other.o act.social.o act.wizard.o aedit.o asciimap.o ban.o boards.o bsd-snprintf.o castle.o cedit.o class.o CharacterAttributes.o comm.o config.o constants.o db.o dg_comm.o dg_db_scripts.o dg_event.o dg_handler.o dg_misc.o dg_mobcmd.o dg_objcmd.o dg_olc.o dg_scripts.o dg_triggers.o dg_variables.o dg_wldcmd.o fight.o genmob.o genobj.o genolc.o genqst.o genshp.o genwld.o genzon.o graph.o handler.o hedit.o house.o ibt.o improved-edit.o interpreter.o limits.o lists.o magic.o mail.o medit.o mobact.o modify.o msgedit.o mud_event.o oasis.o oasis_copy.o oasis_delete.o oasis_list.o objsave.o oedit.o players.o prefedit.o protocol.o qedit.o quest.o race.o random.o redit.o sedit.o shop.o spec_assign.o spec_procs.o spell_parser.o spells.o tedit.o utils.o weather.o zedit.o zmalloc.o


default: all

all: .accepted
	$(MAKE) $(BINDIR)/circle
#	$(MAKE) utils

.accepted:
	@./licheck less

#utils: .accepted
#	(cd util; $(MAKE) all)

circle:
	$(MAKE) $(BINDIR)/circle

$(BINDIR)/circle : $(OBJFILES)
	$(CC) -o $(BINDIR)/circle $(PROFILE) $(OBJFILES) $(LIBS)

$%.o: %.c
	$(CC) $< $(CFLAGS) -c -o $@ 

clean:
	rm -f *.o depend

# Dependencies for the object files (automagically generated with
# gcc -MM)

depend:
	$(CC) -MM *.cpp > depend

-include depend

