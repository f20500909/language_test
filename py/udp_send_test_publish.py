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

branch="z9hG4bK-524287-1---"+generate_random_str(16)
send_data_publish_to_p = bytes(f'''PUBLISH sip:88116000040@ims.mnc008.mcc468.3gppnetwork.org;transport=UDP SIP/2.0\r\n\
Via: SIP/2.0/UDP 10.8.13.114:43642;branch={branch}\r\n\
Max-Forwards: 70\r\n\
Route: <sip:orig@5gscscf.ims.mnc008.mcc468.3gppnetwork.org:5063;lr>\r\n\
Contact: <sip:88116000040@10.8.13.114:43642;transport=UDP>\r\n\
To: <sip:88116000040@ims.mnc008.mcc468.3gppnetwork.org;transport=UDP>\r\n\
From: <sip:88116000040@ims.mnc008.mcc468.3gppnetwork.org;transport=UDP>;tag=e8502678\r\n\
Call-ID: qW7lphZz7dnNvSubkh5eKA..\r\n\
CSeq: 2 PUBLISH\r\n\
Expires: 600\r\n\
User-Agent: Z 3.15.40006 rv2.8.20\r\n\
Event: presence\r\n\
Allow-Events: presence, kpml, talk\r\n\
Content-Length: 0\r\n\
\r\n\
\r\n\
''', 'utf-8')

send_data_publish_to_s = bytes('''PUBLISH sip:88116000040@ims.mnc008.mcc468.3gppnetwork.org;transport=UDP SIP/2.0\r\n\
Via: SIP/2.0/UDP 10.8.13.114:43642;branch=z9hG4bK-524287-1---f6f2efaf328b566d\r\n\
Max-Forwards: 70\r\n\
Route: <sip:orig@5gscscf.ims.mnc008.mcc468.3gppnetwork.org:5063;lr>\r\n\
Contact: <sip:88116000040@10.8.13.114:43642;transport=UDP>\r\n\
To: <sip:88116000040@ims.mnc008.mcc468.3gppnetwork.org;transport=UDP>\r\n\
From: <sip:88116000040@ims.mnc008.mcc468.3gppnetwork.org;transport=UDP>;tag=e8502678\r\n\
Call-ID: qW7lphZz7dnNvSubkh5eKA..\r\n\
CSeq: 2 PUBLISH\r\n\
Expires: 600\r\n\
User-Agent: Z 3.15.40006 rv2.8.20\r\n\
Event: presence\r\n\
Allow-Events: presence, kpml, talk\r\n\
Content-Length: 0\r\n\
\r\n\
\r\n\
''', 'utf-8')

send_data_subscribe = bytes('''SUBSCRIBE sip:88116000040@ims.mnc008.mcc468.3gppnetwork.org;transport=UDP SIP/2.0\r\n\
Via: SIP/2.0/UDP 10.8.13.114:43642;branch=z9hG4bK-524287-1---ed67cd6e5f0939ba\r\n\
Max-Forwards: 70\r\n\
Contact: <sip:88116000040@10.8.13.114:43642;transport=UDP>\r\n\
To: <sip:88116000040@ims.mnc008.mcc468.3gppnetwork.org;transport=UDP>\r\n\
From: <sip:88116000040@ims.mnc008.mcc468.3gppnetwork.org;transport=UDP>;tag=b0723c47\r\n\
Call-ID: 8XCsf1b-Sn7JwsHFi204Zg..\r\n\
CSeq: 1 SUBSCRIBE\r\n\
Expires: 3600\r\n\
Accept: application/simple-message-summary\r\n\
User-Agent: Z 3.15.40006 rv2.8.20\r\n\
Event: message-summary\r\n\
Allow-Events: presence, kpml, talk\r\n\
Content-Length: 0\r\n\
\r\n\
\r\n\
''', 'utf-8')

send_data = send_data_publish_to_p
# send_data = send_data_publish_to_s

# send_data = send_data_subscribe
udpSerSock.sendto(send_data, server_addr_p)  #向客户端发送时间戳数据，必须发送字节数组
# udpSerSock.sendto(send_data, server_addr_s)  #向客户端发送时间戳数据，必须发送字节数组


print("=====================UDP服务器=====================",send_data);
udpSerSock.close()  #关闭服务器socket