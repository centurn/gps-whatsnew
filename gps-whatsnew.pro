TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt
LIBPATH += /usr/local/lib/

LIBS += \
       -lboost_system\
       -lboost_filesystem\

SOURCES += main.cpp \
    pugixml/pugixml.cpp \
    gps_data.cpp \
    gps_utils.cpp \
    gps_analyze.cpp \
    gps_track_aggregator.cpp

HEADERS += \
    pugixml/pugiconfig.hpp \
    pugixml/pugixml.hpp \
    gps_data.h \
    gps_utils.h \
    gps_analyze.h \
    gps_track_aggregator.h
