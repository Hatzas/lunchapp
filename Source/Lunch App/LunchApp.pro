TEMPLATE = app
TARGET = LunchApp

QT += core script widgets gui qml quick network
CONFIG += qt

#win32:RC_FILE = LunchApp.rc

HEADERS += Model/Day.h \
    Model/User.h \
    Model/Dish.h \
    Model/Course.h \
    Model/Week.h \
    Network/RestClient.h \
    Network/DataTransfer.h \
    Network/NetEntity.h \
    View/DayView.h \
    View/DishView.h \
    View/MainWindow.h \
    View/MetroView.h \
    View/Style.h \
    View/WeekView.h
SOURCES += Model/Day.cpp \
    main.cpp \
    Model/Dish.cpp \
    Model/Course.cpp \
    Network/RestClient.cpp \
    Network/DataTransfer.cpp \
    Network/NetEntity.cpp \
    View/DayView.cpp \
    View/DishView.cpp \
    View/MainWindow.cpp \
    View/MetroView.cpp \
    View/WeekView.cpp
RESOURCES += lunchapp.qrc

FORMS += \
    View/Froms/MainWindow.ui

win32: LIBS += -L$$PWD/../../Libs/QJson/lib/ -lqjson-qt5

INCLUDEPATH += $$PWD/../../Libs/QJson/include
DEPENDPATH += $$PWD/../../Libs/QJson/include
