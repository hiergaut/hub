
if (WIN32)
    project(hub-wrapper-CSharp LANGUAGES CSharp)
    include(CSharpUtilities)

    file(GLOB_RECURSE files "*.cs")

    add_executable(${PROJECT_NAME} ${files})

    target_link_libraries(${PROJECT_NAME} PUBLIC hub)

endif()
