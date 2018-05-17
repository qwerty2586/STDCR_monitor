cmake_minimum_required(VERSION 3.0)
project(stimulator_control)

# definice LINUX
if (UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif ()


if (WIN32)
    set(CMAKE_PREFIX_PATH c:/qt/4.8.6)
endif (WIN32)


set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 11)


find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Xml REQUIRED)

#find_package(Qt5 4.4.3 REQUIRED QtGui QtXml)

qt5_add_resources(RESOURCES_RCC
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

        stimulator_outputlauncher/sdl_output_launcher.cpp
        stimulator_outputlauncher/sdl_output_launcher.h

        stimulator_comm/stimulator.cpp

        stimulator_log/log.h
        stimulator_log/log.cpp

        ${RESOURCES_RCC}
        stimulator_gui/widgets/qclickablelabel.cpp stimulator_gui/widgets/qclickablelabel.h)

set(SOURCE_FILES_WIN
#        3rdparty/qextserialport/qextserialport.cpp
#        3rdparty/qextserialport/qextserialport_win.cpp

        stimulator_comm/sysname_win.cpp

        )

set(SOURCE_FILES_LINUX
#        3rdparty/qextserialport/qextserialport.cpp
#        3rdparty/qextserialport/qextserialport_unix.cpp

        stimulator_comm/sysname_linux.cpp)

if (WIN32)
    set(SOURCE_FILES ${SOURCE_FILES} ${SOURCE_FILES_WIN})
    add_executable(stimulator_control ${SOURCE_FILES})
endif (WIN32)

if (LINUX)
    set(SOURCE_FILES ${SOURCE_FILES} ${SOURCE_FILES_LINUX} )
    add_executable(stimulator_control ${SOURCE_FILES})
endif (LINUX)


#add_subdirectory(stimulator_tests/qt_tests)
qt5_use_modules(stimulator_control Core SerialPort Xml)
target_link_libraries(stimulator_control Qt5::Widgets  Qt5::Core Qt5::Xml)