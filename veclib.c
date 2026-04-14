#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "veclib.h"

static void init_vec(vector* v){
    v->size=1;
    v->finindex=0;
    v->data=(char**)malloc(sizeof(char*)*v->size);
}

vector* vec_create(){
    vector* v=(vector*)malloc(sizeof(vector));
    init_vec(v);
    return v;
}

int vec_size(vector* v){
    return v->finindex;
}

static void vec_resize(vector *v,int newsize){
    char** newdata=realloc(v->data,newsize*sizeof(char*));
    if(newdata!=NULL){
        v->size=newsize;
        v->data=newdata;
    }
}

void vec_push_back(vector *v,char* str){
    if(v->size<=v->finindex){
        vec_resize(v,v->size*2);
    }
    v->data[v->finindex]=strdup(str);
    v->finindex++;
}

void vec_clear(vector* v){
    char** temp=v->data;
    for(int i=0;i<v->finindex;i++){
        free(*temp);
        temp++;
    }
    v->finindex=0;
}

void vec_free(vector **v){
    char** temp=(*v)->data;
    for(int i=0;i<(*v)->finindex;i++){
        free(*temp);
        temp++;
    }
    free((*v)->data);
    free(*v);
    *v=NULL;
}

void insertsorted(vector* v,char* str){
    if(v->size<=v->finindex){
        vec_resize(v,v->size*2);
    }
    int i=v->finindex-1;
    while(i>=0 && strcmp(v->data[i],str)>0){
        v->data[i+1]=v->data[i];
        i--;
    }
    if(i>=0 && strcmp(str, v->data[i])){
        return;
    }
    i++;
    v->data[i]=strdup(str);
    v->finindex++;
}

char* vec_at(vector* v,int index){
    if(index<0 || index>=v->finindex){
        return NULL;
    }
    else{
        return v->data[index];
    }
}

void vec_delete(vector* v,char* str){
    for(int i=0;i<v->finindex;i++){
        if(!strcmp(str,v->data[i])){
            free(v->data[i]);
            for(int j=i;j<v->finindex-1;j++){
                v->data[j]=v->data[j+1];
            }
            v->finindex--;
            v->data[v->finindex] = NULL;
            return;
        }
    }
}