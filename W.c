#include "W.h"
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
#include "Global.h"
#define MAXCONN 2
#define ERRORCODE -1
#define BUFFSIZE 2048
int count_connect = 0;
Message topmessage;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
struct pthread_socket
{
    int socket_d;
    pthread_t thrd;
    Message message;
};
static void *thread_send(void *arg)
{
    char buf[BUFFSIZE];
    int sd = *(int *) arg;
    memset(buf, 0, sizeof(buf));
    strcpy(buf, "link success! \n");
    if (send(sd, buf, strlen(buf), 0) == -1)
    {
        printf("send error:%s \n", strerror(errno));
        return NULL;
    }
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
    struct pthread_socket *pt = (struct pthread_socket *) arg;
    while (1)
    {
//        memset(buf, 0, sizeof(buf));
//        read(STDIN_FILENO, buf, sizeof(buf));
        if (send(pt->socket_d, (char*)&(pt->message), sizeof(pt->message), 0) == -1)
        {
            printf("send error:%s \n", strerror(errno));
            break;
        }
        break;
    }
//    close(pt->socket_d);
    return NULL;
}
static void* thread_recv(void *arg)
{
    char buf[BUFFSIZE];
    struct pthread_socket *pt = (struct pthread_socket *) arg;
    int sd = pt->socket_d;
    pthread_t thrd = pt->thrd;
    while (1)
    {
        memset(buf, 0, sizeof(buf));
        int rv = recv(sd, buf, sizeof(buf), 0); //是阻塞的
        if (rv < 0)
        {
            printf("recv error:%s \n", strerror(errno));
            break;
        }
        if (rv == 0) // 这种情况说明client已经关闭socket连接
        {
            break;
        }
        printf("%s", buf); //输出接受到内容
    }
    pthread_cancel(thrd);
    pthread_mutex_lock(&mutex);
    count_connect--;
    pthread_mutex_unlock(&mutex);
    close(sd);
    return NULL;
}
static void* thread_recv1(void *arg)
{
    char buf[BUFFSIZE];
    Message message;
    struct pthread_socket *pt = (struct pthread_socket *) arg;
    int sd = pt->socket_d;
    pthread_t thrd = pt->thrd;
    while (1)
    {
        memset(buf, 0, sizeof(buf));
        int rv = recv(sd, buf, sizeof(message), 0); //是阻塞的
        memcpy(&message,buf, sizeof(message));
        if (rv < 0)
        {
            printf("recv error:%s \n", strerror(errno));
            break;
        }
        if (rv == 0) // 这种情况说明client已经关闭socket连接
        {
            break;
        }
        printf("%s\n", message.req.pld); //输出接受到内容
//        element_printf("%B\n",message.req.pw_);
        break;
    }
    memcpy(&(pt->message),buf, sizeof(message));
    pthread_mutex_lock(&mutex);
    count_connect--;
    pthread_mutex_unlock(&mutex);
    return NULL;
}

static int create_listen(int port)
{

    int listen_st;
    struct sockaddr_in sockaddr; //定义IP地址结构
    int on = 1;
    listen_st = socket(AF_INET, SOCK_STREAM, 0); //初始化socket
    if (listen_st == -1)
    {
        printf("socket create error:%s \n", strerror(errno));
        return ERRORCODE;
    }
    if (setsockopt(listen_st, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1) //设置ip地址可重用
    {
        printf("setsockopt error:%s \n", strerror(errno));
        return ERRORCODE;
    }
    sockaddr.sin_port = htons(port); //指定一个端口号并将hosts字节型传化成Inet型字节型（大端或或者小端问题）
    sockaddr.sin_family = AF_INET;    //设置结构类型为TCP/IP
    sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);    //服务端是等待别人来连，不需要找谁的ip
    //这里写一个长量INADDR_ANY表示server上所有ip，这个一个server可能有多个ip地址，因为可能有多块网卡
    if (bind(listen_st, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) == -1)
    {
        printf("bind error:%s \n", strerror(errno));
        return ERRORCODE;
    }

    if (listen(listen_st, 5) == -1) //     服务端开始监听
    {
        printf("listen error:%s \n", strerror(errno));
        return ERRORCODE;
    }
    return listen_st;
}

int accept_socket(int listen_st)
{
    int accept_st;
    struct sockaddr_in accept_sockaddr; //定义accept IP地址结构
    socklen_t addrlen = sizeof(accept_sockaddr);
    memset(&accept_sockaddr, 0, addrlen);
    accept_st = accept(listen_st, (struct sockaddr*) &accept_sockaddr,&addrlen);
    //accept 会阻塞直到客户端连接连过来 服务端这个socket只负责listen 是不是有客服端连接过来了
    //是通过accept返回socket通信的
    if (accept_st == -1)
    {
        printf("accept error:%s \n", strerror(errno));
        return ERRORCODE;
    }
    printf("accpet ip:%s \n", inet_ntoa(accept_sockaddr.sin_addr));
    return accept_st;
}
int run_W(int listenport,char *ip_str, int serverport)
{

    Sharemessage sharemessage1[n];
    Si si1[n];
    Res res[n];
    for (int i = 0; i < n; ++i) {
        memset(&sharemessage1[i],0, sizeof(Sharemessage));
        memset(&si1[i],0, sizeof(Si));
        memset(&res[i],0, sizeof(Res));
    }
    FILE *fp = fopen("sharemessageki.txt", "rb+");
    fread(sharemessage1, sizeof(Sharemessage),n,fp);
    fclose(fp);
    fp = fopen("si.txt", "rb+");
    fread(si1, sizeof(char)*128,n,fp);
    fclose(fp);
    element_t ppk;
    element_init_G1(ppk,PP.pairing);
    element_from_bytes_compressed(ppk,sharemessage1[0].ppk);

    int listen_st = create_listen(listenport);    //创建监听socket
    pthread_t send_thrd, recv_thrd;
    struct pthread_socket ps;
    struct pthread_socket ps1;
    int accept_st;
    if (listen_st == -1)
    {
        return ERRORCODE;
    }
    //初始化成功
    printf("server start \n");
    while (1)
    {
        accept_st = accept_socket(listen_st); //获取连接的的socket
        if (accept_st == -1)
        {
            return ERRORCODE;
        }
        if (count_connect >= MAXCONN)
        {
            printf("connect have already be full! \n");
            close(accept_st);
            continue;
        }
        pthread_mutex_lock(&mutex);
        count_connect++;
        pthread_mutex_unlock(&mutex);
//U->W
        ps.socket_d = accept_st;
        ps.thrd = send_thrd;
        if (pthread_create(&recv_thrd, NULL, thread_recv1, &ps) != 0)//创建接收信息线程
        {
            printf("create thread error:%s \n", strerror(errno));
            break;
        }
//        pthread_detach(recv_thrd); //设置线程为可分离，这样的话，就不用pthread_join
        pthread_join(recv_thrd,NULL);

        element_t k,y;
        element_t tpk,tpk_;
        element_t pw,pw_;
        element_init_Zr(k,PP.pairing);
        element_init_Zr(y,PP.pairing);
        element_init_G1(tpk_,PP.pairing);
        element_init_G1(tpk,PP.pairing);
        element_init_G1(pw_,PP.pairing);
        element_init_G1(pw,PP.pairing);
        element_random(k);
        element_random(y);
        element_from_bytes_compressed(tpk,ps.message.req.tpk);
        element_printf("tpk = %B\n",tpk);
        element_from_bytes_compressed(pw,ps.message.req.pw_);
        EKM(&tpk_,tpk,y);
        element_pow_zn(pw_,pw,k);
        element_printf("tpk* = %B\n",tpk_);

//W->IS

        //初始化防火墙


        for (int i = 0; i < n; ++i) {
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
            client_sockaddr.sin_port = htons(serverport); //指定一个端口号并将hosts字节型传化成Inet型字节型（大端或或者小端问题）
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

            ps1 = ps;
            ps1.socket_d = client_sd;
            ps1.message.sd = i;
            element_to_bytes_compressed(ps1.message.req.tpk,tpk_);
            element_to_bytes_compressed(ps1.message.req.pw_,pw_);

            if (pthread_create(&thrd1, NULL, thread_send1, &ps1) != 0)
            {
                printf("thread error:%s \n", strerror(errno));
                return ERRORCODE;
            }
            if (pthread_create(&thrd2, NULL, thread_recv1, &ps1) != 0)
            {
                printf("thread error:%s \n", strerror(errno));
                return ERRORCODE;
            }

            pthread_join(thrd1,NULL);
            pthread_join(thrd2,NULL);

            close(client_sd);

            element_t C1_,C2_,C1,C2;
            element_init_G1(C1,PP.pairing);
            element_init_G1(C2,PP.pairing);
            element_init_G1(C1_,PP.pairing);
            element_init_G1(C2_,PP.pairing);
            element_from_bytes_compressed(C1,ps1.message.res.C1);
            element_from_bytes_compressed(C2,ps1.message.res.C2);
            Ererand(&C1_,&C2_,C1,C2,ppk);
            element_to_bytes_compressed(res[i].C1,C1_);
            element_to_bytes_compressed(res[i].C2,C2_);

            element_t sigmai_,sigmai;
            element_init_G1(sigmai_,PP.pairing);
            element_init_G1(sigmai,PP.pairing);
            element_from_bytes_compressed(sigmai_,ps1.message.res.sigmai);
            element_t k_invert;
            element_init_Zr(k_invert,PP.pairing);
            element_invert(k_invert,k);
            element_pow_zn(sigmai,sigmai_, k_invert);
            element_to_bytes_compressed(res[i].sigmai,sigmai);
        }
//      再次启动
        int listen_st2 = create_listen(listenport);    //创建监听socket
        pthread_t send_thrd2, recv_thrd2;
        struct pthread_socket ps2;
        int accept_st2;
        if (listen_st2 == -1)
        {
            return ERRORCODE;
        }
        //初始化成功
        printf("server start \n");
        while (1)
        {
            accept_st2 = accept_socket(listen_st2); //获取连接的的socket
            if (accept_st2 == -1)
            {
                return ERRORCODE;
            }
            if (count_connect >= MAXCONN)
            {
                printf("connect have already be full! \n");
                close(accept_st2);
                continue;
            }
            pthread_mutex_lock(&mutex);
            count_connect++;
            pthread_mutex_unlock(&mutex);}



        strcpy(ps.message.req.pld,"-----------Standing by!----------");
        if (pthread_create(&send_thrd, NULL, thread_send1, &ps) != 0) //创建发送信息线程
        {
            printf("create thread error:%s \n", strerror(errno));
            break;

        }
//        pthread_detach(send_thrd);        //设置线程可分离性，这样的话主线程就不用join
        pthread_join(send_thrd,NULL);
        close(accept_st);//关闭之前的接口
    }

//    close(accept_st);
    close(listen_st);
    return 0;
}
//server main
int main(int argc, char *argv[])
{
//    if (argc < 2)
//    {
//        printf("Usage:port,example:8080 \n");
//        return -1;
//    }
    char buff[129];
    initPP(&PP,param);
    FILE *fp = fopen("P.txt", "rb+");
    fread(buff, sizeof(char),129,fp);
    fclose(fp);
    element_from_bytes_compressed(PP.P,buff);
    element_printf("P = %B\n",PP.P);
    int listenport = atoi("5001");
    int serverport = atoi("5003");

    char *ip_str = "127.0.0.1";
    if (listenport == 0)
    {
        printf("port error! \n");
    }
    else
    {
        run_W(listenport,ip_str,serverport);
    }
    return 0;
}