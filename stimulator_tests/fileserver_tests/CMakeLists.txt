include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

find_package(Qt4 4.4.3 REQUIRED QtGui QtXml)

add_executable(runFileserverTests
        hello.cpp)

target_link_libraries(runFileserverTests gtest gtest_main Qt4::QtGui Qt4::QtXml)
#target_link_libraries(runFileserverTests stimulator_control)