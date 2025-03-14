/*
Group Number - 17
ID:	2022A7PS0173P			Name: Ameesh Sethi
ID:	2022A7PS0097P			Name: Aviral Gupta
ID:	2022A7PS0146P			Name: Yatharth Singh
ID:	2022A7PS1188P			Name: Armaan Sethi
ID:	2022A7PS0052P			Name: Utkarsh Tiwari
*/

#ifndef VECTOR_H
#define VECTOR_H
#define VECTOR_INCOMPLETE
#include "lexerDef.h"

typedef struct Vector
{
    token *data;
    int size;
    int capacity;
} Vector;

typedef Vector *vector;

// Initializes a vector
vector init_vector();

// Pushes data to the end of the vector
void push_back(vector v, token data);

// Returns the element at index i
token get(vector v, int index);
#undef VECTOR_INCOMPLETE
#endif