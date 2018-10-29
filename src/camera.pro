TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    spi/spi.c \
    spictrl.cpp \
    sensorctrl.cpp \
    eepromctrl.cpp \
    common.cpp \
    calc.cpp \
    sockethelper.cpp \
    socketserver.cpp \
    myprotocol.cpp \
    CJsonObject/CJsonObject.cpp \
    CJsonObject/cJSON.c \
    jsonhelper.cpp \
    socketclient.cpp \
    temprule.cpp \
    myqueue.cpp \
    sharedspace.cpp \
    sqlhelper.cpp

HEADERS += \
    ./include/spi.h \
    include/spictrl.h \
    include/sensorctrl.h \
    include/eepromctrl.h \
    include/common.h \
    include/calc.h \
    include/sockethelper.h \
    include/CJsonObject.hpp \
    include/cJSON.h \
    include/socketserver.h \
    include/myprotocol.h \
    include/jsonhelper.h \
    include/transport.h \
    include/socketclient.h \
    include/temprule.h \
    include/myqueue.h \
    include/sharedspace.h \
    include/sqlite3.h \
    include/sqlhelper.h \
    include/memwatch.h

INCLUDEPATH = ./include
LIBS += -lsqlite -lpthread
