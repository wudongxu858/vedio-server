#include "./src/HostAddress.h"
#include "./src/CTcpSocket.h"
#include "./src/CSerSocket.h"
#include "./pack/pack.h"
#include <signal.h>
#include <iostream>
#include <pthread.h>
using namespace std;
void signal_fun(int num)
{
    printf("client get signal:%d\n",num);
}
void *run(void *arg)
{
    char buffer[1024];
    char coutbuff[200]={0};
    CTcpSocket *client_socket=(CTcpSocket *)arg;
    int read_count=0;
    char *temp=buffer;
    while(1)
    {
        temp=buffer;
        read_count = client_socket->Read(buffer, 1024);
        //printf("get cout =%d\n",read_count);
        PACK_RETURN res= PackDel::getPack(read_count,temp);
        memset(res.data+res.lenth,0,PACK_HEAD_SIZE);
        printf("res.lenth =%d get cout =%d data=%s HEAD AND TAIL =%d \n",res.lenth,read_count,res.data,PACK_HEAD_SIZE+PACK_TAIL_SIZE);
        memset(buffer,0,1024);
        //cout<<res.data<<endl;
        /*while(1)
        {
            PACK_RETURN res= PackDel::getPack(read_count,temp);
            if(res.isOver==true)
            {
                char *temp=buffer;
                break;
            }
            memcpy(coutbuff,res.data,res.lenth);
            cout<<coutbuff<<endl;
            memset(buffer, 0, sizeof(buffer));
            memset(coutbuff, 0, sizeof(coutbuff));
        }*/
    }
}
int main()
{
    pthread_t id;
    
    signal(SIGPIPE,signal_fun);
    CTcpSocket client_socket(SOCK_STREAM);
    HostAddress server_addr(9999,(char *)"172.16.16.182");
    client_socket.Connect(server_addr);
    pthread_create(&id,NULL,run,&client_socket);
    char buffer[50]={0};
    int write_count;
    int i=0;
    char sendbuff[200]="";
    while(true)
    {
        if(i>100)
            i=0;
        sprintf(buffer,"-------ok = %d----------\n",i);
        PackDel::setPack(0,strlen(buffer),buffer,sendbuff);
        write_count = client_socket.Write(sendbuff,PACK_HEAD_SIZE+PACK_TAIL_SIZE+strlen(buffer));
      //  printf("%d %s \n",write_count,buffer,sendbuff);
        memset(buffer, 0, sizeof(buffer));
        memset(sendbuff, 0, sizeof(sendbuff));
        i++;
        sprintf(buffer,"-------ok = %d----------\n",i);
        PackDel::setPack(0,strlen(buffer),buffer,sendbuff);
        write_count = client_socket.Write(sendbuff,PACK_HEAD_SIZE+PACK_TAIL_SIZE+strlen(buffer));
      //  printf("%d %s\n",write_count,buffer);
        memset(buffer, 0, sizeof(buffer));
        memset(sendbuff, 0, sizeof(sendbuff));
        i++;
        sprintf(buffer,"-------ok = %d----------\n",i);
        PackDel::setPack(0,strlen(buffer),buffer,sendbuff);
        write_count = client_socket.Write(sendbuff,PACK_HEAD_SIZE+PACK_TAIL_SIZE+strlen(buffer));
     //   printf("%d %s\n",write_count,buffer);
        memset(buffer, 0, sizeof(buffer));
        memset(sendbuff, 0, sizeof(sendbuff));
        i++;
        usleep(1000*10);
    }
    return 0;
}
