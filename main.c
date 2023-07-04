#include <stdio.h>
#include "Global.h"



int main() {
    printf("Hello, World!\n");

    initPP(&PP, param);

    element_t secret;
    element_t *result;
    element_t s;
    result = (element_t*) malloc(sizeof(element_t)*n);
    element_init_Zr(secret,PP.pairing);
    element_set_si(secret,10086);
    element_printf("%B\n",secret);
    genshare(secret,n,t,result,PP.pairing);
    recover_secret(&s,result,PP.pairing);

    element_t tmp;
    char* string;
    string = (char*)malloc(sizeof(char)*256);
    element_init_G1(tmp,PP.pairing);
    element_from_bytes(tmp,"123\n");
//    element_to_bytes(string,tmp);
//    printf("%s",string);
    element_t tmp2;
    element_init_G1(tmp2,PP.pairing);

    element_t pk;
    element_t sk;
    EKeygen(&pk,&sk);

    element_t C1;
    element_t C2;
    EEnc(&C1,&C2,pk,tmp);
    EDec(&tmp2,sk,C1,C2);
    element_to_bytes(string,tmp2);
    printf("%s",string);


    return 0;
}
