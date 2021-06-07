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

#define N 64
void die(char*s){
    perror(s);
    exit(1);
}
int main(int argc,char**argv){
    if (argc!=2){
        printf("usage: ./serv_send <ポート番号>");
    }
    //socket作成
    int s=socket(PF_INET,SOCK_STREAM,0);
    if(s==-1){
        die("Don't make socket");
    }
    
    //bind
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=INADDR_ANY;
    addr.sin_port=htons(atoi(argv[1]));
    int b=bind(s,(struct sockaddr*)&addr,sizeof(addr));
    int yes=1;
    if(setsockopt(s,SOL_SOCKET,SO_REUSEADDR,(const char*)&yes,sizeof(yes))<0){
        die("setsockopt");
    }
    if (b==-1){
        die("not binded");
    }

    //listen
    int l=listen(s,10);
    if(l<0){
        perror("not listened");
    }

    //accept
    struct sockaddr_in client_addr;
    socklen_t len=sizeof(struct sockaddr_in);
    int s2=accept(s,(struct sockaddr*)&client_addr,&len);
    if(l==-1){
        perror("not accepted");
    }

    FILE*fp;
    char*cmd="rec -t raw -b 16 -c 1 -e s -r 44100 - ";
    if((fp=popen(cmd,"r"))==NULL){
        die("not rec");
    }
    unsigned char buf[N];
    // while(!feof(fp)){
    //     fgets(buf,sizeof(buf),fp);
    // }

    while(1){
        // int r=read(buf,data,N);
        // if(r==-1){
        //     die("not read");
        // }
        // if(r==0) break;
        fread(buf,sizeof(char),N,fp);
        int w=write(s2,buf,N);
        if(w==-1) die("write");
        
    }
    pclose(fp);
    int shut=shutdown(s2,SHUT_WR);
    if(shut==-1){
        die("not shutdown");
    }
}