
/******************************************************************************

                  版权所有 (C), 2023-2023，刘欣予

 ******************************************************************************
  文 件 名   : Global.c
  版 本 号   : 初稿
  作    者   : 刘欣予
  生成日期   : 2023/5/25
  最近修改   :
  功能描述   : 
  函数列表   :
  修改历史   :
  1.日    期   : 2023/5/25
    作    者   : 刘欣予
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 * #include "Global.h"
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部变量说明                                 *
 * PublicParameter PP;
 * int q_E;
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 * void Sig();//签名
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 * void initPP();
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
#include "Global.h"
#include "string.h"
#include "stdio.h"


PublicParameter PP;

extern void Sig();//签名

/*****************************************************************************
 函 数 名  : initPP
 功能描述  : 生成公共参数
 输入参数  : 自定义变量类型PublicParameter指针和字符串
 输出参数  : 自定义变量类型PublicParameter指针PP
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2023/5/25
    作    者   : liuxinyu
    修改内容   : 新生成函数

*****************************************************************************/
char *param = "type a\n"
              "q 8780710799663312522437781984754049815806883199414208211028653399266475630880222957078625179422662221423155858769582317459277713367317481324925129998224791\n"
              "h 12016012264891146079388821366740534204802954401251311822919615131047207289359704531102844802183906537786776\n"
              "r 730750818665451621361119245571504901405976559617\n"
              "exp2 159\n"
              "exp1 107\n"
              "sign1 1\n"
              "sign0 1";

int t = 3, n = 5;

int num = 16;

void initPP(PublicParameter *PP,char* param){
    pairing_init_set_buf(PP->pairing, param, strlen(param));
    element_init_G1(PP->P,PP->pairing);
    element_random(PP->P);

    return;
}
void genSHA256(unsigned char* output_sha256, unsigned char* msg) {
    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, msg, strlen(msg));
    SHA256_Final(output_sha256, &ctx);
}
void print_hex(unsigned char *buffer, int size) {
    int i;
    printf("******************start hex code**********************************\n");
    for (i = 1; i <= size; i++) {
        printf("0x%02X ",buffer[i-1]);
        if(i % 16 == 0 && i < size) {
            printf("\n");
        }
    }
    printf("\n");
    printf("********************end hex code************************************\n");
}
void printf_buff(char *buff,int size) {
    int i=0;
    for (i=0;i<size;i++) {
        printf("%02X",(unsigned char )buff[i]);
        if((i+1)%64==0)
            putchar('\n');
    }
    printf("\n");
}
void sprintf_buff(char *buff1, char *buff, int size){
    int i=0;
    for (i=0;i<size;i++) {
        sprintf(buff1,"%s%02X",buff1,(unsigned char )buff[i]);
    }
}
char *padding_buf(char *buf,int size,int *final_size) {
    char *ret=NULL;
    int pidding_size=AES_BLOCK_SIZE-(size%AES_BLOCK_SIZE);
    int i;
    *final_size=size+pidding_size;
    ret=(char *)malloc(size+pidding_size);
    memcpy(ret,buf,size);
    if(pidding_size!=0) {
        for (i=size;i<(size+pidding_size);i++) {
            ret[i]=0;
        }
    }
    return ret;
}
unsigned char *str2hex(char *str) {
    unsigned char *ret=NULL;
    int str_len=strlen(str);
    int i=0;
    assert((str_len%2)==0);
    ret=(char *)malloc(str_len/2 +1);
    for (i=0;i<str_len;i+=2) {
        sscanf(str+i,"%2hhx",&ret[i/2]);
    }
    return ret;
}
void encrpyt_buf(char *raw_buf,char **encrpyt_buf,int len, unsigned char *key) {
    AES_KEY aes;
    unsigned char *iv=str2hex("667b02a85c61c786def4521b060265e8");
    AES_set_encrypt_key(key,128,&aes);
    AES_cbc_encrypt(raw_buf,*encrpyt_buf,len,&aes,iv,AES_ENCRYPT);
    free(iv);
}
void decrpyt_buf(char *raw_buf,char **encrypt_buf,int len, unsigned char *key) {
    AES_KEY aes;
    unsigned char *iv=str2hex("667b02a85c61c786def4521b060265e8");
    AES_set_decrypt_key(key,128,&aes);
    AES_cbc_encrypt(raw_buf,*encrypt_buf,len,&aes,iv,AES_DECRYPT);
    free(iv);
}

void e(element_t temp3,element_t temp2){

}
void SecretShare(element_t s,int n,int t,pairing_t pairing){
    element_t *Fx;
    element_t *si;
    element_t temp;
    Fx = (element_t*)malloc(sizeof(element_t)*t);
    si = (element_t*)malloc(sizeof(element_t)*(n+1));
    element_init_Zr(temp,pairing);
    element_init_Zr(Fx[0],pairing);
    element_set(Fx[0],s);
    for (int i = 1; i < t; ++i) {
        element_init_Zr(Fx[i],pairing);
        element_random(Fx[i]);
    }
    for (int i = 1; i < n+1; ++i) {
        element_set0(temp);
        for (int j = 0; j < t; ++j) {

        }

    }
}


void genshare(element_t secret,int n,int t,element_t *result,pairing_t pairing)
{

    element_t *a;//多项式系数a0....a(t-1)
    a = (element_t*)malloc(sizeof(element_t) * (t));
    element_t *share;//分享的密钥
    share = (element_t*)malloc(sizeof(element_t) * (n));

    //element_printf("secret in = %B\n",secret);

    element_init_Zr(a[0],pairing);//a0 = secret

    element_set(a[0],secret);
    //element_printf("a[0] = %B\n",a[0]);

    for(int i=1;i<t;i++)
    {
        element_init_Zr(a[i],pairing);
//        element_set1(a[i]);
        element_random(a[i]);
        //element_printf("a[%d] = %B\n",i,a[i]);
    }

    for(int i=0;i<n;i++)//share初始化
    {
        element_init_Zr(share[i],pairing);
        element_set(share[i],a[0]);
        //element_printf("share[%d] = %B\n",i,share[i]);
    }

    for(int i=1;i<=n;i++)//计算share0...4 sharei对应x=i+1
    {
        element_t tmp;//存储x ... x^(t-1)
        element_t x;

        element_init_Zr(tmp,pairing);
        element_init_Zr(x,pairing);

        element_set_si(x,i);

        for(int j =1;j<t;j++)
        {
            element_t tmp1;//存储a[j]*x^j
            element_t xpowj;
            element_t jz;

            element_init_Zr(tmp1,pairing);
            element_init_Zr(xpowj,pairing);
            element_init_Zr(jz,pairing);

            element_set_si(jz,j);
            element_pow_zn(xpowj,x,jz);

            element_mul(tmp1,a[j],xpowj);
            element_add(share[i-1],share[i-1],tmp1);

            element_set0(tmp1);
        }
    }
    for(int i=0;i<n;i++)//赋值
    {
        element_init_Zr(result[i],pairing);
        element_set(result[i],share[i]);
        element_printf("%B\n",result[i]);
    }
    free(a);
    free(share);


}

void random_select(int* result)
{
    // 初始化数组
    for (int i = 0; i < n; i++) {
        result[i] = i;
    }
    // 随机交换数组中的元素
    srand(time(NULL));
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = result[i];
        result[i] = result[j];
        result[j] = temp;
    }
    // 返回前 t 个元素
    for (int i = 0; i < t; i++) {
        printf("%d ", result[i]);
    }
    printf("\n");
}

void get_lagrange_coefficient(int a,int *shareid,element_t *lagrange_co,pairing_t pairing)
{
    //printf("lagrang\n");
    int thisid = a+1;
    int idgroup[t];
    for(int i=0;i<t;i++)
    {
        idgroup[i]=shareid[i];
        //printf("%d\n",idgroup[i]);
        idgroup[i]++;
    }


    element_t l;
    element_t ll,lr;
    element_t lrn;

    element_init_Zr(l,pairing);
    element_init_Zr(lrn,pairing);

    element_init_Zr(ll,pairing);
    element_init_Zr(lr,pairing);

    signed long int lleft=1;
    for(int i=0;i<t;i++)
    {
        if(idgroup[i]==thisid)
        {
            continue;
        }
        lleft = lleft * (0-idgroup[i]);
    }

    signed long int lright=1;
    for(int i=0;i<t;i++)
    {
        if(idgroup[i]==thisid)
        {
            continue;
        }
        lright = lright * (idgroup[i]-thisid);
    }

    element_set_si(ll,lleft);
    element_set_si(lr,lright);
    element_invert(lrn,lr);

    element_mul(l,ll,lrn);
    element_init_Zr(lagrange_co,pairing);
    element_set(lagrange_co,l);
    element_printf("lagl %B\n",ll);
    element_printf("lagr %B\n",lr);
    element_printf("lagrn %B\n",lrn);
}

void recover_secret(element_t* s,element_t *result,pairing_t pairing)
{
//    int shareid[3] = {3,2,1};
    int shareid[t];
    printf("选择的%d个用于计算w的服务器为:\n",t);
    random_select(&shareid);
//    for(int i=0;i<t;i++)
//    {
//        IS[shareid[i]].chosen=1;
//    }
    // for(int i=0;i<n;i++)
    // {
    //     printf("%d\n",IS[i].chosen);
    // }

    element_t *l;
    l = (element_t*)malloc(sizeof(element_t) * (t));

    for(int i=0;i<t;i++)
    {
        element_init_Zr(l[i],pairing);
        get_lagrange_coefficient(shareid[i],shareid,&l[i],pairing);
        //element_printf("lagrange_co%d=%B\n",i,l[i]);
    }


    element_t *wl;
    wl = (element_t*)malloc(sizeof(element_t) * (t));
    for(int i=0;i<t;i++)
    {
        element_init_Zr(wl[i],pairing);
        element_mul(wl[i],result[shareid[i]],l[i]);
        element_printf("%B\n",wl[i]);
    }
//    element_add(wl[1],wl[0],wl[1]);
//    element_add(wl[2],wl[1],wl[2]);
//    element_printf("wl %B\n",wl[2]);


    element_t *w_compute;//w_c[0]存w1w2,w_c[1]存w1w2w3....w_c[t-2]存最终结果
    w_compute = (element_t*)malloc(sizeof(element_t) * (t-1));
    element_init_Zr(w_compute[0],pairing);
    element_add(w_compute[0],wl[0],wl[1]);
//    element_printf("%B\n",w_compute[0]);

    for(int i=1;i<t-1;i++)
    {
        element_init_Zr(w_compute[i],pairing);
        element_add(w_compute[i],w_compute[i-1],wl[i+1]);
//        element_printf("%B\n",w_compute[i]);
    }

    element_init_Zr(s,pairing);
    element_set(s,w_compute[t-2]);
    element_printf("\nrecover secret = %B\n",s);
//    printf("here1\n");

    free(l);
    free(wl);
    free(w_compute);


}

void EKeygen(element_t* pk,element_t* sk){
    element_init_G1(pk,PP.pairing);
    element_init_Zr(sk,PP.pairing);
    element_random(sk);
    element_pow_zn(pk,PP.P,sk);
    element_printf("sk = %B; pk = %B\n",sk,pk);
}

void EEnc(element_t* C1,element_t* C2,element_t pk,element_t M){
    element_t r;
    element_t tmp;
    element_init_G1(C1,PP.pairing);
    element_init_G1(C2,PP.pairing);
    element_init_Zr(r,PP.pairing);
    element_init_G1(tmp,PP.pairing);
    element_random(r);
    element_pow_zn(C1,PP.P,r);
    element_pow_zn(tmp,pk,r);
    element_add(C2,M,tmp);
    element_clear(r);
    element_clear(tmp);
}

void EDec(element_t *M,element_t sk,element_t C1, element_t C2){
    element_t tmp;
    element_init_G1(tmp,PP.pairing);
    element_init_G1(M,PP.pairing);
    element_pow_zn(tmp,C1,sk);
    element_sub(M,C2,tmp);
    element_clear(tmp);
}

void Ererand(element_t* C1_,element_t* C2_,element_t C1,element_t C2,element_t pk){
    element_t r_;
    element_t tmp1;
    element_t tmp2;
    element_init_Zr(r_,PP.pairing);
    element_init_G1(tmp1,PP.pairing);
    element_init_G1(tmp2,PP.pairing);
    element_mul_zn(tmp1,PP.P,r_);
    element_mul_zn(tmp2,pk,r_);
    element_add(C1_,C1,tmp1);
    element_add(C2_,C2,tmp2);
}

void

