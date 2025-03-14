/*
Group Number - 17
ID:	2022A7PS0173P			Name: Ameesh Sethi
ID:	2022A7PS0097P			Name: Aviral Gupta
ID:	2022A7PS0146P			Name: Yatharth Singh
ID:	2022A7PS1188P			Name: Armaan Sethi
ID:	2022A7PS0052P			Name: Utkarsh Tiwari
*/

#include "trie.h"
#define ALPHABET 27

// Returns a new trie node
trie getTrieNode(void)
{
    trie newNode = (trie)malloc(sizeof(Trie));
    newNode->token = TK_INVALID;
    for (int i = 0; i < ALPHABET; i++)
        newNode->characters[i] = NULL;
    return newNode;
}

// Inserts a key-value pair into the trie
void insert(trie root, const char *key, token_id token)
{
    int i, size = strlen(key), index;
    trie curr = root;
    for (i = 0; i < size; i++)
    {
        if (key[i] == '_')
            index = 26;
        else
            index = (int)key[i] - (int)'a';
        if (!curr->characters[index])
            curr->characters[index] = getTrieNode();
        curr = curr->characters[index];
    }
    curr->token = token;
}

// Searches for a key in the trie
token_id search(trie root, const char *key)
{
    int i, size = strlen(key), index;
    trie curr = root;
    for (i = 0; i < size; i++)
    {
        if (key[i] == '_')
            index = 26;
        else
            index = (int)key[i] - (int)'a';
        if (!curr->characters[index])
            return TK_INVALID;
        curr = curr->characters[index];
    }
    return curr->token;
}
#undef ALPHABET
