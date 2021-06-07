#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<netinet/tcp.h>
#include<errno.h>
#include<unistd.h>

#define N 64

void die(char*s){
    perror(s);
    exit(1);
}

int main(int argc,char**argv){
    //socket作成
    int s=socket(PF_INET,SOCK_STREAM,0);
    if(s==-1){
        die("socket");
    }

    //接続する
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=inet_addr(argv[1]);
    // int aton=inet_aton(argv[1],&addr.sin_addr);
    // if(aton==0){
    //     die("inet_aton");
    // }
    addr.sin_port=htons(atoi(argv[2]));
    int ret=connect(s,(struct sockaddr*)&addr,sizeof(addr));
    if (ret==-1){
        die("connect");
    }
    //データの送受信
    unsigned char data[N];
    //送信
    while(1){
        int r=read(0,data,N);
        if(r==-1){
            die("read");
        }
        if(r==0) break;
        
        int w=write(s,data,N);
        if(w==-1) die("write");
        
    }
    int shut=shutdown(s,SHUT_WR);
    if(shut==-1){
        die("shutdown");
    }

    //受信
    while(1){
        int r=recv(s,data,N,0);
        if(r==-1){
            die("read_s");
        }
        if(r==0)break;
        
        int w=write(1,data,N);
        if(w==-1) die("write");
    }
    
    //ファイルを閉じる
    close(s);
    if(s==-1){
        die("close");
    }
    return 0;
}