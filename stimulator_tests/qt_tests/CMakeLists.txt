find_package(Qt4 4.4.3 REQUIRED QtGui QtXml QtTest)

add_executable(qt_tests
        test_test.h fileserver_test.cpp fileserver_test.h
        main.cpp qsignaltester.cpp qsignaltester.h

        ../../stimulator_fileserver/fileserver.cpp
        ../../stimulator_fileserver/fileserver.h

        ../../stimulator_fileserver/transfer_protocol.cpp
        ../../stimulator_fileserver/transfer_protocol.h

        ../../stimulator_fileserver/upload_container.cpp
        ../../stimulator_fileserver/upload_container.h
        )

target_link_libraries(qt_tests Qt4::QtGui Qt4::QtXml Qt4::QtTest)