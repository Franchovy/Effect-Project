QT       += core gui multimedia

#CONFIG += no_keywords

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

LIBS += -lstdc++fs

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    GUI/gui_effect.cpp \
    GUI/gui_item.cpp \
    GUI/gui_line.cpp \
    GUI/gui_port.cpp \
    Params/parameter.cpp \
    Params/sliderparam.cpp \
    audio.cpp \
    effect.cpp \
    effectbuffer.cpp \
    effectmap.cpp \
    effectsLib/delayeffect.cpp \
    effectsLib/echoeffect1.cpp \
    effectsLib/inputeffect.cpp \
    effectsLib/joinereffect.cpp \
    effectsLib/multiplyeffect.cpp \
    effectsLib/outputeffect.cpp \
    effectsLib/paneffect.cpp \
    effectsLib/splittereffect.cpp \
    effectsLib/waveeffect.cpp \
    effectsscene.cpp \
    main.cpp \
    mainwindow.cpp \
    ports/inport.cpp \
    ports/outport.cpp \
    ports/port.cpp \
    settingsdialog.cpp

HEADERS += \
    GUI/gui_effect.h \
    GUI/gui_item.h \
    GUI/gui_line.h \
    GUI/gui_port.h \
    Params/parameter.h \
    Params/sliderparam.h \
    audio.h \
    effect.h \
    effectbuffer.h \
    effectmap.h \
    effectsLib/delayeffect.h \
    effectsLib/echoeffect1.h \
    effectsLib/inputeffect.h \
    effectsLib/joinereffect.h \
    effectsLib/multiplyeffect.h \
    effectsLib/outputeffect.h \
    effectsLib/paneffect.h \
    effectsLib/splittereffect.h \
    effectsLib/waveeffect.h \
    effectsscene.h \
    effectsui.h \
    mainwindow.h \
    ports/inport.h \
    ports/outport.h \
    ports/port.h \
    settingsdialog.h

INCLUDEPATH += \
    fft-real/


FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../usr/lib/x86_64-linux-gnu/release/ -ljack
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../usr/lib/x86_64-linux-gnu/debug/ -ljack
else:unix: LIBS += -L$$PWD/../../../usr/lib/x86_64-linux-gnu/ -ljack

INCLUDEPATH += $$PWD/../../../usr/include/jack
DEPENDPATH += $$PWD/../../../usr/include/jack
