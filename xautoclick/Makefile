# 
# xAutoClick
#
# Copyright (C) 2006, 2010 Ivo van Poorten
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#

include config.mak

OBJScommon	= main.o \
			  osdep.o \

PRGgtk1		= gautoclick
OBJSgtk1	= $(OBJScommon) \
			  guigtk1.o \

PRGgtk2		= gautoclick2
OBJSgtk2	= $(OBJScommon) \
			  guigtk2.o \

PRGascii	= aautoclick
OBJSascii	= $(OBJScommon) \
			  guiascii.o \

PRGqt3		= qtautoclick
OBJSqt3		= $(OBJScommon) \
			  guiqt3.o \

PRGqt4		= qt4autoclick
OBJSqt4		= $(OBJScommon) \
			  guiqt4.o \
			  guiqt4-moc.o \

PRGfltk		= fltkautoclick
OBJSfltk	= $(OBJScommon) \
			  guifltk.o \

PRGcmdl		= cautoclick
OBJScmdl	= $(OBJScommon) \
			  guicommandline.o \

ifeq ($(__BUILD_GTK1__),yes)
INCLUDES	+= $(INCLUDES_GTK1)
LDFLAGS		+= $(LDFLAGS_GTK1)
endif

ifeq ($(__BUILD_GTK2__),yes)
INCLUDES	+= $(INCLUDES_GTK2)
LDFLAGS		+= $(LDFLAGS_GTK2)
endif

ifeq ($(__BUILD_ASCII__),yes)
INCLUDES	+= $(INCLUDES_ASCII)
LDFLAGS		+= $(LDFLAGS_ASCII)
endif

ifeq ($(__BUILD_QT3__),yes)
INCLUDES	+= $(INCLUDES_QT3)
LDFLAGS		+= $(LDFLAGS_QT3)
endif

ifeq ($(__BUILD_QT4__),yes)
INCLUDES	+= $(INCLUDES_QT4)
LDFLAGS		+= $(LDFLAGS_QT4)
endif

ifeq ($(__BUILD_FLTK__),yes)
INCLUDES	+= $(INCLUDES_FLTK)
LDFLAGS		+= $(LDFLAGS_FLTK) -lstdc++
endif

ifeq ($(__BUILD_COMMANDLINE__),yes)
INCLUDES	+= $(INCLUDES_COMMANDLINE)
LDFLAGS		+= $(LDFLAGS_COMMANDLINE)
endif

.PHONY:	all
all:
ifeq ($(HAVE_GTK1),yes)
	__BUILD_GTK1__=yes $(MAKE) -C . $(PRGgtk1)
endif
ifeq ($(HAVE_GTK2),yes)
	__BUILD_GTK2__=yes $(MAKE) -C . $(PRGgtk2)
endif
ifeq ($(HAVE_ASCII),yes)
	__BUILD_ASCII__=yes $(MAKE) -C . $(PRGascii)
endif
ifeq ($(HAVE_QT3),yes)
	__BUILD_QT3__=yes $(MAKE) -C . $(PRGqt3)
endif
ifeq ($(HAVE_QT4),yes)
	__BUILD_QT4__=yes $(MAKE) -C . $(PRGqt4)
endif
ifeq ($(HAVE_FLTK),yes)
	__BUILD_FLTK__=yes $(MAKE) -C . $(PRGfltk)
endif
ifeq ($(HAVE_COMMANDLINE),yes)
	__BUILD_COMMANDLINE__=yes $(MAKE) -C . $(PRGcmdl)
endif

$(PRGgtk1):		$(OBJSgtk1)
	$(CC) -o $@ $(OBJSgtk1) $(LDFLAGS)

$(PRGgtk2):		$(OBJSgtk2)
	$(CC) -o $@ $(OBJSgtk2) $(LDFLAGS)

$(PRGascii):	$(OBJSascii)
	$(CC) -o $@ $(OBJSascii) $(LDFLAGS)

$(PRGqt3):		$(OBJSqt3)
	$(CXX) -o $@ $(OBJSqt3) $(LDFLAGS)

$(PRGqt4):		$(OBJSqt4)
	$(CXX) -o $@ $(OBJSqt4) $(LDFLAGS)

$(PRGfltk):		$(OBJSfltk)
	$(CXX) -o $@ $(OBJSfltk) $(LDFLAGS)

$(PRGcmdl):		$(OBJScmdl)
	$(CC) -o $@ $(OBJScmdl) $(LDFLAGS)

guigtk2.c:
	ln -s guigtk1.c guigtk2.c

guiqt4-moc.cpp:	guiqt4.h
	moc-qt4 -oguiqt4-moc.cpp guiqt4.h

#strip:	$(PRG)
#	strip $(PRG)

config.mak:		configure
	@echo
	@echo "#############################"
	@echo "Please run ./configure first!"
	@echo "#############################"
	@echo
	@echo "If you have done so before, you have to do it again, because it"
	@echo "has changed."
	@echo
	@exit 1

ifneq ($(OBJSgtk1),)
-include $(patsubst %,.deps/%,$(OBJSgtk1:.o=.d))
endif

ifneq ($(OBJSgtk2),)
-include $(patsubst %,.deps/%,$(OBJSgtk2:.o=.d))
endif

ifneq ($(OBJSascii),)
-include $(patsubst %,.deps/%,$(OBJSascii:.o=.d))
endif

ifneq ($(OBJSqt3),)
-include $(patsubst %,.deps/%,$(OBJSqt3:.o=.d))
endif

ifneq ($(OBJSqt4),)
-include $(patsubst %,.deps/%,$(OBJSqt4:.o=.d))
endif

ifneq ($(OBJSfltk),)
-include $(patsubst %,.deps/%,$(OBJSfltk:.o=.d))
endif

SILENCE = @

%.o:	%.c
	$(CC) -c $(CFLAGS) $(INCLUDES) $*.c -o $*.o $(DEBUG) $(WARNINGS)
	mkdir -p .deps
	$(CCFORDEPS) $(CFLAGS) $(INCLUDES) $*.c 1>.deps/$*.d

%.o:	%.cpp
	$(CXX) -c $(CFLAGS) $(INCLUDES) $*.cpp -o $*.o $(DEBUG) $(WARNINGS)
	mkdir -p .deps
	$(CXXFORDEPS) $(CFLAGS) $(INCLUDES) $*.cpp 1>.deps/$*.d

.PHONY:	clean
clean:
	rm -f *.o $(PRGgtk1) $(PRGgtk2) $(PRGqt3) $(PRGascii) $(PRGcmdl) guigtk2.c
	rm -f guiqt4-moc.cpp $(PRGqt4) $(PRGfltk)
	rm -rf .deps

.PHONY:	distclean
distclean:		clean
	rm -f config.mak configure.log
	rm -f *~

.PHONY:	install
install:		all
	@echo Installing to $(PREFIX)
	if test ! -d $(PREFIX)/bin ; then mkdir -p $(PREFIX)/bin ; fi
	if test ! -d $(PREFIX)/man/man1 ; then mkdir -p $(PREFIX)/man/man1 ; fi
ifeq ($(HAVE_GTK1),yes)
	cp $(PRGgtk1) $(PREFIX)/bin
endif
ifeq ($(HAVE_GTK2),yes)
	cp $(PRGgtk2) $(PREFIX)/bin
endif
ifeq ($(HAVE_QT3),yes)
	cp $(PRGqt3) $(PREFIX)/bin
endif
ifeq ($(HAVE_QT4),yes)
	cp $(PRGqt4) $(PREFIX)/bin
endif
ifeq ($(HAVE_FLTK),yes)
	cp $(PRGfltk) $(PREFIX)/bin
endif
ifeq ($(HAVE_ASCII),yes)
	cp $(PRGascii) $(PREFIX)/bin
endif
ifeq ($(HAVE_COMMANDLINE),yes)
	cp $(PRGcmdl) $(PREFIX)/bin
endif
	cp xautoclick.1 $(PREFIX)/man/man1

.PHONY: uninstall
uninstall:
	@echo Uninstalling from $(PREFIX)
	rm -f $(PREFIX)/bin/$(PRGgtk1)
	rm -f $(PREFIX)/bin/$(PRGgtk2)
	rm -f $(PREFIX)/bin/$(PRGqt3)
	rm -f $(PREFIX)/bin/$(PRGqt4)
	rm -f $(PREFIX)/bin/$(PRGfltk)
	rm -f $(PREFIX)/bin/$(PRGascii)
	rm -f $(PREFIX)/bin/$(PRGcmdl)
	rm -f $(PREFIX)/man/man1/xautoclick.1

