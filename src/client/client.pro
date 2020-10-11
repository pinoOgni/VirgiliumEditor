QT       += core gui
QT       += network
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
TARGET = Virgilium
TEMPLATE = app
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
    main.cpp \
    clientstuff.cpp \
    view/WelcomePage.cpp \
    view/PersonalPage.cpp \
    view/TextEditor.cpp\
    view/changepassworddialog.cpp \
    view/deletefile.cpp \
    view/managecollaborators.cpp \
    view/renamefile.cpp \
    view/renameordelete.cpp \
    view/unsubscribefile.cpp \
    ../common/User.cpp \
    ../common/messages/BasicMessage.cpp \
    ../common/messages/CrdtMessage.cpp \
    ../common/messages/FileManagementMessage.cpp \
    ../common/messages/UserManagementMessage.cpp \
    ../common/messages/ChangePasswordMessage.cpp \
    ../common/messages/FilesMessage.cpp \
    ../common/messages/UserMessage.cpp \
    ../common/CRDT/Symbol.cpp \
    ../common/ClientSocket.cpp \
    ../common/CRDT/Crdt_editor.cpp


HEADERS += \
    view/WelcomePage.h \
    view/PersonalPage.h \
    clientstuff.h \
    view/changepassworddialog.h \
    view/deletefile.h \
    view/managecollaborators.h \
    view/renamefile.h \
    view/renameordelete.h \
    view/unsubscribefile.h \
    ../common/User.h \
    ../common/messages/BasicMessage.h \
    ../common/messages/CrdtMessage.h \
    ../common/messages/FileManagementMessage.h \
    ../common/messages/UserManagementMessage.h \
    ../common/messages/ChangePasswordMessage.h \
    ../common/messages/UserMessage.h \
    ../common/messages/FilesMessage.h  \
    ../common/constants.h \
    ../common/CRDT/Symbol.h \
    ../common/ClientSocket.h \
    view/TextEditor.h

FORMS += \
    view/WelcomePage.ui \
    view/PersonalPage.ui \
    view/changepassworddialog.ui \
    view/deletefile.ui \
    view/managecollaborators.ui \
    view/renamefile.ui \
    view/renameordelete.ui \
    view/unsubscribefile.ui \
    view/TextEditor.ui

RESOURCES += \
    resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
