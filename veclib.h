#ifndef VECLIB_H
#define VECLIB_H

typedef struct{
    char **data;
    int finindex;
    int size;
}vector;

vector* vec_create();
int vec_size(vector* v);
void vec_push_back(vector *v,char* str);
void vec_clear(vector* v);
void insertsorted(vector* v,char* str);
void vec_free(vector **v);
char* vec_at(vector* v,int index);
void vec_delete(vector* v,char* str);

#endif