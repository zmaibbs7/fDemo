cmake_minimum_required(VERSION 3.14)

project(AbsractModel_QSortFilterProxyModel VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick)

set(PROJECT_SOURCES
        main.cpp
        qml.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(AbsractModel_QSortFilterProxyModel
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET AbsractModel_QSortFilterProxyModel APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(AbsractModel_QSortFilterProxyModel SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(AbsractModel_QSortFilterProxyModel
          ${PROJECT_SOURCES}
          MapModel/MapModel.cpp MapModel/MapModel.h
          MapModel/MapModelManager.cpp MapModel/MapModelManager.h
          MapModel/MapElement.cpp MapModel/MapElement.h
          MapModel/MapPoints/MapPoint.cpp MapModel/MapPoints/MapPoint.h MapModel/MapPoints/MapPointListModel.cpp MapModel/MapPoints/MapPointListModel.h MapModel/MapPoints/MapPointsFilterProxyModel.cpp MapModel/MapPoints/MapPointsFilterProxyModel.h MapModel/MapPoints/MapPointsModel.cpp MapModel/MapPoints/MapPointsModel.h
          CppCommon/singleton.h
        )
    endif()
endif()

target_link_libraries(AbsractModel_QSortFilterProxyModel
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.AbsractModel_QSortFilterProxyModel)
endif()
set_target_properties(AbsractModel_QSortFilterProxyModel PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS AbsractModel_QSortFilterProxyModel
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(AbsractModel_QSortFilterProxyModel)
    qt_finalize_executable(AbsractModel_QSortFilterProxyModel)
endif()
