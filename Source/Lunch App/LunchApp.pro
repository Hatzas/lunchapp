TEMPLATE = app
TARGET = LunchApp

QT += core script widgets gui qml quick
CONFIG += qt

#win32:RC_FILE = LunchApp.rc

HEADERS += Model/Day.h \
    Model/User.h \
    Model/Dish.h \
    Model/Course.h \
    Model/Week.h \
    Network/RestClient.h
SOURCES += Model/Day.cpp \
    main.cpp \
    Model/Dish.cpp \
    Model/Course.cpp \
    Network/RestClient.cpp
RESOURCES += lunchapp.qrc
