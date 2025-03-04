
project(hub-wrapper-Cpp)

file(GLOB_RECURSE files "*.cpp" "*.hpp")

add_executable(${PROJECT_NAME} ${files})

target_link_libraries(${PROJECT_NAME} PUBLIC hub)
