import socket
from socket import *
from time import ctime
import random
 
def generate_random_str(randomlength=16):
    """
    生成一个指定长度的随机字符串
    """
    random_str = ''
    base_str = 'abcdefghigklmnopqrstuvwxyz1234567890'
    length = len(base_str) - 1
    for i in range(randomlength):
        random_str += base_str[random.randint(0, length)]
    return random_str


BUFSIZ = 1024  #接收数据缓冲大小
local_addr =  ('10.8.13.114',43642)
server_addr_p = ('10.14.46.6',5060)
server_addr_s = ('10.14.46.6',5063)

udpSerSock = socket(AF_INET, SOCK_DGRAM) #创建udp服务器套接字
udpSerSock.bind(local_addr)  #套接字与地址绑定



send_data = ""
# send_data = send_data_publish_to_s

# send_data = send_data_subscribe
udpSerSock.sendto(send_data, server_addr_p)  #向客户端发送时间戳数据，必须发送字节数组
# udpSerSock.sendto(send_data, server_addr_s)  #向客户端发送时间戳数据，必须发送字节数组


print("=====================UDP服务器=====================",send_data);
udpSerSock.close()  #关闭服务器socket