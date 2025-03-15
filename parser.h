/*
Group Number - 17
ID:	2022A7PS0173P			Name: Ameesh Sethi
ID:	2022A7PS0097P			Name: Aviral Gupta
ID:	2022A7PS0146P			Name: Yatharth Singh
ID:	2022A7PS1188P			Name: Armaan Sethi
ID:	2022A7PS0052P			Name: Utkarsh Tiwari
*/

#ifndef PARSER_H
#define PARSER_H
#ifndef UTILS_H
#include "utils.h"
#endif
#include "lexerDef.h"
#include "parserDef.h"

// Function to add a rule to the grammar
void add_grammar_rule(grammar *G, int start_variable, int no_of_tokens, int *ptokens);

// Function to initialize the grammar
void init(grammar *G);

// Function to compute first and follow sets
FirstAndFollow ComputeFirstAndFollowSets(grammar G);

// Function to create parse table
bool createParseTable(FirstAndFollow F, table *T);

// Function to parse the input source code
parseTree *parseInputSourceCode(table T, FirstAndFollow F, grammar *G, vector input);

// Function to print the parse tree
void printParseTree(parseTree *ptree, FILE *fp);
#endif
