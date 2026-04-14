#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trielib.h"
#include "veclib.h"

static int min(int a, int b) {
    if(a>=b) return b;
    else return a;
}

static int max(int a, int b) {
    if(a<=b) return b;
    else return a;
}

static void initialise_node(TrieNode *node){
    node->isLeaf = 0;

    for(int ii=0 ; ii<26 ; ii++){
        (node->next_arr)[ii] = NULL;
    }

    return;
}

TrieNode* create_node(){
    TrieNode* node=(TrieNode*)malloc(sizeof(TrieNode));
    initialise_node(node);
    return node;
}

void insert(TrieNode *root , char word[]){
    int N = strlen(word);
    TrieNode *curr = root;

    int i = 0;
    while(i<N){

        if(curr->next_arr[word[i]-'a'] != NULL){
            curr = curr->next_arr[word[i]-'a'];
            i++;
        }
        else{
            curr->next_arr[word[i]-'a'] = create_node();
            curr = curr->next_arr[word[i]-'a'];
            i++;
        }
    }

    curr->isLeaf = 1;
    return;
}

static int rec_delete(TrieNode *root , char word[] , int i , TrieNode *head){
    if(i == strlen(word)){

        root->isLeaf = 0;
        int flag = 0;

        for(int ii = 0 ; ii<26 ; ii++){
            if((root->next_arr)[ii] != NULL){
                flag = 1;
                break;
            }
        }

        if(flag == 1){
            return 0;
        }
        else{
            free(root);
            return 1;
        }

    }
    else{

        int rv = rec_delete((root->next_arr)[word[i]-'a'] , word , i+1 , head);
        
        if(rv == 1){
            (root->next_arr)[word[i]-'a'] = NULL;
        }
        
        int flag = 0;
        for(int ii = 0 ; ii<26 ; ii++){
            if((root->next_arr)[ii] != NULL){
                flag = 1;
                break;
            }
        }

        if(flag == 1){
            return 0;
        }
        else{
            if(root->isLeaf == 0 && root != head){
                free(root);
                return 1;
            }
            else{
                return 0;
            }
        }
    }

}

void delete(TrieNode *root , char word[]){
    rec_delete(root , word , 0, root);    
}

static int rec_search(TrieNode* root, char word[], int i){ 
    if(i==strlen(word)){

        if(root->isLeaf==1){
            return 1;
        }
        else{
            return 0;
        }
    }
    else{

        if((root->next_arr)[word[i]-'a']==NULL){
            return 0;
        }
        else{
            return rec_search((root->next_arr)[word[i]-'a'], word, i+1);
        }
    }
}

int search(TrieNode* root, char word[]){
    int rv=rec_search(root, word, 0);
    return rv;
}

void word_finder(TrieNode* root, char word[], int i, vector* v){
    if(root->isLeaf==1){
        word[i]='\0';
        vec_push_back(v, word);
    }

    for(int ii=0;ii<26;ii++){
        if(root->next_arr[ii]!=NULL){
            word[i]='a'+ii;
            word_finder(root->next_arr[ii], word, i+1, v);
        }
    }
}

vector* prefix_search(TrieNode* root, char word[]){
    vector* v=vec_create();
    int len = strlen(word);
    TrieNode* curr=root;

    for(int i=0;i<len;i++){
        if((curr->next_arr)[word[i] - 'a']==NULL){
            return v;
        }
        curr=curr->next_arr[word[i] - 'a'];
    }

    char buf[MAX_WORD_LENGTH+1];
    strcpy(buf,word);
    word_finder(curr, buf, len, v);
    return v;
}

static void rec_destroy(TrieNode* root){
    if(root==NULL) return;
    
    for(int i=0;i<26;i++){
        if(root->next_arr[i]!=NULL){
            rec_destroy(root->next_arr[i]);
        }
    }

    free(root);
}

void Trie_Destroy(TrieNode** root){
    TrieNode* node=*root;
    rec_destroy(node);
    *root=NULL;
}

void rec_word_correction(vector* v,TrieNode* node,char* word,int len,char* curr_word,int* prev_row, int dist, int depth){
    int row[len+1];
    row[0]=prev_row[0]+1;

    int x;
    for(int i=1;i<=len;i++){
        if(word[i-1]==curr_word[depth-1]){
            x=prev_row[i-1];
        }
        else{
            x=prev_row[i-1]+1;
        }
        row[i]=min(row[i-1]+1,min(prev_row[i]+1,x));
    }
    
    if(row[len]<=dist && node->isLeaf){
        curr_word[depth]='\0';
        vec_push_back(v, curr_word);
    }
    
    int mini=row[0];
    for(int i=1;i<=len;i++){
        mini=min(mini,row[i]);
    }

    if(mini<=dist){
        for(int i=0;i<26;i++){
            if(node->next_arr[i]!=NULL){
                curr_word[depth]='a'+i;
                rec_word_correction(v, node->next_arr[i], word, len, curr_word, row, dist, depth+1);
            }
        }
    }
}

vector* word_correction(TrieNode* root, char* word, int dist){
    vector* v=vec_create();
    int len=strlen(word);
    int row[len+1];

    for(int i=0;i<=len;i++){
        row[i]=i;
    }

    char curr_word[MAX_WORD_LENGTH];
    for(int i=0;i<26;i++){
        if(root->next_arr[i]!=NULL){
            curr_word[0]='a'+i;
            rec_word_correction(v, root->next_arr[i], word, len, curr_word, row, dist, 1);
        }
    }
    return v;
}

int wordcount(TrieNode* root){
    if (root == NULL) {
        return 0;
    }
    
    int sum=0;
    for(int i=0;i<26;i++){
        if(root->next_arr[i]==NULL){
            continue;
        }
        sum+=wordcount(root->next_arr[i]);
    }

    if(root->isLeaf){
        sum+=1;
    }

    return sum;
}

int max_depth(TrieNode* root){
    if (root == NULL) {
        return 0;
    }
    
    int maxdepth=0;
    
    for(int i=0;i<26;i++){
        if(root->next_arr[i]==NULL){
            continue;
        }
        maxdepth=max(maxdepth,max_depth(root->next_arr[i]));
    }
    
    return 1+maxdepth;
}

int node_count(TrieNode* root){
    if (root == NULL) {
        return 0;
    }
    
    int sum=1;
    for(int i=0;i<26;i++){
        if(root->next_arr[i]==NULL){
            continue;
        }
        sum+=node_count(root->next_arr[i]);
    }

    return sum;
}

void alpha_distri(TrieNode* root, int arr[]){
    if (root == NULL) {
        return;
    }

    for(int i=0;i<26;i++){
        if(root->next_arr[i]==NULL){
            arr[i]=0;
            continue;
        }

        arr[i]=wordcount(root->next_arr[i]);
    }
}