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
    socketclient.cpp \
    temprule.cpp \
    sharedspace.cpp \
    sqlhelper.cpp \
    jsoncpp.cpp \
    serialport.cpp \
    ipset.cpp \
    httprequest.cpp \
    threadPool.cpp \
    thread.cpp \
    recovery.cpp \
    sendtoarduino.cpp

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
    include/transport.h \
    include/socketclient.h \
    include/temprule.h \
    include/sharedspace.h \
    include/sqlite3.h \
    include/sqlhelper.h \
    include/json/allocator.h \
    include/json/assertions.h \
    include/json/autolink.h \
    include/json/config.h \
    include/json/features.h \
    include/json/forwards.h \
    include/json/json.h \
    include/json/reader.h \
    include/json/value.h \
    include/json/version.h \
    include/json/writer.h \
    include/jsoncpp.h \
    include/serialport.h \
    include/ipset.h \
    include/curl/curl.h \
    include/httprequest.h \
    include/threadPool.h \
    include/thread.h \
    include/recovery.h \
    include/md5.h \
    include/sendtoarduino.h

INCLUDEPATH = ./include
LIBS += -lsqlite -lpthread
