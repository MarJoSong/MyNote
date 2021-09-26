QT       += core gui network sql webenginewidgets webchannel

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# RC_ICONS = r2o.ico

#ICON = r2o.icns

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    document.cpp \
    main.cpp \
    mainwindow.cpp \
    previewpage.cpp \
    qformtable.cpp

HEADERS += \
    document.h \
    mainwindow.h \
    previewpage.h \
    qformtable.h \
    stdafx.h

FORMS += \
    mainwindow.ui \
    qformtable.ui

# RELEASE USE
QTQUICK_COMPILER_SKIPPED_RESOURCES  += \
  res.qrc

# DEBUG USE
RESOURCES += \
  res.qrc
