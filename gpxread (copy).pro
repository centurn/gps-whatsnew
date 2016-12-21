TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    pugixml/pugixml.cpp \
    gps_data.cpp \
    gps_utils.cpp

HEADERS += \
    pugixml/pugiconfig.hpp \
    pugixml/pugixml.hpp \
    gps_data.h \
    gps_utils.h
