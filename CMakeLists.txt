cmake_minimum_required(VERSION 3.15)
project(Virgilium)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
include_directories(src)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5PrintSupport REQUIRED)
find_package(Qt5Sql REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Charts REQUIRED)

#include_directories(src/include/)

qt5_add_resources(PROJECT_RESOURCE src/client/Resources/Icons.qrc)

add_executable(VirgiliumServer src/serverMain.cpp src/common/constants.h
        src/common/CRDT/Symbol.cpp src/common/CRDT/Symbol.h
        src/common/messages/BasicMessage.cpp src/common/messages/BasicMessage.h
        src/common/ClientSocket.cpp src/common/ClientSocket.h
        src/server/Server.cpp src/server/Server.h
        src/common/messages/CrdtMessage.cpp src/common/messages/CrdtMessage.h
        src/common/messages/UserMessage.cpp src/common/messages/UserMessage.h
        src/common/User.cpp src/common/User.h
        src/common/messages/FileManagementMessage.cpp src/common/messages/FileManagementMessage.h
        src/common/messages/FilesMessage.cpp src/common/messages/FilesMessage.h
        src/common/messages/ChangePasswordMessage.cpp src/common/messages/ChangePasswordMessage.h
        src/common/messages/FileManagementMessage.cpp src/common/messages/FileManagementMessage.h
        src/common/messages/FilesMessage.cpp src/common/messages/FilesMessage.h
        src/common/messages/UserManagementMessage.cpp src/common/messages/UserManagementMessage.h
        src/server/model/Database.h src/server/model/Database.cpp
        src/server/model/Model.h src/server/model/Model.cpp
        src/common/messages/StorageMessage.cpp src/common/messages/StorageMessage.h
        src/common/messages/InvitationMessage.cpp src/common/messages/InvitationMessage.h
        src/common/messages/ActiveUserMessage.cpp src/common/messages/ActiveUserMessage.h
        src/common/CRDT/Crdt_editor.h src/common/CRDT/Crdt_editor.cpp
        src/server/model/SaveFileService.cpp src/server/model/SaveFileService.h)



add_executable(VirgiliumClient ${PROJECT_RESOURCE} src/clientMain.cpp src/common/constants.h
        src/common/CRDT/Symbol.cpp src/common/CRDT/Symbol.h
        src/common/messages/BasicMessage.cpp src/common/messages/BasicMessage.h
        src/common/ClientSocket.cpp src/common/ClientSocket.h
        src/common/messages/CrdtMessage.cpp src/common/messages/CrdtMessage.h
        src/common/messages/UserMessage.cpp src/common/messages/UserMessage.h
        src/common/User.cpp src/common/User.h src/common/User.cpp src/common/User.cpp
        src/common/messages/ChangePasswordMessage.cpp src/common/messages/ChangePasswordMessage.h
        src/common/messages/FileManagementMessage.cpp src/common/messages/FileManagementMessage.h
        src/common/messages/FilesMessage.cpp src/common/messages/FilesMessage.h
        src/common/messages/UserManagementMessage.cpp src/common/messages/UserManagementMessage.h
        src/common/messages/ChangePasswordMessage.cpp src/common/messages/ChangePasswordMessage.h
        src/common/messages/FileManagementMessage.cpp src/common/messages/FileManagementMessage.h
        src/common/messages/FilesMessage.cpp src/common/messages/FilesMessage.h
        src/common/messages/UserManagementMessage.cpp src/common/messages/UserManagementMessage.h
        src/client/view/WelcomePage.h src/client/view/WelcomePage.cpp src/client/view/WelcomePage.ui
        src/client/view/PersonalPage.h src/client/view/PersonalPage.cpp src/client/view/PersonalPage.ui
        src/client/view/changepassworddialog.h src/client/view/changepassworddialog.cpp src/client/view/changepassworddialog.ui
        src/client/view/deletefile.h src/client/view/deletefile.cpp src/client/view/deletefile.ui
        src/client/view/managecollaborators.h src/client/view/managecollaborators.cpp src/client/view/managecollaborators.ui
        src/client/view/renamefile.h src/client/view/renamefile.cpp src/client/view/renamefile.ui
        src/client/view/renameordelete.h src/client/view/renameordelete.cpp src/client/view/renameordelete.ui
        src/client/view/unsubscribefile.h src/client/view/unsubscribefile.cpp src/client/view/unsubscribefile.ui
        src/client/clientstuff.h src/client/clientstuff.cpp
        src/common/CRDT/Crdt_editor.h src/common/CRDT/Crdt_editor.cpp
        src/client/view/TextEditor.h src/client/view/TextEditor.cpp src/client/view/TextEditor.ui
        src/common/messages/StorageMessage.cpp src/common/messages/StorageMessage.h
        src/common/messages/InvitationMessage.cpp src/common/messages/InvitationMessage.h
        src/common/messages/ActiveUserMessage.cpp src/common/messages/ActiveUserMessage.h)



target_link_libraries(VirgiliumServer Qt5::Core Qt5::Network Qt5::Widgets Qt5::PrintSupport Qt5::Sql)
target_link_libraries(VirgiliumClient Qt5::Core Qt5::Network Qt5::Widgets Qt5::Charts Qt5::PrintSupport Qt5::Gui)
