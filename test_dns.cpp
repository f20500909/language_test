#include<iostream>

#include <exception>
#include <string>
#include "memory.h"

#include <arpa/inet.h>

#include "unistd.h"
#include "netdb.h"
#include <netinet/in.h>      

std::string hostname2ip(const char* hostname) {
    char buf[256];
    in_addr ipv4_addr;
    if (::inet_pton(AF_INET, hostname, (void*)&ipv4_addr) == 1) {
        // HTTPUA_DEBUG<<"v4 "<<hostname;
        return std::string{hostname};
    }
    memset(&ipv4_addr,0,sizeof(ipv4_addr));
    if (NULL == hostname) {
        if (gethostname(buf, sizeof(buf)) < 0) {
            // HTTPUA_ERROR<<"gethostname failed "<<hostname;
            return "";
        }
        hostname = buf;
    } else {
        // skip heading space
        for (; isspace(*hostname); ++hostname);
    }

    char aux_buf[1024];
    int error = 0;
    struct hostent ent;
    struct hostent* result = NULL;
    if (gethostbyname_r(hostname, &ent, aux_buf, sizeof(aux_buf),
                        &result, &error) != 0 || result == NULL) {
        // HTTPUA_ERROR<<"gethostbyname_r failed "<<hostname;
        return "";
    }
    // Only fetch the first address here
    bcopy((char*)result->h_addr, (char*)&ipv4_addr, result->h_length);
    char IPdotdec[16]={0};
    if(inet_ntop(AF_INET, (void *)&ipv4_addr, IPdotdec, 16) < 0){
        // HTTPUA_ERROR<<"inet_ntop failed "<<hostname;
    }
    // HTTPUA_DEBUG<<"hostname="<<hostname<<" IPV4="<<(char*)IPdotdec;
    return std::string{IPdotdec};
}

int main(){
    char* test= "maap201.sc.5gmc.189.cn";

    for(int i=0;i<100;i++){
        std::string res = hostname2ip(test);
        std::cout<<res<<std::endl;
    }
    
}