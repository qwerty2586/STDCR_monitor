
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(runFileserverTests
        hello.cpp)

target_link_libraries(runFileserverTests gtest gtest_main)
target_link_libraries(runFileserverTests stimulator_control)