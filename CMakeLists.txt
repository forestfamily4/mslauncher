
cmake_minimum_required(VERSION 3.5)

project(mslauncher VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools Concurrent)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt6 REQUIRED COMPONENTS Network)


set(TS_FILES mslauncher_ja_JP.ts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        data/data.cpp
        data/data.h
        data/server.h data/server.cpp
        data/serverproperty.h data/serverproperty.cpp
        back/discordsrv.h back/discordsrv.cpp
        icon.ico
        back/commandline.h back/commandline.cpp
        icon.rc
        back/rcon.h back/rcon.cpp
        back/downloadmanager.h back/downloadmanager.cpp

        ${TS_FILES}
)




if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(mslauncher
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET mslauncher APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})


else()
    if(ANDROID)
        add_library(mslauncher SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    elseif(WIN32)
        add_executable(mslauncher
            ${PROJECT_SOURCES}
            icon.rc
        )
    else()
        add_executable(mslauncher
            ${PROJECT_SOURCES}
        )

    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(mslauncher PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(mslauncher PRIVATE Qt6::Network)
set_target_properties(mslauncher PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS mslauncher
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})



if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(mslauncher)
endif()
