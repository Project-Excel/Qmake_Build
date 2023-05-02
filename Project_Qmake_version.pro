QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    maintable.cpp \
    search.cpp
HEADERS += \
    maintable.h \
    search.h
FORMS += \
    maintable.ui \
    search.ui
win32:RC_FILE = file.rc
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../libxl-4.1.2/lib64/
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../libxl-4.1.2/lib64/
else:unix: LIBS += -L$$PWD/../../libxl-4.1.2/lib64/

INCLUDEPATH += $$PWD/../../libxl-4.1.2/lib64
DEPENDPATH += $$PWD/../../libxl-4.1.2/lib64
