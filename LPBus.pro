QT += core
QT -= gui

CONFIG += c++11

DEFINES += PROJECT_PATH="\"$$PWD\""
DEFINES += BUILD_PATH="\"$$OUT_PWD\""

TARGET = LPBus
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    passengercreator.cpp \
    router.cpp \
    graph.cpp \
    main.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    passengercreator.h \
    router.h \
    graph.h

win32: LIBS += -L$$PWD/lib/lp_solve_5.5.2.5_dev_win32/ -llpsolve55

INCLUDEPATH += $$PWD/lib/lp_solve_5.5.2.5_dev_win32
DEPENDPATH += $$PWD/lib/lp_solve_5.5.2.5_dev_win32

DISTFILES += \
    maps/map-stanford-small.txt \
    maps/map-istanbul.txt \
    maps/map-san-francisco.txt \
    maps/map-stanford-small.txt
