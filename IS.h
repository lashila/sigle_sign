
/******************************************************************************
 文 件 名   : IS.h
 版 本 号   : 初稿
 作    者   : 刘欣予
 生成日期   : 2023/7/4
 最近修改   :
 功能描述   :
 函数列表   :
 修改历史   :
 1.日    期   : 2023/7/4
   作    者   : 刘欣予
   修改内容   : 创建文件

******************************************************************************/
#ifndef SIGLE_SIGN_IS_H
#define SIGLE_SIGN_IS_H

typedef struct {
    int fdorport;
    void *m;
}message;

int initIS(int port);

void* ISreceive(int listenfd,int connfd, void* m);

void* ISreceive_thread(void* args);

void* ISsend(int port,void *m);

int closeIS(int listenfd);

#endif //SIGLE_SIGN_IS_H
