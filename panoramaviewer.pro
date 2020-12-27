######################################################################
# Automatically generated by qmake (2.01a) Tue Oct 6 19:48:14 2009
# but modified by me to accomodate for the includes for qwt, hamlib and
# portaudio
######################################################################

TEMPLATE	= app
TARGET		= panoramaViewer
QT		+= widgets
#QMAKE_CFLAGS    +=  -O3 -ffast-math
#QMAKE_CXXFLAGS  +=  -O3 -ffast-math
QMAKE_CFLAGS   +=  -g
QMAKE_CXXFLAGS +=  -g
QMAKE_LFLAGS   +=  -g
QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]
RC_ICONS        =  viewer.ico
RESOURCES       += resources.qrc

TRANSLATIONS = i18n/de_DE.ts i18n/it_IT.ts i18n/hu_HU.ts

DEPENDPATH += . \
	      ./src \
	      ./devices

INCLUDEPATH += . \
	      ./src \
	      ./devices

# Input
HEADERS += ./panorama-viewer.h \
	   ./panorama-constants.h \
           ./src/ringbuffer.h \
	   ./src/scope.h \
	   ./src/processor.h \
	   ./devices/device-handler.h

FORMS += ./scopewidget.ui 

SOURCES += ./main.cpp \
           ./panorama-viewer.cpp \
	   ./src/scope.cpp \
	   ./src/processor.cpp \
	   ./devices/device-handler.cpp 

#for Fedora and Ubuntu use
unix { 
CONFIG		+= console 
DESTDIR		= ./linux-bin
exists ("./.git") {
   GITHASHSTRING = $$system(git rev-parse --short HEAD)
   !isEmpty(GITHASHSTRING) {
       message("Current git hash = $$GITHASHSTRING")
       DEFINES += GITHASH=\\\"$$GITHASHSTRING\\\"
   }
}
isEmpty(GITHASHSTRING) {
    DEFINES += GITHASH=\\\"------\\\"
}

INCLUDEPATH	+= /usr/include/qt5/qwt
INCLUDEPATH	+= /usr/local/include
#LIBS		+= -lqwt -lusb-1.0 -lrt -lfftw3f -ldl		# ubuntu 15.04
LIBS		+= -lqwt-qt5 -lusb-1.0 -lrt -lfftw3f -ldl	# fedora 25
LIBS            += -lsndfile
LIBS            += -lsamplerate

CONFIG		+= sdrplay
CONFIG		+= sdrplay-v3
CONFIG		+= elad-s1
CONFIG		+= colibri
#CONFIG		+= rtlsdr
}

## and for windows32 we use:
win32 {
CONFIG		-= console 
DESTDIR		= /usr/shared/w32-programs/windows-panoramaviewer

exists ("./.git") {
   GITHASHSTRING = $$system(git rev-parse --short HEAD)
   !isEmpty(GITHASHSTRING) {
       message("Current git hash = $$GITHASHSTRING")
       DEFINES += GITHASH=\\\"$$GITHASHSTRING\\\"
   }
}

isEmpty(GITHASHSTRING) {
    DEFINES += GITHASH=\\\"------\\\"
}

# includes in mingw differ from the includes in fedora linux
INCLUDEPATH 	+= /usr/i686-w64-mingw32/sys-root/mingw/include
INCLUDEPATH 	+= /usr/i686-w64-mingw32/sys-root/mingw/include/qt5/qwt
LIBS		+= -lfftw3f
LIBS		+= -lportaudio		# for the cardreader
LIBS		+= -lqwt-qt5
#LIBS		+= -lsndfile
LIBS		+= -lsamplerate
LIBS		+= -lole32
LIBS		+= -lwinmm
LIBS 		+= -lstdc++
LIBS		+= -lusb-1.0
LIBS		+= -lpthread
CONFIG		+= sdrplay
CONFIG		+= sdrplay-v3
CONFIG		+= extio
CONFIG		+= colibri
}
#
#	the devices
#
#	the SDRplay
#
sdrplay {
DEFINES		+= HAVE_SDRPLAY
	INCLUDEPATH	+= ./devices/sdrplay-handler
	HEADERS		+= ./devices/sdrplay-handler/sdrplay-handler.h \
	                   ./devices/sdrplay-handler/sdrplayselect.h
	SOURCES		+= ./devices/sdrplay-handler/sdrplay-handler.cpp \
	                   ./devices/sdrplay-handler/sdrplayselect.cpp
	FORMS		+= ./devices/sdrplay-handler/sdrplay-widget.ui
}

#
sdrplay-v3 {
        DEFINES         += HAVE_SDRPLAY_V3
        DEPENDPATH      += ./devices/sdrplay-handler-v3
        INCLUDEPATH     += ./devices/sdrplay-handler-v3 \
                           ./devices/sdrplay-handler-v3/include
        HEADERS         += ./devices/sdrplay-handler-v3/sdrplay-handler-v3.h \
                           ./devices/sdrplay-handler-v3/sdrplay-commands.h
        SOURCES         += ./devices/sdrplay-handler-v3/sdrplay-handler-v3.cpp
        FORMS           += ./devices/sdrplay-handler-v3/sdrplay-widget-v3.ui
}
#

elad-s1 {
	DEFINES		+= HAVE_ELAD_S1
	INCLUDEPATH	+= ./devices/sw-elad-s1
	HEADERS		+= ./devices/sw-elad-s1/elad-s1.h \
	                   ./devices/sw-elad-s1/elad-worker.h \
	                   ./devices/sw-elad-s1/elad-loader.h
	SOURCES		+= ./devices/sw-elad-s1/elad-s1.cpp \
	                   ./devices/sw-elad-s1/elad-worker.cpp \
	                   ./devices/sw-elad-s1/elad-loader.cpp
	FORMS		+= ./devices/sw-elad-s1/elad-widget.ui
}

soundcard {
	DEFINES		+= HAVE_SOUNDCARD
	INCLUDEPATH	+= ./devices/soundcard
	HEADERS		+= ./devices/soundcard/pa-reader.h \
	                   ./devices/soundcard/soundcard.h
	SOURCES		+= ./devices/soundcard/pa-reader.cpp \
	                   ./devices/soundcard/soundcard.cpp
	FORMS		+= ./devices/soundcard/soundcard-widget.ui
	LIBS		+= -lportaudio
}

#
#       the HACKRF One
#
hackrf {
        DEFINES         += HAVE_HACKRF
        DEPENDPATH      += ./devices/hackrf-handler
        INCLUDEPATH     += ./devices/hackrf-handler
        HEADERS         += ./devices/hackrf-handler/hackrf-handler.h
        SOURCES         += ./devices/hackrf-handler/hackrf-handler.cpp
        FORMS           += ./devices/hackrf-handler/hackrf-widget.ui
}

#
#       the HACKRF One
#
lime {
        DEFINES         += HAVE_LIME
        DEPENDPATH      += ./devices/lime-handler
        INCLUDEPATH     += ./devices/lime-handler
        HEADERS         += ./devices/lime-handler/lime-handler.h \
       	                   ./devices/lime-handler/lime-reader.h
        SOURCES         += ./devices/lime-handler/lime-handler.cpp \
	                   ./devices/lime-handler/lime-reader.cpp
        FORMS           += ./devices/lime-handler/lime-widget.ui
}

colibri	{
	DEFINES		+= HAVE_COLIBRI
        DEPENDPATH      += ./devices/colibri-handler
        INCLUDEPATH     += ./devices/colibri-handler
        HEADERS         += ./devices/colibri-handler/colibri-handler.h 
        SOURCES         += ./devices/colibri-handler/colibri-handler.cpp 
        FORMS           += ./devices/colibri-handler/colibri-widget.ui
}

rtlsdr {
	DEFINES		+= HAVE_RTLSDR
	INCLUDEPATH	+= ./devices/rtlsdr-handler
	HEADERS		+= ./devices/rtlsdr-handler/rtlsdr-handler.h \
	                   ./devices/rtlsdr-handler/dongleselect.h
	SOURCES		+= ./devices/rtlsdr-handler/rtlsdr-handler.cpp \
	                   ./devices/rtlsdr-handler/dongleselect.cpp
	FORMS		+= ./devices/rtlsdr-handler/dabstick-widget.ui
}
