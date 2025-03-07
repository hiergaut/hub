
project(hub-wrapper-Python)
# set(_PROJECT_NAME "hub-python")

find_package(Python COMPONENTS NumPy QUIET)

if (Python_FOUND)

    file(GLOB_RECURSE files "*.c" "*.h")
    file(GLOB_RECURSE tests *.py)

    set(CMAKE_DEBUG_POSTFIX "")
    set(CMAKE_SHARED_LIBRARY_PREFIX "")
    add_library(${PROJECT_NAME} SHARED ${files} ${tests})
    set_property(TARGET ${PROJECT_NAME} PROPERTY OUTPUT_NAME hubPy)

    if (WIN32)
        set(HUB_PYTHON_DLL_FILENAME "hubPy.dll" CACHE PATH "" FORCE)
    elseif (LINUX)
        set(HUB_PYTHON_DLL_FILENAME "hubPy.so" CACHE PATH "" FORCE)
    elseif (APPLE)
        set(HUB_PYTHON_DLL_FILENAME "hubPy.dlyb" CACHE PATH "" FORCE)
    endif()
    set(HUB_PYTHON_DLL ${CMAKE_CURRENT_BINARY_DIR}/${HUB_PYTHON_DLL_FILENAME} CACHE PATH "" FORCE)
    if (NOT EXISTS ${HUB_PYTHON_DLL})
        message(FATAL_ERROR "hub python dll not found")
    endif()

    target_include_directories(${PROJECT_NAME} PRIVATE ${PYTHON_INCLUDE_PATH})

    target_link_libraries(${PROJECT_NAME} PRIVATE ${PYTHON_LIBRARIES} Python::NumPy hub)

    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/main.py" $<TARGET_FILE_DIR:${PROJECT_NAME}>
        # COMMAND ${PYTHON_EXECUTABLE} main.py
    )

    add_custom_target(
        hub-wrapper-python-test # WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMAND ${PYTHON_EXECUTABLE} main.py
        COMMENT "Running python tests")


endif()
