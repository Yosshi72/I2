#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<netinet/tcp.h>
#include<errno.h>
#include<unistd.h>

#define N 1000
void die(char*s){
    perror(s);
    exit(1);
}
int main(int argc,char**argv){
    //socket作成
    int s=socket(PF_INET,SOCK_DGRAM,0);
    if(s==-1){
        die("not socket");
    }
    //接続する
    struct sockaddr_in addr;
    socklen_t addr_size;
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=inet_addr(argv[1]);
    addr.sin_port=htons(atoi(argv[2]));

    unsigned char data[N];
    //送信
    int n=0;
    while(n<50){
        int r=read(0,data,N);
        if(r==-1){
            die("read");
        }
        if(r==0) break;
        
        int w=sendto(s,&data,sizeof(data),0,(struct sockaddr*)&addr,sizeof(addr));
        if(w==-1) die("write");
        n++;
    }
    // int shut=shutdown(s,SHUT_WR);
    // if(shut==-1){
    //     die("shutdown");
    // }
    // int ret=bind(s,(struct sockaddr*)&addr,sizeof(addr));
    // int yes=1;
    // if(setsockopt(s,SOL_SOCKET,SO_REUSEADDR,(const char*)&yes,sizeof(yes))<0){
    //     die("setsockopt");
    // }
    // printf("%d\n",ret);
    // if (ret==-1){
    //     close(s);
    //     die("not bind");
    // }
    while(1){
        int recv=recvfrom(s,&data,sizeof(data),0,(struct sockaddr*)&addr,&addr_size);
        if(recv==-1){
            die("not recieve");
        }
        if(recv==1)break;
        
        int c=0;
        while((c<N) &&(data[c]=='1')){
            c+=1;
        }
        if(c==N){
            break;
        }
        write(1,data,recv);
    }
    close(s);
    if(s==-1){
        die("not closed");
    }
    return 0;
}