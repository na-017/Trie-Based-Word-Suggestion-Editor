#include "trielib.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define MAX_WORD_LENGTH 100

char *trim(char *s) {
    char *ptr;
    if (!s) return NULL;
    while (isspace((unsigned char)*s)) s++; 
    if (*s == 0) return s;                 
    ptr = s + strlen(s) - 1;
    while (ptr > s && isspace((unsigned char)*ptr)) ptr--; 
    ptr[1] = '\0';                          
    return s;
}

int main(int arg_v,char* arg_c[]){
    if(arg_v!=3){
        printf("wrong usage\n");
    }
    FILE* instream=fopen(arg_c[1],"r");
    FILE* outstream=fopen(arg_c[2],"w");
    char c[MAX_WORD_LENGTH+1];
    char* ptr;
    char* ptr1;
    while(fgets(c,MAX_WORD_LENGTH,instream)){
        if(c[strlen(c)-1]=='\n')c[strlen(c)-1]='\0';
        ptr=trim(c);
        ptr1=ptr;
        int flag=0;
        for(int i=0;i<strlen(ptr);i++){
            if(ptr[i]-'a'>=0 && ptr[i]-'a'<=25) continue;
            if(ptr[i]-'A'>=0 && ptr[i]-'A'<=25){
                ptr[i]+='a'-'A';
                continue;
            }
            flag=1;
            break;
        }
        if(flag)continue;
        fprintf(outstream, "%s\n", ptr1);
    }
}