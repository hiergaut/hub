
project(hubPy)
# set(_PROJECT_NAME "hub-python")

find_package(Python COMPONENTS NumPy QUIET)

if (Python_FOUND)

    file(GLOB_RECURSE files "*.c" "*.h")
    file(GLOB_RECURSE tests *.py)

    set(CMAKE_DEBUG_POSTFIX "")
    set(CMAKE_SHARED_LIBRARY_PREFIX "")
    add_library(${PROJECT_NAME} SHARED ${files} ${tests})

    target_include_directories(${PROJECT_NAME} PRIVATE ${PYTHON_INCLUDE_PATH})

    target_link_libraries(${PROJECT_NAME} PRIVATE ${PYTHON_LIBRARIES} Python::NumPy)

    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/test.py" $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${PYTHON_EXECUTABLE} test.py
    )

    add_custom_target(
        wrapper-python-test # WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMAND ${PYTHON_EXECUTABLE} test.py
        COMMENT "Running python tests")


endif()