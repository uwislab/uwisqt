QT       += core gui webenginewidgets webchannel network gui-private

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

RC_ICONS = qBlock.ico

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cloudprojectedit.cpp \
    cloudprojectswidget.cpp \
    cloudprojectview.cpp \
    commentwidget.cpp \
    communitycard.cpp \
    communitywidget.cpp \
    filetree.cpp \
    httpmanager.cpp \
    inisettings.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    output.cpp \
    projectcard.cpp \
    showcodes.cpp \
    sidebar.cpp \
    usermanager.cpp \
    webhandle.cpp \
    zipcompress.cpp

HEADERS += \
    cloudprojectedit.h \
    cloudprojectswidget.h \
    cloudprojectview.h \
    commentwidget.h \
    communitycard.h \
    communitywidget.h \
    filetree.h \
    httpmanager.h \
    inisettings.h \
    login.h \
    mainwindow.h \
    output.h \
    projectcard.h \
    showcodes.h \
    sidebar.h \
    usermanager.h \
    webhandle.h \
    zipcompress.h

FORMS += \
    cloudprojectswidget.ui \
    filetree.ui \
    inisettings.ui \
    login.ui \
    mainwindow.ui \
    output.ui \
    projectcard.ui \
    showcodes.ui \
    sidebar.ui \
    usermanager.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
