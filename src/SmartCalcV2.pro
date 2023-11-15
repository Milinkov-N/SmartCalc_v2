QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += plot

SOURCES += \
    main.cc \
    model/model.cc \
    view/mainwindow.cc \
    plot/qcustomplot.cc \
    view/plotgraph.cc

HEADERS += \
    model/model.h \
    view/mainwindow.h \
    controller/controller.h \
    plot/qcustomplot.h \
    view/plotgraph.h

FORMS += \
    view/mainwindow.ui \
    view/plotgraph.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
