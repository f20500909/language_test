clear

ulimit -c unlimited 

rm -rf ./core*
basepath=$(cd `dirname $0`; pwd)
# project_name=CSCF_dev_tata_bulkcall_sip_ua

echo ${basepath} 

root_path=`cd $(dirname $basepath/); pwd`

cd ${root_path}/cpp_test/build
echo ${root_path} 

# cmake ..&&make&&  ./asm_vsyscall_test-64
# cmake ..&&make&&  ./asm_vsyscall_test-32
# cmake ..&&make&&  ./vsyscall_test
cmake ..&&make 

# time ./time_benchmark 3

# echo "vsyscall"
# time ./time_benchmark vsyscall

# echo "vdso\n"
# time ./time_benchmark vdso

# echo "3\n"
# time ./time_benchmark 3
                
# ./time_benchmark 3
# ./time_thread  1000

time ./test_time_cache


