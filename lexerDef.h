/*
Group Number - 17
ID:	2022A7PS0173P			Name: Ameesh Sethi
ID:	2022A7PS0097P			Name: Aviral Gupta
ID:	2022A7PS0146P			Name: Yatharth Singh
ID:	2022A7PS1188P			Name: Armaan Sethi
ID:	2022A7PS0052P			Name: Utkarsh Tiwari
*/

#ifndef LEXERDEF_H
#define LEXERDEF_H

#include <stdio.h>

#define MAX_BUFFER_SIZE 50
#define MAX_STATES 64
#define MAX_TOKENS 100

static const char ALPHABETS[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ' ', '\n', '\t', '~', '(', '[', ']', ')', '<', '>', '!', '@', '#', '%', '&', '*', '_', '+', '-', '/', '=', ';', ':', '.', ','};
static const int ALPHABET_SIZE = sizeof(ALPHABETS) / sizeof(ALPHABETS[0]);
static const char *const TOKENS[] = {"program", "mainFunction", "otherFunctions", "function", "input_par", "output_par", "parameter_list", "dataType", "primitiveDatatype", "constructedDatatype", "remaining_list", "stmts", "typeDefinitions", "typeDefinition", "actualOrRedefined", "fieldDefinitions", "fieldDefinition", "fieldType", "moreFields", "declarations", "declaration", "global_or_not", "otherStmts", "stmt", "assignmentStmt", "singleOrRecId", "option_single_constructed", "oneExpansion", "moreExpansions", "funCallStmt", "outputParameters", "inputParameters", "iterativeStmt", "conditionalStmt", "elsePart", "ioStmt", "arithmeticExpression", "expPrime", "termPrime", "term", "factor", "lowPrecedenceOperators", "highPrecedenceOperators", "booleanExpression", "var", "logicalOp", "relationalOp", "returnStmt", "optionalReturn", "idList", "more_ids", "definetypestmt", "A", "TK_NOTOKEN", "TK_INVALID", "TK_ASSIGNOP", "TK_COMMENT", "TK_FIELDID", "TK_ID", "TK_NUM", "TK_RNUM", "TK_FUNID", "TK_RUID", "TK_WITH", "TK_PARAMETERS", "TK_END", "TK_WHILE", "TK_UNION", "TK_ENDUNION", "TK_DEFINETYPE", "TK_AS", "TK_TYPE", "TK_MAIN", "TK_GLOBAL", "TK_PARAMETER", "TK_LIST", "TK_SQL", "TK_SQR", "TK_INPUT", "TK_OUTPUT", "TK_INT", "TK_REAL", "TK_COMMA", "TK_SEM", "TK_COLON", "TK_DOT", "TK_ENDWHILE", "TK_OP", "TK_CL", "TK_IF", "TK_THEN", "TK_ENDIF", "TK_READ", "TK_WRITE", "TK_RETURN", "TK_PLUS", "TK_MINUS", "TK_MUL", "TK_DIV", "TK_CALL", "TK_RECORD", "TK_ENDRECORD", "TK_ELSE", "TK_AND", "TK_OR", "TK_NOT", "TK_LT", "TK_LE", "TK_EQ", "TK_GT", "TK_GE", "TK_NE", "TK_DOLLAR", "TK_EPSILON", 0};

typedef enum state_id
{
    STATE_1,
    STATE_2,
    STATE_3,
    STATE_4,
    STATE_5,
    STATE_6,
    STATE_7,
    STATE_8,
    STATE_9,
    STATE_10,

    STATE_11,
    STATE_12,
    STATE_13,
    STATE_14,
    STATE_15,
    STATE_16,
    STATE_17,
    STATE_18,
    STATE_19,
    STATE_20,

    STATE_21,
    STATE_22,
    STATE_23,
    STATE_24,
    STATE_25,
    STATE_26,
    STATE_27,
    STATE_28,
    STATE_29,
    STATE_30,

    STATE_31,
    STATE_32,
    STATE_33,
    STATE_34,
    STATE_35,
    STATE_36,
    STATE_37,
    STATE_38,
    STATE_39,
    STATE_40,

    STATE_41,
    STATE_42,
    STATE_43,
    STATE_44,
    STATE_45,
    STATE_46,
    STATE_47,
    STATE_48,
    STATE_49,
    STATE_50,

    STATE_51,
    STATE_52,
    STATE_53,
    STATE_54,
    STATE_55,
    STATE_56,
    STATE_57,
    STATE_58,
    STATE_59,
    STATE_60,

    STATE_61,
    STATE_62,
    START,
    INVALID,
} state_id; // Enum for state ids

typedef enum token_id
{
    program,
    mainFunction,
    otherFunctions,
    function,
    input_par,
    output_par,
    parameter_list,
    dataType,
    primitiveDatatype,
    constructedDatatype,
    remaining_list,
    stmts,
    typeDefinitions,
    typeDefinition,
    actualOrRedefined,
    fieldDefinitions,
    fieldDefinition,
    fieldType,
    moreFields,
    declarations,
    declaration,
    global_or_not,
    otherStmts,
    stmt,
    assignmentStmt,
    singleOrRecId,
    option_single_constructed,
    oneExpansion,
    moreExpansions,
    funCallStmt,
    outputParameters,
    inputParameters,
    iterativeStmt,
    conditionalStmt,
    elsePart,
    ioStmt,
    arithmeticExpression,
    expPrime,
    termPrime,
    term,
    factor,
    lowPrecedenceOperators,
    highPrecedenceOperators,
    booleanExpression,
    var,
    logicalOp,
    relationalOp,
    returnStmt,
    optionalReturn,
    idList,
    more_ids,
    definetypestmt,
    A,
    TK_NOTOKEN,
    TK_INVALID,
    TK_ASSIGNOP,
    TK_COMMENT,
    TK_FIELDID,
    TK_ID,
    TK_NUM,
    TK_RNUM,
    TK_FUNID,
    TK_RUID,
    TK_WITH,
    TK_PARAMETERS,
    TK_END,
    TK_WHILE,
    TK_UNION,
    TK_ENDUNION,
    TK_DEFINETYPE,
    TK_AS,
    TK_TYPE,
    TK_MAIN,
    TK_GLOBAL,
    TK_PARAMETER,
    TK_LIST,
    TK_SQL,
    TK_SQR,
    TK_INPUT,
    TK_OUTPUT,
    TK_INT,
    TK_REAL,
    TK_COMMA,
    TK_SEM,
    TK_COLON,
    TK_DOT,
    TK_ENDWHILE,
    TK_OP,
    TK_CL,
    TK_IF,
    TK_THEN,
    TK_ENDIF,
    TK_READ,
    TK_WRITE,
    TK_RETURN,
    TK_PLUS,
    TK_MINUS,
    TK_MUL,
    TK_DIV,
    TK_CALL,
    TK_RECORD,
    TK_ENDRECORD,
    TK_ELSE,
    TK_AND,
    TK_OR,
    TK_NOT,
    TK_LT,
    TK_LE,
    TK_EQ,
    TK_GT,
    TK_GE,
    TK_NE,
    TK_DOLLAR,
    TK_EPSILON
} token_id; // Enum for token ids

// Core token and state management structures
typedef struct Token
{
    token_id tk;
    int lc;
    char *lexeme;
} Token;

typedef struct State
{
    struct Transition **transitions;
    int length;
    token_id token;
    int retract_count;
    int state_id;
} State;

typedef struct Transition
{
    char next_char;
    struct State *next_state;
} Transition;

// Buffer management for lexical analysis
typedef struct DualBuffer
{
    char primary_buffer[MAX_BUFFER_SIZE];   // Main buffer for current processing
    char secondary_buffer[MAX_BUFFER_SIZE]; // Overflow buffer for token spanning reads
    int primary_buffer_index;               // Current position in primary buffer
    int secondary_buffer_index;             // Current position in secondary buffer
    int line_count;                        // Current line number being processed
} DualBuffer;

// Token collection and management
typedef struct TokenInfo
{
    struct Token **tokens;    // Array of tokens found
    int token_count;         // Number of tokens processed
} TokenInfo;

// Type definitions for pointer management
typedef struct State *state;           
typedef struct Transition *transition; 
typedef struct DualBuffer *dualBuffer;  
typedef struct TokenInfo *tokenInfo;   
typedef struct Token *token;           

// Array of states for the graph


// #ifndef TRIE_INCOMPLETE
// #include "trie.h"

// #endif

#include "lexer.h"
#endif