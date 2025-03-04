
project(hub-wrapper-C)

file(GLOB_RECURSE files "*.c" "*.h")

add_executable(${PROJECT_NAME} ${files})

target_link_libraries(${PROJECT_NAME} PUBLIC hub)
