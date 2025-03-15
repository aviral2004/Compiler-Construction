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

// ff_element is the first and follow set of a non-terminal
typedef struct ff_element
{
    // no_of_first is the number of tokens in the first set
    int first_set_size;
    // no_of_follow is the number of tokens in the follow set
    int follow_set_size;
    // first is the first set of the non-terminal
    int first[MAX_TOKENS];
    // follow is the follow set of the non-terminal
    int follow[MAX_TOKENS];
    // first_rule is the index of the rule in the grammer for each token in first
    int first_set_rule[MAX_TOKENS];
    // follow_rule is the index of the rule in the grammer for each token in follow
    int follow_set_rule[MAX_TOKENS];
} ff_element;

// table is the parsing table of the language
typedef struct table
{
    // row is the index of the non-terminal in the language
    int rows[MAX_TOKENS];
    // column is the index of the terminal in the language
    int cols[MAX_TOKENS];
    // table is the parsing table
    int table[MAX_TOKENS][MAX_TOKENS];
    // no_of_rows is the number of non-terminals in the language
    int row_cnt;
    // no_of_columns is the number of terminals in the language
    int col_cnt;
} table;

// gtokens are the tokens of the grammer
typedef struct gtoken
{
    // name is the index of the token in the grammer
    int tk_name;
    // is_terminal is 1 if the token is a terminal and 0 if it is a non-terminal
    int terminal;
} gtoken;

// variable is a RHS of a rule in the grammer
typedef struct variable
{
    // no_of_tokens is the number of tokens in the rule
    int token_number;
    // rule_no is the index of the rule in the variable
    int prod_num;
    // gtokens are the tokens of the rule
    gtoken *list_gtoken[MAX_TOKENS];
    // next is the next RHS with the same LHS in the grammer
    struct variable *next;
} variable;

// grammer is the grammer of the language
typedef struct grammar
{
    // terminals are the terminals in the grammer
    int grammar_terminals[MAX_TOKENS];
    // start_variable is the index of the start variable in the grammer
    int first_var[MAX_TOKENS];
    // variables are the RHSs of the rule with start_variable as LHS
    variable *variables[MAX_TOKENS];
    // no_of_variables is the number of non-terminals in the grammer
    int variable_number;
    // no_of_terminals is the number of terminals in the grammer
    int terminal_number;
    // no_of_rules is the number of rules in the grammer
    int no_of_rules;
} grammar;

// FirstAndFollow is the first and follow set of the language
typedef struct FirstAndFollow
{
    // start_variable is the index of the start variable in the language
    int first_var[MAX_TOKENS];
    // terminals are the terminals in the language
    int terminals[MAX_TOKENS];
    // ff_element is the first and follow set of each non-terminal
    ff_element first_follow_set[MAX_TOKENS];
    // no_of_terminals is the number of terminals in the language
    int terminal_number;
    // no_of_variables is the number of non-terminals in the language
    int variable_number;
} FirstAndFollow;



// parseTree is the parse tree of the language
typedef struct parseTree
{
    // t is the token of the node
    token t;
    // no_of_children is the number of children of the node
    int child_cnt;
    //child_list is the list of children of the node
    struct parseTree *child_list[MAX_TOKENS];
    // parent is the index of the parent in the parse tree
    int parent;
} parseTree;

#endif
