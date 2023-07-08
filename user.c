#include "user.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include<unistd.h>
#include<errno.h>
#include<pthread.h>

#define BUFFSIZE 2048
#define ERRORCODE -1
#define IDe "lashila"
#define pw "lashila"

static void *thread_send(void *arg)
{
    char buf[BUFFSIZE];
    int sd = *(int *) arg;
    while (1)
    {
        memset(buf, 0, sizeof(buf));
        read(STDIN_FILENO, buf, sizeof(buf));
        if (send(sd, buf, strlen(buf), 0) == -1)
        {
            printf("send error:%s \n", strerror(errno));
            break;
        }
    }
    return NULL;
}
static void *thread_send1(void *arg)
{
    char buf[BUFFSIZE];
    Message message = *(Message *) arg;
    while (1)
    {
//        memset(buf, 0, sizeof(buf));
//        read(STDIN_FILENO, buf, sizeof(buf));

        if (send(message.sd, (char*)&message, sizeof(message), 0) == -1)
        {
            printf("send error:%s \n", strerror(errno));
            break;
        }
        break;
    }
    return NULL;
}
static void *thread_recv(void *arg)
{
    char buf[BUFFSIZE];
    int sd = *(int *) arg;
    while (1)
    {
        memset(buf, 0, sizeof(buf));
        int rv = recv(sd, buf, sizeof(buf), 0);
        if (rv <= 0)
        {
            if(rv == 0) //server socket关闭情况
            {
                printf("server have already full !\n");
                exit(0);//退出整个客服端
            }
            printf("recv error:%s \n", strerror(errno));
            break;
        }
        printf("%s", buf);//输出接收到的内容
    }
    return NULL;
}
static void *thread_recv1(void *arg)
{
    char buf[BUFFSIZE];
    Message message = *(Message *) arg;
    while (1)
    {
        memset(buf, 0, sizeof(buf));
        int rv = recv(message.sd, buf, sizeof(message), 0);
        memcpy(&message,buf, sizeof(message));
        if (rv <= 0)
        {
            if(rv == 0) //server socket关闭情况
            {
                printf("server have already full !\n");
                exit(0);//退出整个客服端
            }
            printf("recv error:%s \n", strerror(errno));
            break;
        }
        printf("%s\n", message.req.pld);//输出接收到的内容
//        element_printf("%B\n",message.req.pw_);
        break;
    }
    return NULL;
}
int run_client(char *ip_str, int port)
{
    element_t tpk,tsk;
    EKeygen(&tpk,&tsk);
    element_t r;
    element_init_Zr(r,PP.pairing);
    element_random(r);
    element_t pw_;
    element_init_G1(pw_,PP.pairing);
    element_t hash;
    H2(hash,pw);
    element_pow_zn(pw_, hash,r);
    element_clear(hash);
    element_printf("tpk = %B\n",tpk);
    //构造消息结构体
    Req req;
    memset(req.ID,0, 64* sizeof(char));
    memset(req.pld,0, 64* sizeof(char));
    memset(req.tpk,0,256*sizeof(char));
    memset(req.pw_,0,256*sizeof(char));
    strcpy(req.ID,IDe);
    element_to_bytes_compressed(req.pw_,pw_);
    strcpy(req.pld,"hello\0");
    element_to_bytes_compressed(req.tpk,tpk);
    element_from_bytes_compressed(tpk,req.tpk);
    element_printf("tpk = %B\n",tpk);


//初始化用户
    int client_sd;
    int con_rv;
    pthread_t thrd1, thrd2;
    struct sockaddr_in client_sockaddr; //定义IP地址结构
    client_sd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sd == -1)
    {
        printf("socket create error:%s \n", strerror(errno));
        return ERRORCODE;
    }
    memset(&client_sockaddr, 0, sizeof(client_sockaddr));
    client_sockaddr.sin_port = htons(port); //指定一个端口号并将hosts字节型传化成Inet型字节型（大端或或者小端问题）
    client_sockaddr.sin_family = AF_INET; //设置结构类型为TCP/IP
    client_sockaddr.sin_addr.s_addr = inet_addr(ip_str);//将字符串的ip地址转换成int型,客服端要连接的ip地址
    con_rv = connect(client_sd, (struct sockaddr*) &client_sockaddr,
                     sizeof(client_sockaddr));
    //调用connect连接到指定的ip地址和端口号,建立连接后通过socket描述符通信
    if (con_rv == -1)
    {
        printf("connect error:%s \n", strerror(errno));
        return ERRORCODE;
    }

    Message message;
    message.sd = client_sd;
    message.req = req;

    if (pthread_create(&thrd1, NULL, thread_send1, &message) != 0)
    {
        printf("thread error:%s \n", strerror(errno));
        return ERRORCODE;
    }

    if (pthread_create(&thrd2, NULL, thread_recv1, &message) != 0)
    {
        printf("thread error:%s \n", strerror(errno));
        return ERRORCODE;
    }
    pthread_join(thrd2, NULL);// 等待线程退出
    pthread_join(thrd1, NULL);






    close(client_sd);
    return 0;
}
int main(int argc, char *argv[])
{
    char buff[129];
    initPP(&PP,param);
    FILE *fp = fopen("P.txt", "rb+");
    fread(buff, sizeof(char),129,fp);
    fclose(fp);
    element_from_bytes_compressed(PP.P,buff);
    element_printf("P = %B\n",PP.P);

    int port = atoi("5001");
    char *ip_str = "127.0.0.1";
    run_client(ip_str,port);
    return 0;
}