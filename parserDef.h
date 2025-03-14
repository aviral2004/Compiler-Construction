/*
Group Number - 17
ID:	2022A7PS0173P			Name: Ameesh Sethi
ID:	2022A7PS0097P			Name: Aviral Gupta
ID:	2022A7PS0146P			Name: Yatharth Singh
ID:	2022A7PS1188P			Name: Armaan Sethi
ID:	2022A7PS0052P			Name: Utkarsh Tiwari
*/

#ifndef PARSERDEF_H
#define PARSERDEF_H
#include <stdbool.h>
#include "lexerDef.h"
#include "utils.h"

#define MAX_TOKENS 100

// ptokens are the tokens of the grammer
typedef struct ptoken
{
    // name is the index of the token in the grammer
    int name;
    // is_terminal is 1 if the token is a terminal and 0 if it is a non-terminal
    int is_terminal;
} ptoken;

// variable is a RHS of a rule in the grammer
typedef struct variable
{
    // no_of_tokens is the number of tokens in the rule
    int no_of_tokens;
    // rule_no is the index of the rule in the variable
    int rule_no;
    // ptokens are the tokens of the rule
    ptoken *ptokens[MAX_TOKENS];
    // next is the next RHS with the same LHS in the grammer
    struct variable *next;
} variable;

// grammer is the grammer of the language
typedef struct grammer
{
    // no_of_variables is the number of non-terminals in the grammer
    int no_of_variables;
    // no_of_terminals is the number of terminals in the grammer
    int no_of_terminals;
    // no_of_rules is the number of rules in the grammer
    int no_of_rules;
    // terminals are the terminals in the grammer
    int terminals[MAX_TOKENS];
    // start_variable is the index of the start variable in the grammer
    int start_variable[MAX_TOKENS];
    // variables are the RHSs of the rule with start_variable as LHS
    variable *variables[MAX_TOKENS];
} grammer;

// first_follow_element is the first and follow set of a non-terminal
typedef struct first_follow_element
{
    // no_of_first is the number of tokens in the first set
    int no_of_first;
    // no_of_follow is the number of tokens in the follow set
    int no_of_follow;
    // first is the first set of the non-terminal
    int first[MAX_TOKENS];
    // first_rule is the index of the rule in the grammer for each token in first
    int first_rule[MAX_TOKENS];
    // follow is the follow set of the non-terminal
    int follow[MAX_TOKENS];
    // follow_rule is the index of the rule in the grammer for each token in follow
    int follow_rule[MAX_TOKENS];
} first_follow_element;

// FirstAndFollow is the first and follow set of the language
typedef struct FirstAndFollow
{
    // no_of_terminals is the number of terminals in the language
    int no_of_terminals;
    // no_of_variables is the number of non-terminals in the language
    int no_of_variables;
    // start_variable is the index of the start variable in the language
    int start_variable[MAX_TOKENS];
    // terminals are the terminals in the language
    int terminals[MAX_TOKENS];
    // first_follow_element is the first and follow set of each non-terminal
    first_follow_element elements[MAX_TOKENS];
} FirstAndFollow;

// table is the parsing table of the language
typedef struct table
{
    // no_of_rows is the number of non-terminals in the language
    int no_of_rows;
    // no_of_columns is the number of terminals in the language
    int no_of_columns;
    // row is the index of the non-terminal in the language
    int row[MAX_TOKENS];
    // column is the index of the terminal in the language
    int column[MAX_TOKENS];
    // table is the parsing table
    int table[MAX_TOKENS][MAX_TOKENS];
} table;

// parseTree is the parse tree of the language
typedef struct parseTree
{
    // t is the token of the node
    token t;
    // parent is the index of the parent in the parse tree
    int parent;
    // no_of_children is the number of children of the node
    int no_of_children;
    // children are the children of the node
    struct parseTree *children[MAX_TOKENS];
} parseTree;

#endif
