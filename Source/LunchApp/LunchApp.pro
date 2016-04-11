######################################################################
# Automatically generated by qmake (3.0) Mon Apr 11 17:49:46 2016
######################################################################

QT += core gui widgets network opengl quick script xml

TEMPLATE = app
TARGET = LunchApp
INCLUDEPATH += .

# Input
HEADERS += MainWindow.h \
           Controller/Controller.h \
           Model/Day.h \
           Model/Dish.h \
           Model/InterestCruncher.h \
           Model/User.h \
           Model/Week.h \
           Network/DataTransfer.h \
           Network/NetEntity.h \
           Network/RestClient.h \
           View/AllWeeksView.h \
           View/CommonStructs.h \
           View/DayDishesView.h \
           View/DayView.h \
           View/DishRatingView.h \
           View/DishView.h \
           View/InfiniteBackground.h \
           View/MetroView.h \
           View/NotificationWindow.h \
           View/SelectedEffect.h \
           View/Style.h \
           View/WeekView.h \
           /View/MetroView.h \
           /View/Style.h \
           /Network/DataTransfer.h \
           /Model/InterestCruncher.h
FORMS += "From Files/MainWindow.ui"
SOURCES += main.cpp \
           MainWindow.cpp \
           Controller/Controller.cpp \
           Model/Day.cpp \
           Model/Dish.cpp \
           Model/InterestCruncher.cpp \
           Model/User.cpp \
           Model/Week.cpp \
           Network/DataTransfer.cpp \
           Network/NetEntity.cpp \
           Network/RestClient.cpp \
           View/AllWeeksView.cpp \
           View/DayDishesView.cpp \
           View/DayView.cpp \
           View/DishRatingView.cpp \
           View/DishView.cpp \
           View/InfiniteBackground.cpp \
           View/MetroView.cpp \
           View/NotificationWindow.cpp \
           View/SelectedEffect.cpp \
           View/Style.cpp \
           View/WeekView.cpp
RESOURCES += lunchapp.qrc
