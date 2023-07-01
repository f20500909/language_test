
\
case_name="app_to_net"; \
temp_file_dir=~/temp_`hostname`;\
mkdir -p $temp_file_dir; \
cur_dir=`echo $temp_file_dir/$case_name`; \
mkdir -p $cur_dir;  \
echo "cur:"$cur_dir; \
cd $cur_dir;
file_name=`date +%Y-%m-%d_%H-%M-%S`;host_name=`hostname`;tcpdump -i any -w $cur_dir/$file_name[$host_name].pcap






\
case_name="app_to_net"; temp_file_dir=~/temp_`hostname`; mkdir -p $temp_file_dir; cur_dir=`echo $temp_file_dir/$case_name`; mkdir -p $cur_dir;  echo "cur:"$cur_dir; cd $cur_dir; file_name=`date +%Y-%m-%d_%H-%M-%S`;host_name=`hostname`;tcpdump -i any -w $cur_dir/$file_name[$host_name].pcap




case_name="app_to_net"; temp_file_dir=~/temp_`hostname`; mkdir -p $temp_file_dir;scp $temp_file_dir/ root@173.110.101.20:/root/temp_trace_file/
 


trap onCtrlC INT ; function onCtrlC () { echo 'ptured====\r\n'   }


trap "ehco 'program exit...'; exit 2" SIGINT




