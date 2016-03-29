cmake_minimum_required(VERSION 3.3)
project(STDCR_monitor)

set(CMAKE_PREFIX_PATH c:/qt/4.8.6)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_VERBOSE_MAKEFILE ON)


find_package(Qt4 4.4.3 REQUIRED QtGui QtXml)


set(SOURCE_FILES stdcr_monitor/main.cpp stdcr_monitor/params.cpp stdcr_monitor/main_window.cpp
        stdcr_monitor/experiment.cpp stdcr_monitor/experiments/ERP.cpp stdcr_monitor/experiments/BCI.cpp
        stdcr_monitor/experiments/AUTOSTIMULATION.h stdcr_monitor/experiments/BIOSENSOR_LOGGER.h stdcr_monitor/widgets/qcheckgrid.cpp stdcr_monitor/experiments/TEST_MODE.cpp stdcr_monitor/widgets/qwaveform.cpp stdcr_monitor/widgets/qwaveform.h stdcr_monitor/experiments/RESPONSE_EXPERIMENT.cpp stdcr_monitor/widgets/qsaveloadwidget.cpp stdcr_monitor/widgets/qsaveloadwidget.h)

add_executable(STDCR_monitor ${SOURCE_FILES})

target_link_libraries(STDCR_monitor Qt4::QtGui)