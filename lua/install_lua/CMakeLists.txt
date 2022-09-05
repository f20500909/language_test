cmake_minimum_required(VERSION 3.0)

project(lua)

add_subdirectory(src)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/build/bin)

include_directories(src)
set(interpreter lua.c)
set(luacomplier luac.c)

#虽然对生成的lib进行了重新命名，但还是可以使用去最初的名字
add_executable(lua ${interpreter})
target_link_libraries(lua luastatic)

add_executable(luac ${luacomplier})
target_link_libraries(luac luastatic)