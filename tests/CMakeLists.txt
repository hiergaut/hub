# cmake_minimum_required(VERSION 3.5)

project(tests LANGUAGES CXX)

set(LINK_LIBRARIES Catch2::Catch2WithMain hub)
set(INCLUDE_LIBRARIES)

if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/external/src/Catch2-install/lib/cmake/Catch2")
    set(Catch2_DIR
        "${CMAKE_CURRENT_BINARY_DIR}/external/src/Catch2-install/lib/cmake/Catch2"
        CACHE PATH "" FORCE)
endif()

find_package(Catch2 CONFIG)

option(HUB_USE_FETCH "Use fetch content to get Catch2" ON)
if(HUB_USE_FETCH)
    if(NOT Catch2_FOUND)
        include(FetchContent)
        FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG v3.8.0
            GIT_PROGRESS TRUE)
        FetchContent_MakeAvailable(Catch2)
    endif()

else()

    if(NOT Catch2_FOUND)
        include(ExternalProject)

        list(
            APPEND
            CMAKE_ARGS
            "-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/external/src/Catch2-install"
            "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
            "-DBUILD_TESTING=OFF"
            "-DCATCH_BUILD_TESTING=OFF"
            "-DCATCH_DEVELOPMENT_BUILD=OFF"
            "-DCATCH_INSTALL_DOCS=OFF"
            "-DCATCH_INSTALL_EXTRAS=OFF")

        message(STATUS "Preparing external project Catch2 with args:")
        foreach(CMAKE_ARG ${CMAKE_ARGS})
            message(STATUS " ${CMAKE_ARG}")
        endforeach()

        ExternalProject_Add(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG v3.8.0 # or a later release
            GIT_SHALLOW ON
            GIT_PROGRESS TRUE
            PREFIX "external"
            CMAKE_ARGS "${CMAKE_ARGS}")

        # # Expose required variable (CATCH_INCLUDE_DIR) to parent scope ExternalProject_Get_Property(catch2 source_dir)
    endif()
endif()

if(Catch2_FOUND)
    message(STATUS "${HEADER_MSG} Found Catch2 ${Catch2_VERSION} (${Catch2_DIR})")
endif()

enable_testing()

############################# FOR EACH TEST ######################

foreach(testFile ${test_srcs})

if(HUB_USE_FETCH OR Catch2_FOUND)
    set(test_srcs)
    set(test_headers)
    foreach(HUB_MODULE ${HUB_MODULES})
        file(GLOB_RECURSE test_src ${CMAKE_CURRENT_SOURCE_DIR}/${HUB_MODULE}/*.cpp)
        list(APPEND test_srcs ${test_src})

        file(GLOB_RECURSE test_header ${CMAKE_CURRENT_SOURCE_DIR}/${HUB_MODULE}/*.hpp)
        list(APPEND test_headers ${test_header})
    endforeach()

    list(APPEND test_headers ${CMAKE_CURRENT_SOURCE_DIR}/test_common.hpp)

    add_library(test-common STATIC ${test_headers} ${CMAKE_CURRENT_SOURCE_DIR}/test_common.cpp)
    add_dependencies(test-common Catch2::Catch2WithMain)
    target_link_libraries(test-common PUBLIC ${LINK_LIBRARIES})
    set(LINK_LIBRARIES ${LINK_LIBRARIES} test-common)

    if(HUB_BUILD_CORE)
        find_package(Eigen3 QUIET)
        if(Eigen3_FOUND)
            set(LINK_LIBRARIES ${LINK_LIBRARIES} Eigen3::Eigen)
            set(INCLUDE_LIBRARIES ${INCLUDE_LIBRARIES} ${EIGEN3_INCLUDE_DIR})
            message(STATUS "${HEADER_MSG} Found Eigen ${EIGEN3_VERSION_STRING}")
        else()
            list(REMOVE_ITEM test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/core/matrix/test-core-matrix-compatEigen3.cpp)
            message(WARNING "core/matrix/test-core-matrix-compatEigen3 disabled")
        endif()

        find_package(OpenCV QUIET COMPONENTS core imgproc)
        if(OpenCV_FOUND)
            set(LINK_LIBRARIES ${LINK_LIBRARIES} opencv_core opencv_imgproc)
            set(INCLUDE_LIBRARIES ${INCLUDE_LIBRARIES} ${OpenCV_INCLUDE_DIRS}/opencv2)
            message(STATUS "${HEADER_MSG} Found OpenCV ${OpenCV_VERSION}")
        else()
            list(REMOVE_ITEM test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/core/matrix/test-core-matrix-compatOpenCV.cpp)
            message(WARNING "core/matrix/test-core-matrix-compatOpenCV disabled")
        endif()

        find_package(OpenImageIO QUIET)
        if(OpenImageIO_FOUND)
            set(LINK_LIBRARIES ${LINK_LIBRARIES} OpenImageIO)
            set(INCLUDE_LIBRARIES ${INCLUDE_LIBRARIES} ${OpenImageIO_INCLUDE_DIR}/OpenImageIO)
            message(
                STATUS "${HEADER_MSG} Found OIIO ${OIIO_USING_IMATH_VERSION_MAJOR}.${OIIO_USING_IMATH_VERSION_MINOR}")
        else()
            list(REMOVE_ITEM test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/core/matrix/test-core-matrix-compatOIIO.cpp)
            message(WARNING "core/matrix/test-core-matrix-compatOIIO disabled")
        endif()
    endif()

    find_package(PahoMqttCpp QUIET)
    if(PahoMqttCpp_FOUND)
        set(LINK_LIBRARIES ${LINK_LIBRARIES} PahoMqttCpp::paho-mqttpp3)
    else()
        list(FILTER test_srcs EXCLUDE REGEX ".*Mqtt\\.cpp$")
    endif()

    if(NOT HUB_BUILD_SERVER)
        list(FILTER test_srcs EXCLUDE REGEX ".*inputOutputStream/.*\\.cpp$")
        list(FILTER test_srcs EXCLUDE REGEX ".*test-io-InputOutputStream.cpp$")
    endif()

    if(NOT HUB_BUILD_SENSOR)
        list(REMOVE_ITEM test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/client/test-client-ViewerSensor.cpp)
    endif()

    list(REMOVE_ITEM test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/test-end.cpp)
    list(APPEND test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/test-end.cpp)

    list(REMOVE_ITEM test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/test-start.cpp)
    list(PREPEND test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/test-start.cpp)

    add_executable(hub-test-all ${test_srcs} ${test_headers})
    add_dependencies(hub-test-all Catch2::Catch2WithMain)
    target_link_libraries(hub-test-all PRIVATE ${LINK_LIBRARIES})
    target_include_directories(hub-test-all PRIVATE ${INCLUDE_LIBRARIES} ${CMAKE_CURRENT_SOURCE_DIR})

    if(PahoMqttCpp_FOUND)
        target_compile_definitions(hub-test-all PUBLIC -DHUB_TESTS_MQTT_FOUND)
    endif()

    if(WIN32)
        if(HUB_BUILD_SHARED AND NOT HUB_BUILD_STATIC)
            add_custom_command(
                TARGET hub-test-all
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:hub_shared> $<TARGET_FILE_DIR:hub-test-all>)
        endif()
    endif()

    include(CTest)

    if(HUB_SERVICE_PORT)
        set(HUB_SERVER_TEST_BIN_PORT ${HUB_SERVICE_PORT})
    else()
        if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
            set(HUB_SERVER_TEST_BIN_PORT 6000) # 5000 used by macos
        else()
            set(HUB_SERVER_TEST_BIN_PORT 8000) # 7000 used by macos
        endif()
    endif()

    enable_testing()
    foreach(testFile ${test_srcs})

        get_filename_component(test ${testFile} NAME_WE)

        add_executable(hub-${test} ${testFile})
        add_dependencies(hub-${test} Catch2::Catch2WithMain)
        target_link_libraries(hub-${test} PRIVATE ${LINK_LIBRARIES})
        target_include_directories(hub-${test} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
        if(PahoMqttCpp_FOUND)
            target_compile_definitions(hub-${test} PUBLIC -DHUB_TESTS_MQTT_FOUND)
        endif()

        add_test(NAME hub-${test} COMMAND hub-${test})

        add_custom_target(
            hub-tests-bin-${test}
            COMMAND bash ${HUB_PROJECT_DIR}/scripts/generateServerPort.sh ${HUB_SERVER_TEST_BIN_PORT}
            COMMAND bash ${HUB_PROJECT_DIR}/scripts/launcher.sh ${HUB_BIN_DIR}/hub-server --port
                    ${HUB_SERVER_TEST_BIN_PORT} --daemon
            COMMAND bash ${HUB_PROJECT_DIR}/scripts/launcher.sh ${HUB_BIN_DIR}/hub-viewer --port
                    ${HUB_SERVER_TEST_BIN_PORT} --exitWhenServerLost
            COMMAND bash ${HUB_PROJECT_DIR}/scripts/launcher.sh ${HUB_BIN_DIR}/hub-nativeViewer --port
                    ${HUB_SERVER_TEST_BIN_PORT} --exitWhenServerLost
            COMMAND ctest -C ${CMAKE_BUILD_TYPE} -V -R ${test}
            COMMAND ctest -C ${CMAKE_BUILD_TYPE} -V -R test-end
            USES_TERMINAL
            COMMENT "Running server")

    endforeach()

    if(HUB_BUILD_NATIVE)
        add_custom_target(
            hub-tests-bin
            COMMAND bash ${HUB_PROJECT_DIR}/scripts/generateServerPort.sh ${HUB_SERVER_TEST_BIN_PORT}
            COMMAND bash ${HUB_PROJECT_DIR}/scripts/launcher.sh ${HUB_BIN_DIR}/hub-server --port
                    ${HUB_SERVER_TEST_BIN_PORT} --daemon
            COMMAND bash ${HUB_PROJECT_DIR}/scripts/launcher.sh ${HUB_BIN_DIR}/hub-viewer --port
                    ${HUB_SERVER_TEST_BIN_PORT} --exitWhenServerLost
            COMMAND bash ${HUB_PROJECT_DIR}/scripts/launcher.sh ${HUB_BIN_DIR}/hub-nativeViewer --port
                    ${HUB_SERVER_TEST_BIN_PORT} --exitWhenServerLost
            COMMAND ctest -C ${CMAKE_BUILD_TYPE} -V
            USES_TERMINAL
            COMMENT "Running server")

    elseif(HUB_BUILD_CLIENT)
        add_custom_target(
            hub-tests-bin
            COMMAND bash ${HUB_PROJECT_DIR}/scripts/generateServerPort.sh ${HUB_SERVER_TEST_BIN_PORT}
            COMMAND bash ${HUB_PROJECT_DIR}/scripts/launcher.sh ${HUB_BIN_DIR}/hub-server --port
                    ${HUB_SERVER_TEST_BIN_PORT} --daemon
            COMMAND bash ${HUB_PROJECT_DIR}/scripts/launcher.sh ${HUB_BIN_DIR}/hub-viewer --port
                    ${HUB_SERVER_TEST_BIN_PORT} --exitWhenServerLost
            COMMAND ctest -C ${CMAKE_BUILD_TYPE} -V
            USES_TERMINAL
            COMMENT "Running server")

    elseif(HUB_BUILD_SERVER)
        add_custom_target(
            hub-tests-bin
            COMMAND bash ${HUB_PROJECT_DIR}/scripts/generateServerPort.sh ${HUB_SERVER_TEST_BIN_PORT}
            COMMAND bash ${HUB_PROJECT_DIR}/scripts/launcher.sh ${HUB_BIN_DIR}/hub-server --port
                    ${HUB_SERVER_TEST_BIN_PORT} --daemon
            COMMAND ctest -C ${CMAKE_BUILD_TYPE} -V
            USES_TERMINAL
            COMMENT "Running server")
    endif()

    if(TARGET hub-tests-bin)
        custom_target_added(tests-bin)
    endif()

    add_custom_target(
        hub-tests-bin-${test}
        COMMAND bash ${HUB_PROJECT_DIR}/scripts/generateServerPort.sh ${HUB_SERVER_TEST_BIN_PORT}
        COMMAND bash ${HUB_PROJECT_DIR}/scripts/launcher.sh ${HUB_BIN_DIR}/hub-bin-server --port ${HUB_SERVER_TEST_BIN_PORT} --daemon
        COMMAND bash ${HUB_PROJECT_DIR}/scripts/launcher.sh ${HUB_BIN_DIR}/hub-bin-viewer --port ${HUB_SERVER_TEST_BIN_PORT} --exitWhenServerLost
        COMMAND bash ${HUB_PROJECT_DIR}/scripts/launcher.sh ${HUB_BIN_DIR}/hub-bin-nativeViewer --port ${HUB_SERVER_TEST_BIN_PORT} --exitWhenServerLost
        COMMAND ctest -C ${CMAKE_BUILD_TYPE} -V -R test-start
        COMMAND ctest -C ${CMAKE_BUILD_TYPE} -V -R ${test}
        COMMAND ctest -C ${CMAKE_BUILD_TYPE} -V -R test-end
        USES_TERMINAL
        COMMENT "Running server")

endforeach()

############################# FOR ALL TEST ######################

if(HUB_BUILD_NATIVE)
    add_custom_target(
        hub-tests-bin
        COMMAND bash ${HUB_PROJECT_DIR}/scripts/generateServerPort.sh ${HUB_SERVER_TEST_BIN_PORT}
        COMMAND bash ${HUB_PROJECT_DIR}/scripts/launcher.sh ${HUB_BIN_DIR}/hub-bin-server --port ${HUB_SERVER_TEST_BIN_PORT} --daemon
        COMMAND bash ${HUB_PROJECT_DIR}/scripts/launcher.sh ${HUB_BIN_DIR}/hub-bin-viewer --port ${HUB_SERVER_TEST_BIN_PORT} --exitWhenServerLost
        COMMAND bash ${HUB_PROJECT_DIR}/scripts/launcher.sh ${HUB_BIN_DIR}/hub-bin-nativeViewer --port ${HUB_SERVER_TEST_BIN_PORT} --exitWhenServerLost
        COMMAND ctest -C ${CMAKE_BUILD_TYPE} -V
        USES_TERMINAL
        COMMENT "Running tests")
    custom_target_added(tests)

elseif(HUB_BUILD_CLIENT)
    add_custom_target(
        hub-tests-bin
        COMMAND bash ${HUB_PROJECT_DIR}/scripts/generateServerPort.sh ${HUB_SERVER_TEST_BIN_PORT}
        COMMAND bash ${HUB_PROJECT_DIR}/scripts/launcher.sh ${HUB_BIN_DIR}/hub-bin-server --port ${HUB_SERVER_TEST_BIN_PORT} --daemon
        COMMAND bash ${HUB_PROJECT_DIR}/scripts/launcher.sh ${HUB_BIN_DIR}/hub-bin-viewer --port ${HUB_SERVER_TEST_BIN_PORT} --exitWhenServerLost
        COMMAND ctest -C ${CMAKE_BUILD_TYPE} -V
        USES_TERMINAL
        COMMENT "Running server")

elseif(HUB_BUILD_SERVER)
    add_custom_target(
        hub-tests-bin
        COMMAND bash ${HUB_PROJECT_DIR}/scripts/generateServerPort.sh ${HUB_SERVER_TEST_BIN_PORT}
        COMMAND bash ${HUB_PROJECT_DIR}/scripts/launcher.sh ${HUB_BIN_DIR}/hub-bin-server --port ${HUB_SERVER_TEST_BIN_PORT} --daemon
        COMMAND ctest -C ${CMAKE_BUILD_TYPE} -V
        USES_TERMINAL
        COMMENT "Running server")
endif()
