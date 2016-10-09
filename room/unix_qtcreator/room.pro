TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    ../sources/main.cpp \
    ../sources/buffer.cpp \
    ../sources/log.cpp \
    ../sources/server.cpp \
    ../sources/netacceptor.cpp \
    ../sources/netconnection.cpp \
    ../sources/player.cpp \
    ../sources/table.cpp \
    ../sources/chair.cpp \
    ../sources/pool.cpp

DISTFILES += \
    ../sources/setting.xml

HEADERS += \
    ../sources/buffer.h \
    ../sources/common.h \
    ../sources/igamelogic.h \
    ../sources/itable.h \
    ../sources/log.h \
    ../sources/nmdefs.h \
    ../sources/platform.h \
    ../sources/server.h \
    ../sources/netacceptor.h \
    ../sources/netconnection.h \
    ../sources/player.h \
    ../sources/table.h \
    ../sources/playerdata.h \
    ../sources/chair.h \
    ../sources/pool.h

unix: LIBS +=   -lboost_system \
                -lboost_thread \
                -lboost_log \
                -lboost_log_setup \
                -lpthread
