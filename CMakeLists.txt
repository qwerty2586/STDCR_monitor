cmake_minimum_required(VERSION 3.0)
project(stimulator_control)

# definice LINUX
if (UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif ()

#[[ pro kompilaci na windows je zapotrebi nainstalovat
qt 4.8.6 a umistit do c:/qt/4.8.6, pro beh na windows jsou pak zapotrebi ve
stejnem adresari s binarkou:
    QtCore4.dll QtGui4.dll QtXml4.dll libgcc_s_dw2-1.dll libstdc++-6.dll libwinpthread-1.dll
pro debug je jeste treba:
    QtCored4.dll QtGuid4.dll QtXmld4.dll
]]
if (WIN32)
    set(CMAKE_PREFIX_PATH c:/qt/4.8.6)
endif (WIN32)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_VERBOSE_MAKEFILE ON)


find_package(Qt4 4.4.3 REQUIRED QtGui QtXml)

QT4_ADD_RESOURCES(RESOURCES_RCC
        stimulator_gui/resources/images.qrc
        )



set(SOURCE_FILES
        stimulator_gui/main.cpp
        stimulator_gui/params.cpp
        stimulator_gui/main_window.cpp

        stimulator_gui/experiment.cpp
        stimulator_gui/experiments/ERP.cpp
        stimulator_gui/experiments/BCI.cpp
        stimulator_gui/experiments/AUTOSTIMULATION.h
        stimulator_gui/experiments/BIOSENSOR_LOGGER.h
        stimulator_gui/experiments/RESPONSE_EXPERIMENT.cpp
        stimulator_gui/experiments/TEST_MODE.cpp
        stimulator_gui/experiments/STIMULATOR_OUTPUTS.cpp

        stimulator_gui/widgets/qcheckgrid.cpp
        stimulator_gui/widgets/qwaveform.cpp
        stimulator_gui/widgets/qsaveloadwidget.cpp
        stimulator_gui/widgets/qverticalscrollarea.cpp


        stimulator_fileserver/transfer_protocol.cpp
        stimulator_fileserver/transfer_protocol.h
        stimulator_fileserver/fileserver.cpp
        stimulator_fileserver/upload_container.cpp

        stimulator_comm/stimulator.cpp

        stimulator_log/log.h

        ${RESOURCES_RCC}
        )

set(SOURCE_FILES_WIN
        3rdparty/qextserialport/qextserialport.cpp
        3rdparty/qextserialport/qextserialport_win.cpp

        stimulator_comm/serials_win.cpp
        stimulator_comm/sysname_win.cpp
        )

set(SOURCE_FILES_LINUX
        3rdparty/qextserialport/qextserialport.cpp
        3rdparty/qextserialport/qextserialport_unix.cpp

        stimulator_comm/serials_linux.cpp
        stimulator_comm/sysname_linux.cpp)

if (WIN32)
    set(SOURCE_FILES ${SOURCE_FILES} ${SOURCE_FILES_WIN})
    add_executable(stimulator_control ${SOURCE_FILES})
endif (WIN32)

if (LINUX)
    set(SOURCE_FILES ${SOURCE_FILES} ${SOURCE_FILES_LINUX} stimulator_log/log.cpp)
    add_executable(stimulator_control ${SOURCE_FILES})
endif (LINUX)


add_subdirectory(stimulator_tests/qt_tests)

target_link_libraries(stimulator_control Qt4::QtGui Qt4::QtXml)