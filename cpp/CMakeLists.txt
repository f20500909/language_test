cmake_minimum_required(VERSION 2.8)
project(temp_test CXX)


add_compile_options(-g)

add_compile_options(-std=c++11 )

# add_compile_options(-m32)
# set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -m32")
# set(CMAKE_SHARED_LINKER_FLAGS  "${CMAKE_SHARED_LINKER_FLAGS} -m32")
# set(CMAKE_MODULE_LINKER_FLAGS  "${CMAKE_MODULE_LINKER_FLAGS} -m32")
# set(CMAKE_STATIC_LINKER_FLAGS  "${CMAKE_STATIC_LINKER_FLAGS} -m32")



include_directories(./evpp) 

# add_executable(test_dns ./test_dns.cpp)
# add_executable(time_benchmark time_benchmark.cpp)
# add_executable(vsyscall_test vsyscall_test.cpp)
# add_executable(asm_vsyscall_test-32 asm_vsyscall_test.cpp)
# add_executable(asm_vsyscall_test-64 asm_vsyscall_test.cpp)
# add_executable(get_vdso_file get_vdso_file.cpp )


# add_executable(cur_test clock_gettime_test.cpp )
# add_executable(cur_test test_time_cache.cc)
# add_executable(cur_test cpu/sched_setaffinity.cpp)
# add_executable(cur_test ./gettimeofday/test_vdso_gettimeofday.cpp)
# add_executable(cur_test ./gettimeofday/test.cpp)
# add_executable(cur_test ./gettimeofday/atomic_time_cache.cpp)
# add_executable(cur_test ./gettimeofday/rwlock_time_cache.cpp)

# add_executable(cur_test ./cpp_language/init_test.cpp)
# add_executable(cur_test ./cpp_language/sprintf_test.cpp)
# add_executable(cur_test ./cpp_language/return_func_test.cpp)
# add_executable(cur_test    ./temp_interview/interview_1.cpp )
add_executable(cur_test    ./temp/temp1.cpp )


# link_directories(./lib)
target_link_libraries(cur_test pthread )
