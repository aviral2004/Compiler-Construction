/*
Group Number - 17
ID:	2022A7PS0173P			Name: Ameesh Sethi
ID:	2022A7PS0097P			Name: Aviral Gupta
ID:	2022A7PS0146P			Name: Yatharth Singh
ID:	2022A7PS1188P			Name: Armaan Sethi
ID:	2022A7PS0052P			Name: Utkarsh Tiwari
*/

#ifndef TRIE_H
#define TRIE_H
#define ALPHABET 27
#define TRIE_INCOMPLETE
#include "lexerDef.h"

// Trie node
typedef struct Trie
{
    struct Trie *characters[ALPHABET];
    token_id token;
} Trie;
typedef struct Trie *trie;

// Returns a new trie node
trie getTrieNode(void);

// Inserts a key-value pair into the trie
void insert(trie root, const char *key, token_id token);

// Searches for a key in the trie
token_id search(trie root, const char *key);

#undef ALPHABET
#undef TRIE_INCOMPLETE
#endif