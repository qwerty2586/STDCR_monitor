cmake_minimum_required(VERSION 3.0)
project(STDCR_monitor)

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


set(SOURCE_FILES
        stdcr_monitor/main.cpp
        stdcr_monitor/params.cpp
        stdcr_monitor/main_window.cpp

        stdcr_monitor/experiment.cpp
        stdcr_monitor/experiments/ERP.cpp
        stdcr_monitor/experiments/BCI.cpp
        stdcr_monitor/experiments/AUTOSTIMULATION.h
        stdcr_monitor/experiments/BIOSENSOR_LOGGER.h
        stdcr_monitor/experiments/TEST_MODE.cpp

        stdcr_monitor/widgets/qcheckgrid.cpp
        stdcr_monitor/widgets/qwaveform.cpp
        stdcr_monitor/experiments/RESPONSE_EXPERIMENT.cpp
        stdcr_monitor/widgets/qsaveloadwidget.cpp
        )

set(SOURCE_FILES_WIN
        3rdparty/qextserialport/qextserialport.cpp
        3rdparty/qextserialport/qextserialport_win.cpp

        stdcr_comm/stimulator.cpp
        stdcr_comm/serials_win.cpp)

set(SOURCE_FILES_LINUX
        3rdparty/qextserialport/qextserialport.cpp
        3rdparty/qextserialport/qextserialport_unix.cpp

        stdcr_comm/stimulator.cpp
        stdcr_comm/serials_linux.cpp)

if (WIN32)
    set(SOURCE_FILES ${SOURCE_FILES} ${SOURCE_FILES_WIN})
    add_executable(STDCR_monitor WIN32 ${SOURCE_FILES})
endif (WIN32)

if (LINUX)
    set(SOURCE_FILES ${SOURCE_FILES} ${SOURCE_FILES_LINUX})
    add_executable(STDCR_monitor ${SOURCE_FILES})
endif (LINUX)

target_link_libraries(STDCR_monitor Qt4::QtGui Qt4::QtXml)