# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

TEMPLATE = app
TARGET = LunchApp
DESTDIR = ../Win32/Debug
QT += core network xml script opengl widgets gui qml quick multimedia multimediawidgets androidextras
CONFIG += debug mobility
MOBILITY = 
DEFINES += WIN64 QT_DLL QT_QML_LIB QT_QUICK_LIB QT_OPENGL_LIB QT_WIDGETS_LIB QT_XML_LIB QT_SCRIPT_LIB QT_NETWORK_LIB
INCLUDEPATH += ./GeneratedFiles \
    . \
    ./GeneratedFiles/Debug \
    ./View \
    ./Controller
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(LunchApp.pri)
win32:RC_FILE = LunchApp.rc

DISTFILES += \
    Android/AndroidManifest.xml \
    Android/gradle/wrapper/gradle-wrapper.jar \
    Android/gradlew \
    Android/res/values/libs.xml \
    Android/build.gradle \
    Android/gradle/wrapper/gradle-wrapper.properties \
    Android/gradlew.bat \
    Android/src/org/qtproject/LunchApp/LunchApp.java

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/Android
