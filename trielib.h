#ifndef TRIELIB_H
#define TRIELIB_H

#define MAX_WORD_LENGTH 100
#include "veclib.h"

typedef struct TrieNode{

    struct TrieNode *next_arr[26];
    int isLeaf;

}TrieNode;

TrieNode* create_node();
void insert(TrieNode *root , char word[]);
void delete(TrieNode *root , char word[]);
int search(TrieNode* root, char word[]);
void Trie_Destroy(TrieNode** root);
vector* prefix_search(TrieNode* root, char word[]);
vector* word_correction(TrieNode* root, char* word, int dist);
int wordcount(TrieNode* root);
int max_depth(TrieNode* root);
int node_count(TrieNode* root);
void alpha_distri(TrieNode* root, int arr[]);

#endif