cmake_minimum_required(VERSION 3.1)
project(temp_test CXX)


add_compile_options(-g)

add_compile_options(-std=c++11 )

add_compile_options(-m32)
set(cmake_exe_linker_flags  "${cmake_exe_linker_flags} -m32")
set(cmake_shared_linker_flags  "${cmake_shared_linker_flags} -m32")
# set(cmake_module_linker_flaGS  "${CMAKE_MODULE_LINKER_FLAGS} -m32")


# include_directories(/mnt/hgfs/main_work/temp_test/cpp_test/lib) 

include_directories(/mnt/hgfs/main_work/temp_test/cpp_test/evpp) 
include_directories(./evpp) 



# add_executable(test_dns ./test_dns.cpp)
# add_executable(time_benchmark time_benchmark.cpp)
# add_executable(vsyscall_test vsyscall_test.cpp)
# add_executable(asm_vsyscall_test-32 asm_vsyscall_test.cpp)
# add_executable(asm_vsyscall_test-64 asm_vsyscall_test.cpp)
# add_executable(get_vdso_file get_vdso_file.cpp )


add_executable(test_time_cache test_time_cache.cc)
# add_executable(clock_gettime_test clock_gettime_test.cpp )

# link_directories(./lib)
target_link_libraries(test_time_cache pthread)
