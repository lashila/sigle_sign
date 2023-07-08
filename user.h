
/******************************************************************************
 文 件 名   : user.h
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
#ifndef SIGLE_SIGN_USER_H
#define SIGLE_SIGN_USER_H
#include "Global.h"

int initUser(int port);

void* Userreceive(int listenfd, void* m);

void* Usersend(int port,void *m);

int closeUser(int listenfd);


#endif //SIGLE_SIGN_USER_H
