/*
Group Number - 17
ID:	2022A7PS0173P			Name: Ameesh Sethi
ID:	2022A7PS0097P			Name: Aviral Gupta
ID:	2022A7PS0146P			Name: Yatharth Singh
ID:	2022A7PS1188P			Name: Armaan Sethi
ID:	2022A7PS0052P			Name: Utkarsh Tiwari
*/

#include "lexerDef.h"
#include "parserDef.h"
#define INT_MAX 21435
#define INT_MIN -21435

// Computes FIRST and FOLLOW sets for all non-terminals in the grammar
FirstAndFollow ComputeFirstAndFollowSets(grammar inputGrammar)
{
    FirstAndFollow firstFollowSets;
    firstFollowSets.variable_number = inputGrammar.variable_number;
    firstFollowSets.terminal_number = inputGrammar.terminal_number;

    // Initialize first and follow sets for each non-terminal
    for (int nonTermIdx = 0; nonTermIdx < inputGrammar.variable_number; nonTermIdx++)
    {
        firstFollowSets.first_follow_set[nonTermIdx].first_set_size = 0;
        firstFollowSets.first_follow_set[nonTermIdx].follow_set_size = 0;
        firstFollowSets.first_var[nonTermIdx] = inputGrammar.first_var[nonTermIdx];
    }

    // Copy terminal symbols
    for (int termIdx = 0; termIdx < inputGrammar.terminal_number; termIdx++)
    {
        firstFollowSets.terminals[termIdx] = inputGrammar.grammar_terminals[termIdx];
    }

    // Compute First Sets
    bool isFirstSetComputed[inputGrammar.variable_number];
    for (int nonTermIdx = 0; nonTermIdx < inputGrammar.variable_number; nonTermIdx++)
    {
        isFirstSetComputed[nonTermIdx] = false;
    }

    // Process each non-terminal
    for (int currentNonTerm = 0; currentNonTerm < inputGrammar.variable_number;)
    {
        if (!isFirstSetComputed[currentNonTerm])
        {
            // Queue for dependency resolution
            int dependencyQueue[inputGrammar.variable_number];
            int queueFront = 0;
            int queueRear = 0;
            dependencyQueue[queueRear++] = currentNonTerm;

            // Process dependencies
            while (queueFront < queueRear)
            {
                int processedNonTerm = dependencyQueue[queueFront++];
                if (isFirstSetComputed[processedNonTerm])
                {
                    continue;
                }

                variable *currentRule = inputGrammar.variables[processedNonTerm];
                bool canComputeFirst = true;

                // Check each production rule
                while (currentRule != NULL)
                {
                    if (currentRule->token_number > 0)
                    {
                        int symbolPos = 0;
                        bool hasEpsilon = true;

                        // Process symbols in the rule until a non-nullable symbol is found
                        while (symbolPos < currentRule->token_number && hasEpsilon)
                        {
                            if (currentRule->list_gtoken[symbolPos]->terminal)
                            {
                                break;
                            }
                            else
                            {
                                int dependentNonTerm = currentRule->list_gtoken[symbolPos]->tk_name;
                                // Add uncomputed dependencies to queue
                                if (!isFirstSetComputed[dependentNonTerm] && processedNonTerm != dependentNonTerm)
                                {
                                    dependencyQueue[queueRear++] = dependentNonTerm;
                                    canComputeFirst = false;
                                }
                                else
                                {
                                    hasEpsilon = false;
                                    // Check if dependent non-terminal can derive epsilon
                                    for (int firstIdx = 0; firstIdx < firstFollowSets.first_follow_set[dependentNonTerm].first_set_size; firstIdx++)
                                    {
                                        if (firstFollowSets.first_follow_set[dependentNonTerm].first[firstIdx] == TK_EPSILON)
                                        {
                                            hasEpsilon = true;
                                            break;
                                        }
                                    }
                                }
                            }
                            symbolPos++;
                        }
                    }
                    currentRule = currentRule->next;
                }

                // Compute first set if all dependencies are resolved
                if (canComputeFirst)
                {
                    currentRule = inputGrammar.variables[processedNonTerm];
                    
                    // Process each production rule
                    while (currentRule != NULL)
                    {
                        if (currentRule->token_number > 0)
                        {
                            int symbolPos = 0;
                            bool hasEpsilon = true;

                            // Process symbols in the rule until a non-nullable symbol is found
                            while (symbolPos < currentRule->token_number && hasEpsilon)
                            {
                                if (currentRule->list_gtoken[symbolPos]->terminal)
                                {
                                    // Add terminal to first set if not already present
                                    bool isSymbolPresent = false;
                                    for (int firstIdx = 0; firstIdx < firstFollowSets.first_follow_set[processedNonTerm].first_set_size; firstIdx++)
                                    {
                                        if (firstFollowSets.first_follow_set[processedNonTerm].first[firstIdx] == currentRule->list_gtoken[symbolPos]->tk_name)
                                        {
                                            isSymbolPresent = true;
                                            break;
                                        }
                                    }
                                    if (!isSymbolPresent)
                                    {
                                        firstFollowSets.first_follow_set[processedNonTerm].first[firstFollowSets.first_follow_set[processedNonTerm].first_set_size] = currentRule->list_gtoken[symbolPos]->tk_name;
                                        firstFollowSets.first_follow_set[processedNonTerm].first_set_rule[firstFollowSets.first_follow_set[processedNonTerm].first_set_size] = currentRule->prod_num;
                                        firstFollowSets.first_follow_set[processedNonTerm].first_set_size++;
                                    }
                                    hasEpsilon = false;
                                }
                                else
                                {
                                    int dependentNonTerm = currentRule->list_gtoken[symbolPos]->tk_name;
                                    hasEpsilon = false;

                                    // Add first set of dependent non-terminal
                                    for (int firstIdx = 0; firstIdx < firstFollowSets.first_follow_set[dependentNonTerm].first_set_size; firstIdx++)
                                    {
                                        if (firstFollowSets.first_follow_set[dependentNonTerm].first[firstIdx] == TK_EPSILON)
                                        {
                                            hasEpsilon = true;
                                            continue;
                                        }
                                        bool isSymbolPresent = false;
                                        for (int existingIdx = 0; existingIdx < firstFollowSets.first_follow_set[processedNonTerm].first_set_size; existingIdx++)
                                        {
                                            if (firstFollowSets.first_follow_set[processedNonTerm].first[existingIdx] == firstFollowSets.first_follow_set[dependentNonTerm].first[firstIdx])
                                            {
                                                isSymbolPresent = true;
                                                break;
                                            }
                                        }
                                        if (!isSymbolPresent)
                                        {
                                            firstFollowSets.first_follow_set[processedNonTerm].first[firstFollowSets.first_follow_set[processedNonTerm].first_set_size] = firstFollowSets.first_follow_set[dependentNonTerm].first[firstIdx];
                                            firstFollowSets.first_follow_set[processedNonTerm].first_set_rule[firstFollowSets.first_follow_set[processedNonTerm].first_set_size] = currentRule->prod_num;
                                            firstFollowSets.first_follow_set[processedNonTerm].first_set_size++;
                                        }
                                    }
                                }
                                symbolPos++;
                            }

                            // Add epsilon if all symbols in the rule can derive epsilon
                            if (hasEpsilon)
                            {
                                bool isEpsilonPresent = false;
                                for (int firstIdx = 0; firstIdx < firstFollowSets.first_follow_set[processedNonTerm].first_set_size; firstIdx++)
                                {
                                    if (firstFollowSets.first_follow_set[processedNonTerm].first[firstIdx] == TK_EPSILON)
                                    {
                                        isEpsilonPresent = true;
                                        break;
                                    }
                                }
                                if (!isEpsilonPresent)
                                {
                                    firstFollowSets.first_follow_set[processedNonTerm].first[firstFollowSets.first_follow_set[processedNonTerm].first_set_size] = TK_EPSILON;
                                    firstFollowSets.first_follow_set[processedNonTerm].first_set_rule[firstFollowSets.first_follow_set[processedNonTerm].first_set_size] = currentRule->prod_num;
                                    firstFollowSets.first_follow_set[processedNonTerm].first_set_size++;
                                }
                            }
                        }
                        currentRule = currentRule->next;
                    }
                    isFirstSetComputed[processedNonTerm] = true;
                }
            }
        }
        else
        {
            currentNonTerm++;
        }
    }

    // Initialize follow set of start symbol with end marker
    firstFollowSets.first_follow_set[0].follow[firstFollowSets.first_follow_set[0].follow_set_size] = TK_DOLLAR;
    firstFollowSets.first_follow_set[0].follow_set_rule[firstFollowSets.first_follow_set[0].follow_set_size] = 0;
    firstFollowSets.first_follow_set[0].follow_set_size++;

    // Compute follow sets
    // Assumption 0 is the start variable and therefore adding $ in its follow set represented by TK_DOLLAR
    for (int i = 0; i < inputGrammar.variable_number; i++)
    {
        // iterate through all the starting variables
        variable *v = inputGrammar.variables[i];
        while (v != NULL)
        {
            // iterate through all the rules of the variable
            if (v->token_number > 0)
            {
                for (int k = 0; k < v->token_number; k++)
                {
                    // iterate through all the gtokens of the rule
                    if (v->list_gtoken[k]->terminal == 0)
                    {
                        // if the token is a variable
                        int c = -2;
                        for (int i = 0; i < firstFollowSets.terminal_number; i++)
                        {
                            if (firstFollowSets.first_var[i] == v->list_gtoken[k]->tk_name)
                            {
                                c = i;
                                break;
                            }
                        }
                        if (k < v->token_number - 1)
                        {
                            // if the token is not the last token of the rule
                            if (v->list_gtoken[k + 1]->terminal == 1)
                            {
                                // if the next token is a terminal
                                // check if the terminal is already present in the follow set of the current token
                                bool is_present = false;
                                for (int l = 0; l < firstFollowSets.first_follow_set[c].follow_set_size; l++)
                                {
                                    if (firstFollowSets.first_follow_set[c].follow[l] == v->list_gtoken[k + 1]->tk_name)
                                    {
                                        is_present = true;
                                        break;
                                    }
                                }
                                // if the terminal is not present in the follow set of the current token then add it to the follow set
                                if (is_present == false)
                                {
                                    firstFollowSets.first_follow_set[c].follow[firstFollowSets.first_follow_set[c].follow_set_size] = v->list_gtoken[k + 1]->tk_name;
                                    firstFollowSets.first_follow_set[c].follow_set_rule[firstFollowSets.first_follow_set[c].follow_set_size] = v->prod_num;
                                    firstFollowSets.first_follow_set[c].follow_set_size++;
                                }
                            }
                            else
                            {
                                // if the next token is a variable
                                int l = k + 1;
                                bool is_TK_EPSILON = true;
                                while (l < v->token_number && is_TK_EPSILON == true)
                                {
                                    // iterate through all the gtokens after the token k
                                    if (v->list_gtoken[l]->terminal == 1)
                                    {
                                        // if the token is a terminal
                                        // check if the terminal is already present in the first set of the current token
                                        bool is_present = false;
                                        is_TK_EPSILON = false;
                                        for (int r = 0; r < firstFollowSets.first_follow_set[c].first_set_size; r++)
                                        {
                                            if (firstFollowSets.first_follow_set[c].first[r] == v->list_gtoken[l]->tk_name)
                                            {
                                                is_present = true;
                                                break;
                                            }
                                        }
                                        // if the terminal is not present in the first set of the next token then add it to the follow set of the current token
                                        if (is_present == false)
                                        {
                                            firstFollowSets.first_follow_set[c].follow[firstFollowSets.first_follow_set[c].follow_set_size] = v->list_gtoken[l]->tk_name;
                                            firstFollowSets.first_follow_set[c].follow_set_rule[firstFollowSets.first_follow_set[c].follow_set_size] = v->prod_num;
                                            firstFollowSets.first_follow_set[c].follow_set_size++;
                                        }
                                        is_TK_EPSILON = false;
                                    }
                                    else
                                    {
                                        // if the token is a variable
                                        int lindex = -2;
                                        for (int i = 0; i < firstFollowSets.terminal_number; i++)
                                        {
                                            if (firstFollowSets.first_var[i] == v->list_gtoken[l]->tk_name)
                                            {
                                                lindex = i;
                                                break;
                                            }
                                        }
                                        is_TK_EPSILON = false;
                                        for (int r = 0; r < firstFollowSets.first_follow_set[lindex].first_set_size; r++)
                                        {
                                            if (firstFollowSets.first_follow_set[lindex].first[r] == TK_EPSILON)
                                            {
                                                is_TK_EPSILON = true;
                                                continue;
                                            }
                                            bool is_present = false;
                                            for (int m = 0; m < firstFollowSets.first_follow_set[c].follow_set_size; m++)
                                            {
                                                if (firstFollowSets.first_follow_set[c].follow[m] == firstFollowSets.first_follow_set[lindex].first[r])
                                                {
                                                    is_present = true;
                                                    break;
                                                }
                                            }
                                            if (is_present == false)
                                            {
                                                firstFollowSets.first_follow_set[c].follow[firstFollowSets.first_follow_set[c].follow_set_size] = firstFollowSets.first_follow_set[lindex].first[r];
                                                firstFollowSets.first_follow_set[c].follow_set_rule[firstFollowSets.first_follow_set[c].follow_set_size] = v->prod_num;
                                                firstFollowSets.first_follow_set[c].follow_set_size++;
                                            }
                                        }
                                    }
                                    l++;
                                }
                                // if is_TK_EPSILON is true then add the lhs variable to the follow set of the current token which will be computed later
                                if (is_TK_EPSILON == true)
                                {
                                    bool is_present = false;
                                    for (int r = 0; r < firstFollowSets.first_follow_set[c].follow_set_size; r++)
                                    {
                                        if (firstFollowSets.first_follow_set[c].follow[r] == inputGrammar.first_var[i])
                                        {
                                            is_present = true;
                                            break;
                                        }
                                    }
                                    if (is_present == false)
                                    {
                                        firstFollowSets.first_follow_set[c].follow[firstFollowSets.first_follow_set[c].follow_set_size] = inputGrammar.first_var[i];
                                        firstFollowSets.first_follow_set[c].follow_set_rule[firstFollowSets.first_follow_set[c].follow_set_size] = v->prod_num;
                                        firstFollowSets.first_follow_set[c].follow_set_size++;
                                    }
                                }
                            }
                        }
                        // if the token is the last token of the rule
                        else
                        {
                            bool is_present = false;
                            for (int l = 0; l < firstFollowSets.first_follow_set[c].follow_set_size; l++)
                            {
                                if (firstFollowSets.first_follow_set[c].follow[l] == inputGrammar.first_var[i])
                                {
                                    is_present = true;
                                    break;
                                }
                            }
                            if (is_present == false)
                            {
                                firstFollowSets.first_follow_set[c].follow[firstFollowSets.first_follow_set[c].follow_set_size] = inputGrammar.first_var[i];
                                firstFollowSets.first_follow_set[c].follow_set_rule[firstFollowSets.first_follow_set[c].follow_set_size] = v->prod_num;
                                firstFollowSets.first_follow_set[c].follow_set_size++;
                            }
                        }
                    }
                }
            }
            v = v->next;
        }
    }

    // complete follow sets by adding the follow sets of non-terminal to the follow sets of the variables
    //  make a list of non-terminals
    for (int i = 0; i < inputGrammar.terminal_number; i++)
    {
        isFirstSetComputed[i] = false;
    }

    // iterate through all follow sets
    for (int i = 0; i < firstFollowSets.variable_number; i++)
    {
        if (isFirstSetComputed[firstFollowSets.first_var[i]] == false)
        {
            // if the follow set is not computed iterate through the follow set and check if there is a non-terminal
            for (int j = 0; j < firstFollowSets.first_follow_set[i].follow_set_size; j++)
            {
                bool check = false;
                for (int k = 0; k < firstFollowSets.variable_number; k++)
                {
                    if (firstFollowSets.first_follow_set[i].follow[j] == firstFollowSets.first_var[k])
                    {
                        check = true;
                        break;
                    }
                }
                if (firstFollowSets.first_follow_set[i].follow[j] != TK_DOLLAR && check == true)
                {
                    // if there is a non-terminal then add the follow set of the non-terminal to the follow set of the current token
                    for (int k = 0; k < firstFollowSets.first_follow_set[firstFollowSets.first_follow_set[i].follow[j]].follow_set_size; k++)
                    {
                        bool is_present = false;
                        for (int l = 0; l < firstFollowSets.first_follow_set[i].follow_set_size; l++)
                        {
                            if (firstFollowSets.first_follow_set[i].follow[l] == firstFollowSets.first_follow_set[firstFollowSets.first_follow_set[i].follow[j]].follow[k])
                            {
                                is_present = true;
                                break;
                            }
                        }
                        if (is_present == false)
                        {
                            firstFollowSets.first_follow_set[i].follow[firstFollowSets.first_follow_set[i].follow_set_size] = firstFollowSets.first_follow_set[firstFollowSets.first_follow_set[i].follow[j]].follow[k];
                            firstFollowSets.first_follow_set[i].follow_set_rule[firstFollowSets.first_follow_set[i].follow_set_size] = firstFollowSets.first_follow_set[firstFollowSets.first_follow_set[i].follow[j]].follow_set_rule[k];
                            firstFollowSets.first_follow_set[i].follow_set_size++;
                        }
                    }
                }
            }
            isFirstSetComputed[firstFollowSets.first_var[i]] = true;
        }
    }
    // iterate through all the follow sets
    for (int i = 0; i < firstFollowSets.variable_number; i++)
    {
        // iterate through the follow set to check if a non_terminal is present
        for (int j = 0; j < firstFollowSets.first_follow_set[i].follow_set_size; j++)
        {
            if (firstFollowSets.first_follow_set[i].follow[j] == TK_DOLLAR)
                continue;

            bool check = false;
            for (int k = 0; k < firstFollowSets.variable_number; k++)
            {
                if (firstFollowSets.first_follow_set[i].follow[j] == firstFollowSets.first_var[k])
                {
                    check = true;
                    break;
                }
            }
            if (check == true)
            {
                // if a non-terminal is present , remove it from the follow set
                for (int k = j; k < firstFollowSets.first_follow_set[i].follow_set_size - 1; k++)
                {
                    firstFollowSets.first_follow_set[i].follow[k] = firstFollowSets.first_follow_set[i].follow[k + 1];
                    firstFollowSets.first_follow_set[i].follow_set_rule[k] = firstFollowSets.first_follow_set[i].follow_set_rule[k + 1];
                }
                firstFollowSets.first_follow_set[i].follow_set_size--;
                j--;
            }
        }
    }
    return firstFollowSets;
}

// Creates LL(1) parsing table from FIRST and FOLLOW sets
// Returns true if grammar is LL(1), false otherwise
bool createParseTable(FirstAndFollow firstFollowSets, table *parseTable)
{
    // Initialize parse table dimensions based on grammar size
    parseTable->row_cnt = firstFollowSets.variable_number;
    parseTable->col_cnt = firstFollowSets.terminal_number;

    // Initialize row labels (non-terminals)
    for (int rowIdx = 0; rowIdx < firstFollowSets.variable_number; rowIdx++)
    {
        parseTable->rows[rowIdx] = firstFollowSets.first_var[rowIdx];
    }

    // Initialize column labels (terminals)
    for (int colIdx = 0; colIdx < firstFollowSets.terminal_number; colIdx++)
    {
        parseTable->cols[colIdx] = firstFollowSets.terminals[colIdx];
    }

    // Initialize table cells with error state
    for (int rowIdx = 0; rowIdx < firstFollowSets.variable_number; rowIdx++)
    {
        for (int colIdx = 0; colIdx < firstFollowSets.terminal_number; colIdx++)
        {
            parseTable->table[rowIdx][colIdx] = INT_MIN;
        }
    }

    // Initialize sync entries in parse table
    for (int nonTermIdx = 0; nonTermIdx < firstFollowSets.variable_number; nonTermIdx++)
    {
        for (int followIdx = 0; followIdx < firstFollowSets.first_follow_set[nonTermIdx].follow_set_size; followIdx++)
        {
            int terminalIdx = -1;
            for (int idx = 0; idx < firstFollowSets.terminal_number; idx++)
            {
                if (firstFollowSets.first_follow_set[nonTermIdx].follow[followIdx] == parseTable->cols[idx])
                {
                    terminalIdx = idx;
                    break;
                }
            }
            parseTable->table[nonTermIdx][terminalIdx] = INT_MAX;
        }
    }

    bool isLL1Grammar = true;

    // Fill parse table using first and follow sets
    for (int nonTermIdx = 0; nonTermIdx < firstFollowSets.variable_number; nonTermIdx++)
    {
        // Process each symbol in first set
        for (int firstIdx = 0; firstIdx < firstFollowSets.first_follow_set[nonTermIdx].first_set_size; firstIdx++)
        {
            if (firstFollowSets.first_follow_set[nonTermIdx].first[firstIdx] == TK_EPSILON)
            {
                // Handle epsilon productions - add follow set entries
                for (int followIdx = 0; followIdx < firstFollowSets.first_follow_set[nonTermIdx].follow_set_size; followIdx++)
                {
                    int terminalIdx = -1;
                    for (int idx = 0; idx < firstFollowSets.terminal_number; idx++)
                    {
                        if (firstFollowSets.first_follow_set[nonTermIdx].follow[followIdx] == parseTable->cols[idx])
                        {
                            terminalIdx = idx;
                            break;
                        }
                    }
                    // Check for LL(1) conflict
                    if (parseTable->table[nonTermIdx][terminalIdx] != INT_MIN)
                    {
                        isLL1Grammar = false;
                    }
                    parseTable->table[nonTermIdx][terminalIdx] = firstFollowSets.first_follow_set[nonTermIdx].first_set_rule[firstIdx];
                }
            }
            else
            {
                // Handle non-epsilon productions
                int terminalIdx = -1;
                for (int idx = 0; idx < firstFollowSets.terminal_number; idx++)
                {
                    if (firstFollowSets.first_follow_set[nonTermIdx].first[firstIdx] == parseTable->cols[idx])
                    {
                        terminalIdx = idx;
                        break;
                    }
                }
                // Check for LL(1) conflict
                if (parseTable->table[nonTermIdx][terminalIdx] != INT_MIN)
                {
                    isLL1Grammar = false;
                }
                parseTable->table[nonTermIdx][terminalIdx] = firstFollowSets.first_follow_set[nonTermIdx].first_set_rule[firstIdx];
            }
        }
    }
    return isLL1Grammar;
}

// Parses input token stream using parse table and returns parse tree
// Returns NULL if parsing fails
parseTree *parseInputSourceCode(table parseTable, FirstAndFollow firstFollowSets, grammar *grammarPtr, vector tokenStream)
{
    // Filter out comments from the token stream
    vector filteredTokens = init_vector();
    for (int tokenIdx = 0; tokenIdx < tokenStream->size; tokenIdx++) {
        if (get(tokenStream, tokenIdx)->tk != TK_COMMENT) {
            push_back(filteredTokens, get(tokenStream, tokenIdx));
        }
    }
    tokenStream = filteredTokens;
    
    // Initialize parsing stacks
    int inputLength = tokenStream->size;
    int symbolStack[100];          // Stack for grammar symbols
    parseTree *treeNodeStack[100]; // Stack for parse tree nodes
    int symbolTop = -1;           // Top of symbol stack
    int treeTop = -1;            // Top of parse tree stack

    // Initialize stacks with start symbol and EOF marker
    symbolStack[++symbolTop] = TK_DOLLAR;
    symbolStack[++symbolTop] = grammarPtr->first_var[0];

    // Create root node of parse tree
    treeNodeStack[++treeTop] = (parseTree *)malloc(sizeof(parseTree));
    treeNodeStack[treeTop]->t = (token)malloc(sizeof(Token));
    treeNodeStack[treeTop]->t->tk = symbolStack[symbolTop];
    treeNodeStack[treeTop]->t->lexeme = NULL;
    treeNodeStack[treeTop]->t->lc = -1;
    treeNodeStack[treeTop]->parent = -1;
    treeNodeStack[treeTop]->child_cnt = 0;
    parseTree *rootNode = treeNodeStack[treeTop];

    // Initialize parsing state variables
    int currentPos = 0;           // Current position in input
    int errorControl = 1;         // Error reporting control
    int hasError = 0;            // Error flag

    // Main parsing loop
    while (currentPos < inputLength && symbolTop > 0)
    {
        // Find column index for current token
        int terminalIdx = -2;
        for (int idx = 0; idx < firstFollowSets.terminal_number; idx++)
        {
            if (get(tokenStream, currentPos)->tk == parseTable.cols[idx])
            {
                terminalIdx = idx;
                break;
            }
        }

        // Find row index for current stack top
        int nonTerminalIdx = -2;
        for (int idx = 0; idx < firstFollowSets.variable_number; idx++)
        {
            if (symbolStack[symbolTop] == parseTable.rows[idx])
            {
                nonTerminalIdx = idx;
                break;
            }
        }

        bool shouldApplyRule = true;

        // Handle invalid tokens
        if (get(tokenStream, currentPos)->tk == TK_INVALID || terminalIdx == -2)
        {
            printf("Line %d Error: %s\n", get(tokenStream, currentPos)->lc, get(tokenStream, currentPos)->lexeme);
            currentPos++;
            errorControl = 0;
            hasError = 1;
            continue;
        }

        // Handle stack-input mismatch
        if (nonTerminalIdx == -2)
        {
            if (errorControl)
                printf("Line %d Error: The token %s for lexeme %s does not match with the expected token %s\n", 
                    get(tokenStream, currentPos)->lc, 
                    TOKENS[get(tokenStream, currentPos)->tk], 
                    get(tokenStream, currentPos)->lexeme, 
                    TOKENS[symbolStack[symbolTop]]);
            symbolTop--;
            treeTop--;
            errorControl = 0;
            hasError = 1;
            shouldApplyRule = false;
        }

        // Handle parse table error entry
        if (parseTable.table[nonTerminalIdx][terminalIdx] == INT_MIN)
        {
            if (errorControl)
                printf("Line %d Error: Invalid token %s encountered with %s stack top %s\n", 
                    get(tokenStream, currentPos)->lc, 
                    TOKENS[get(tokenStream, currentPos)->tk], 
                    get(tokenStream, currentPos)->lexeme, 
                    TOKENS[symbolStack[symbolTop]]);
            currentPos++;
            errorControl = 0;
            hasError = 1;
            continue;
        }

        // Handle synchronization entry
        if (parseTable.table[nonTerminalIdx][terminalIdx] == INT_MAX)
        {
            if (errorControl)
                printf("Line %d Error: Invalid token %s encountered with %s stack top %s\n", 
                    get(tokenStream, currentPos)->lc, 
                    TOKENS[get(tokenStream, currentPos)->tk], 
                    get(tokenStream, currentPos)->lexeme, 
                    TOKENS[symbolStack[symbolTop]]);
            symbolTop--;
            treeTop--;
            errorControl = 0;
            shouldApplyRule = false;
        }

        // Apply production rule
        if (shouldApplyRule)
        {
            variable *currentRule = grammarPtr->variables[nonTerminalIdx];
            while (currentRule != NULL)
            {
                if (currentRule->prod_num == parseTable.table[nonTerminalIdx][terminalIdx])
                {
                    // Pop current symbol and get its tree node
                    symbolTop--;
                    parseTree *parentNode = treeNodeStack[treeTop];
                    treeTop--;

                    // Push RHS symbols in reverse order
                    for (int symbolIdx = currentRule->token_number - 1; symbolIdx >= 0; symbolIdx--)
                    {
                        symbolTop++;
                        treeTop++;
                        symbolStack[symbolTop] = currentRule->list_gtoken[symbolIdx]->tk_name;
                        
                        // Create new tree node
                        treeNodeStack[treeTop] = (parseTree *)malloc(sizeof(parseTree));
                        treeNodeStack[treeTop]->t = (token)malloc(sizeof(Token));
                        treeNodeStack[treeTop]->t->tk = symbolStack[symbolTop];
                        treeNodeStack[treeTop]->t->lexeme = NULL;
                        treeNodeStack[treeTop]->t->lc = -1;
                        treeNodeStack[treeTop]->parent = parentNode->t->tk;
                        treeNodeStack[treeTop]->child_cnt = 0;
                        
                        // Link to parent
                        parentNode->child_list[parentNode->child_cnt] = treeNodeStack[treeTop];
                        parentNode->child_cnt++;
                    }
                    break;
                }
                currentRule = currentRule->next;
            }
        }

        // Match terminals and advance input
        while (symbolTop > 0 && currentPos < inputLength && 
               (symbolStack[symbolTop] == get(tokenStream, currentPos)->tk || 
                symbolStack[symbolTop] == TK_EPSILON))
        {
            treeNodeStack[treeTop]->t->lexeme = get(tokenStream, currentPos)->lexeme;
            treeNodeStack[treeTop]->t->lc = get(tokenStream, currentPos)->lc;
            if (symbolStack[symbolTop] != TK_EPSILON)
                currentPos++;
            symbolTop--;
            treeTop--;
            errorControl = 1;
        }
    }

    // Check for successful parse
    if (symbolStack[symbolTop] == TK_DOLLAR && hasError == 0)
    {
        printf("COMPILATION SUCCESSFUL \n");
    }
    else
    {
        printf("COMPILATION ERROR\n");
    }
    return rootNode;
}

// Prints parse tree in a formatted table
// Each node printed with: lexeme, line number, token, value, parent, leaf status, symbol
void printParseTree(parseTree *node, FILE *output_stream)
{
    // Base case: if node is null, return
    if (node == NULL)
    {
        return;
    }

    // Process first child if node is not a leaf
    if (node->child_cnt != 0 && node->child_list[0] != NULL)
    {
        printParseTree(node->child_list[0], output_stream);
    }

    // Print node information to output stream
    if (output_stream != NULL)
    {
        // Format: lexeme, line_number, token_id, value, parent_symbol, is_leaf, node_symbol
        
        // Print lexeme field
        if (node->t->lexeme != NULL)
        {
            fprintf(output_stream, "%-30s", node->t->lexeme);
        }
        else
        {
            fprintf(output_stream, "%-30s", "----");
        }

        // Print line number
        if (node->t->lc != -1)
        {
            fprintf(output_stream, "%-30d", node->t->lc);
        }
        else
        {
            fprintf(output_stream, "%-30s", "----");
        }

        // Print token identifier
        fprintf(output_stream, "%-30d", node->t->tk);

        // Print numeric value if token is a number
        if (node->t->tk == TK_NUM || node->t->tk == TK_RNUM)
        {
            fprintf(output_stream, "%-30s", node->t->lexeme);
        }
        else
        {
            fprintf(output_stream, "%-30s", "----");
        }

        // Print parent symbol
        if (node->parent != -1)
        {
            fprintf(output_stream, "%-30s", TOKENS[node->parent]);
        }
        else
        {
            fprintf(output_stream, "%-30s", "----");
        }

        // Print leaf status
        if (node->child_cnt == 0)
        {
            fprintf(output_stream, "%-30s", "YES");
        }
        else
        {
            fprintf(output_stream, "%-30s", "NO");
        }

        // Print node symbol
        fprintf(output_stream, "%s", TOKENS[node->t->tk]);
        fprintf(output_stream, "\n");
    }

    // Process remaining children
    for (int child_idx = 1; child_idx < node->child_cnt; child_idx++)
    {
        if(node->child_list[child_idx] != NULL)
        {
            printParseTree(node->child_list[child_idx], output_stream);
        }
    }
}

// Adds a new production rule to the grammar
// lhs_symbol: Left-hand side non-terminal
// rhs_length: Number of symbols on right-hand side
// rhs_symbols: Array of right-hand side symbols
void insert_production_rule(grammar *grammar_ptr, int lhs_symbol, int rhs_length, int *rhs_symbols)
{
    // finding the index of the lhs symbol
    int symbol_idx;
    for (int i = 0; i < grammar_ptr->variable_number; i++)
    {
        symbol_idx = i;
        if (grammar_ptr->first_var[i] == lhs_symbol)
        {
            break;
        }
    }
    
    // iterate through the rules of the variable to reach the end of the linked list
    variable *current_rule;
    if (grammar_ptr->variables[symbol_idx] == NULL)
    {
        grammar_ptr->variables[symbol_idx] = (variable *)malloc(sizeof(variable));
        current_rule = grammar_ptr->variables[symbol_idx];
    }
    else
    {
        current_rule = grammar_ptr->variables[symbol_idx];
        while (current_rule->next != NULL)
        {
            current_rule = current_rule->next;
        }
        current_rule->next = (variable *)malloc(sizeof(variable));
        current_rule = current_rule->next;
    }
    
    // add the rule to the grammar
    current_rule->token_number = rhs_length;
    current_rule->prod_num = grammar_ptr->no_of_rules + 1;
    
    for (int i = 0; i < rhs_length; i++)
    {
        current_rule->list_gtoken[i] = (gtoken *)malloc(sizeof(gtoken));
        current_rule->list_gtoken[i]->tk_name = rhs_symbols[i];
        current_rule->list_gtoken[i]->terminal = 0;
        
        for (int j = 0; j < grammar_ptr->terminal_number; j++)
        {
            if (grammar_ptr->grammar_terminals[j] == rhs_symbols[i])
            {
                current_rule->list_gtoken[i]->terminal = 1;
                break;
            }
        }
    }
    
    current_rule->next = NULL;
    grammar_ptr->no_of_rules++;
}

// Initializes grammar with all production rules
// Sets up non-terminals, terminals and their relationships
void init(grammar *G)
{
    // Initialize grammar size parameters
    G->variable_number = 53;  // Number of non-terminals
    G->terminal_number = 61;  // Number of terminals
    G->no_of_rules = 0;      // Start with no rules

    // Initialize array of non-terminals (variables)
    // Index corresponds to the non-terminal's unique identifier
    G->first_var[0] = program;
    G->first_var[1] = mainFunction;
    G->first_var[2] = otherFunctions;
    G->first_var[3] = function;
    G->first_var[4] = input_par;
    G->first_var[5] = output_par;
    G->first_var[6] = parameter_list;
    G->first_var[7] = dataType;
    G->first_var[8] = primitiveDatatype;
    G->first_var[9] = constructedDatatype;
    G->first_var[10] = remaining_list;
    G->first_var[11] = stmts;
    G->first_var[12] = typeDefinitions;
    G->first_var[13] = typeDefinition;
    G->first_var[14] = actualOrRedefined;
    G->first_var[15] = fieldDefinitions;
    G->first_var[16] = fieldDefinition;
    G->first_var[17] = fieldType;
    G->first_var[18] = moreFields;
    G->first_var[19] = declarations;
    G->first_var[20] = declaration;
    G->first_var[21] = global_or_not;
    G->first_var[22] = otherStmts;
    G->first_var[23] = stmt;
    G->first_var[24] = assignmentStmt;
    G->first_var[25] = singleOrRecId;
    G->first_var[26] = option_single_constructed;
    G->first_var[27] = oneExpansion;
    G->first_var[28] = moreExpansions;
    G->first_var[29] = funCallStmt;
    G->first_var[30] = outputParameters;
    G->first_var[31] = inputParameters;
    G->first_var[32] = iterativeStmt;
    G->first_var[33] = conditionalStmt;
    G->first_var[34] = elsePart;
    G->first_var[35] = ioStmt;
    G->first_var[36] = arithmeticExpression;
    G->first_var[37] = expPrime;
    G->first_var[38] = termPrime;
    G->first_var[39] = term;
    G->first_var[40] = factor;
    G->first_var[41] = lowPrecedenceOperators;
    G->first_var[42] = highPrecedenceOperators;
    G->first_var[43] = booleanExpression;
    G->first_var[44] = var;
    G->first_var[45] = logicalOp;
    G->first_var[46] = relationalOp;
    G->first_var[47] = returnStmt;
    G->first_var[48] = optionalReturn;
    G->first_var[49] = idList;
    G->first_var[50] = more_ids;
    G->first_var[51] = definetypestmt;
    G->first_var[52] = A;

    // Initialize array of terminals
    // Index corresponds to the terminal's token type
    G->grammar_terminals[0] = TK_NOTOKEN;
    G->grammar_terminals[1] = TK_INVALID;
    G->grammar_terminals[2] = TK_ASSIGNOP;
    G->grammar_terminals[3] = TK_COMMENT;
    G->grammar_terminals[4] = TK_FIELDID;
    G->grammar_terminals[5] = TK_ID;
    G->grammar_terminals[6] = TK_NUM;
    G->grammar_terminals[7] = TK_RNUM;
    G->grammar_terminals[8] = TK_FUNID;
    G->grammar_terminals[9] = TK_RUID;
    G->grammar_terminals[10] = TK_WITH;
    G->grammar_terminals[11] = TK_PARAMETERS;
    G->grammar_terminals[12] = TK_END;
    G->grammar_terminals[13] = TK_WHILE;
    G->grammar_terminals[14] = TK_UNION;
    G->grammar_terminals[15] = TK_ENDUNION;
    G->grammar_terminals[16] = TK_DEFINETYPE;
    G->grammar_terminals[17] = TK_AS;
    G->grammar_terminals[18] = TK_TYPE;
    G->grammar_terminals[19] = TK_MAIN;
    G->grammar_terminals[20] = TK_GLOBAL;
    G->grammar_terminals[21] = TK_PARAMETER;
    G->grammar_terminals[22] = TK_LIST;
    G->grammar_terminals[23] = TK_SQL;
    G->grammar_terminals[24] = TK_SQR;
    G->grammar_terminals[25] = TK_INPUT;
    G->grammar_terminals[26] = TK_OUTPUT;
    G->grammar_terminals[27] = TK_INT;
    G->grammar_terminals[28] = TK_REAL;
    G->grammar_terminals[29] = TK_COMMA;
    G->grammar_terminals[30] = TK_SEM;
    G->grammar_terminals[31] = TK_COLON;
    G->grammar_terminals[32] = TK_DOT;
    G->grammar_terminals[33] = TK_ENDWHILE;
    G->grammar_terminals[34] = TK_OP;
    G->grammar_terminals[35] = TK_CL;
    G->grammar_terminals[36] = TK_IF;
    G->grammar_terminals[37] = TK_THEN;
    G->grammar_terminals[38] = TK_ENDIF;
    G->grammar_terminals[39] = TK_READ;
    G->grammar_terminals[40] = TK_WRITE;
    G->grammar_terminals[41] = TK_RETURN;
    G->grammar_terminals[42] = TK_PLUS;
    G->grammar_terminals[43] = TK_MINUS;
    G->grammar_terminals[44] = TK_MUL;
    G->grammar_terminals[45] = TK_DIV;
    G->grammar_terminals[46] = TK_CALL;
    G->grammar_terminals[47] = TK_RECORD;
    G->grammar_terminals[48] = TK_ENDRECORD;
    G->grammar_terminals[49] = TK_ELSE;
    G->grammar_terminals[50] = TK_AND;
    G->grammar_terminals[51] = TK_OR;
    G->grammar_terminals[52] = TK_NOT;
    G->grammar_terminals[53] = TK_LT;
    G->grammar_terminals[54] = TK_LE;
    G->grammar_terminals[55] = TK_EQ;
    G->grammar_terminals[56] = TK_GT;
    G->grammar_terminals[57] = TK_GE;
    G->grammar_terminals[58] = TK_NE;
    G->grammar_terminals[59] = TK_DOLLAR;
    G->grammar_terminals[60] = TK_EPSILON;

    // Initialize all production rule lists to NULL
    for (int i = 0; i < G->variable_number; i++)
    {
        G->variables[i] = NULL;
    }

    // Add all production rules to the grammar
    // Format: LHS non-terminal, RHS length, array of RHS symbols
    insert_production_rule(G, program, 2, (int[]){otherFunctions, mainFunction});
    insert_production_rule(G, mainFunction, 3, (int[]){TK_MAIN, stmts, TK_END});
    insert_production_rule(G, otherFunctions, 2, (int[]){function, otherFunctions});
    insert_production_rule(G, otherFunctions, 1, (int[]){TK_EPSILON});
    insert_production_rule(G, function, 6, (int[]){TK_FUNID, input_par, output_par, TK_SEM, stmts, TK_END});
    insert_production_rule(G, input_par, 6, (int[]){TK_INPUT, TK_PARAMETER, TK_LIST, TK_SQL, parameter_list, TK_SQR});
    insert_production_rule(G, output_par, 6, (int[]){TK_OUTPUT, TK_PARAMETER, TK_LIST, TK_SQL, parameter_list, TK_SQR});
    insert_production_rule(G, output_par, 1, (int[]){TK_EPSILON});
    insert_production_rule(G, parameter_list, 3, (int[]){dataType, TK_ID, remaining_list});
    insert_production_rule(G, dataType, 1, (int[]){primitiveDatatype});
    insert_production_rule(G, dataType, 1, (int[]){constructedDatatype});
    insert_production_rule(G, primitiveDatatype, 1, (int[]){TK_INT});
    insert_production_rule(G, primitiveDatatype, 1, (int[]){TK_REAL});
    insert_production_rule(G, constructedDatatype, 2, (int[]){TK_RECORD, TK_RUID});
    insert_production_rule(G, constructedDatatype, 2, (int[]){TK_UNION, TK_RUID});
    insert_production_rule(G, constructedDatatype, 1, (int[]){TK_RUID});
    insert_production_rule(G, remaining_list, 2, (int[]){TK_COMMA, parameter_list});
    insert_production_rule(G, remaining_list, 1, (int[]){TK_EPSILON});
    insert_production_rule(G, stmts, 4, (int[]){typeDefinitions, declarations, otherStmts, returnStmt});
    insert_production_rule(G, typeDefinitions, 2, (int[]){actualOrRedefined, typeDefinitions});
    insert_production_rule(G, typeDefinitions, 1, (int[]){TK_EPSILON});
    insert_production_rule(G, actualOrRedefined, 1, (int[]){typeDefinition});
    insert_production_rule(G, actualOrRedefined, 1, (int[]){definetypestmt});
    insert_production_rule(G, typeDefinition, 4, (int[]){TK_RECORD, TK_RUID, fieldDefinitions, TK_ENDRECORD});
    insert_production_rule(G, typeDefinition, 4, (int[]){TK_UNION, TK_RUID, fieldDefinitions, TK_ENDUNION});
    insert_production_rule(G, fieldDefinitions, 3, (int[]){fieldDefinition, fieldDefinition, moreFields});
    insert_production_rule(G, fieldDefinition, 5, (int[]){TK_TYPE, fieldType, TK_COLON, TK_FIELDID, TK_SEM});
    insert_production_rule(G, fieldType, 1, (int[]){primitiveDatatype});
    insert_production_rule(G, fieldType, 1, (int[]){constructedDatatype});
    insert_production_rule(G, moreFields, 2, (int[]){fieldDefinition, moreFields});
    insert_production_rule(G, moreFields, 1, (int[]){TK_EPSILON});
    insert_production_rule(G, declarations, 2, (int[]){declaration, declarations});
    insert_production_rule(G, declarations, 1, (int[]){TK_EPSILON});
    insert_production_rule(G, declaration, 6, (int[]){TK_TYPE, dataType, TK_COLON, TK_ID, global_or_not, TK_SEM});
    insert_production_rule(G, global_or_not, 2, (int[]){TK_COLON, TK_GLOBAL});
    insert_production_rule(G, global_or_not, 1, (int[]){TK_EPSILON});
    insert_production_rule(G, otherStmts, 2, (int[]){stmt, otherStmts});
    insert_production_rule(G, otherStmts, 1, (int[]){TK_EPSILON});
    insert_production_rule(G, stmt, 1, (int[]){assignmentStmt});
    insert_production_rule(G, stmt, 1, (int[]){iterativeStmt});
    insert_production_rule(G, stmt, 1, (int[]){conditionalStmt});
    insert_production_rule(G, stmt, 1, (int[]){ioStmt});
    insert_production_rule(G, stmt, 1, (int[]){funCallStmt});
    insert_production_rule(G, assignmentStmt, 4, (int[]){singleOrRecId, TK_ASSIGNOP, arithmeticExpression, TK_SEM});
    insert_production_rule(G, singleOrRecId, 2, (int[]){TK_ID, option_single_constructed});
    insert_production_rule(G, option_single_constructed, 1, (int[]){TK_EPSILON});
    insert_production_rule(G, option_single_constructed, 2, (int[]){oneExpansion, moreExpansions});
    insert_production_rule(G, oneExpansion, 2, (int[]){TK_DOT, TK_FIELDID});
    insert_production_rule(G, moreExpansions, 2, (int[]){oneExpansion, moreExpansions});
    insert_production_rule(G, moreExpansions, 1, (int[]){TK_EPSILON});
    insert_production_rule(G, funCallStmt, 7, (int[]){outputParameters, TK_CALL, TK_FUNID, TK_WITH, TK_PARAMETERS, inputParameters, TK_SEM});
    insert_production_rule(G, outputParameters, 4, (int[]){TK_SQL, idList, TK_SQR, TK_ASSIGNOP});
    insert_production_rule(G, outputParameters, 1, (int[]){TK_EPSILON});
    insert_production_rule(G, inputParameters, 3, (int[]){TK_SQL, idList, TK_SQR});
    insert_production_rule(G, iterativeStmt, 7, (int[]){TK_WHILE, TK_OP, booleanExpression, TK_CL, stmt, otherStmts, TK_ENDWHILE});
    insert_production_rule(G, conditionalStmt, 8, (int[]){TK_IF, TK_OP, booleanExpression, TK_CL, TK_THEN, stmt, otherStmts, elsePart});
    insert_production_rule(G, elsePart, 4, (int[]){TK_ELSE, stmt, otherStmts, TK_ENDIF});
    insert_production_rule(G, elsePart, 1, (int[]){TK_ENDIF});
    insert_production_rule(G, ioStmt, 5, (int[]){TK_READ, TK_OP, var, TK_CL, TK_SEM});
    insert_production_rule(G, ioStmt, 5, (int[]){TK_WRITE, TK_OP, var, TK_CL, TK_SEM});
    insert_production_rule(G, arithmeticExpression, 2, (int[]){term, expPrime});
    insert_production_rule(G, expPrime, 3, (int[]){lowPrecedenceOperators, term, expPrime});
    insert_production_rule(G, expPrime, 1, (int[]){TK_EPSILON});
    insert_production_rule(G, term, 2, (int[]){factor, termPrime});
    insert_production_rule(G, termPrime, 3, (int[]){highPrecedenceOperators, factor, termPrime});
    insert_production_rule(G, termPrime, 1, (int[]){TK_EPSILON});
    insert_production_rule(G, factor, 3, (int[]){TK_OP, arithmeticExpression, TK_CL});
    insert_production_rule(G, factor, 1, (int[]){var});
    insert_production_rule(G, highPrecedenceOperators, 1, (int[]){TK_MUL});
    insert_production_rule(G, highPrecedenceOperators, 1, (int[]){TK_DIV});
    insert_production_rule(G, lowPrecedenceOperators, 1, (int[]){TK_PLUS});
    insert_production_rule(G, lowPrecedenceOperators, 1, (int[]){TK_MINUS});
    insert_production_rule(G, booleanExpression, 7, (int[]){TK_OP, booleanExpression, TK_CL, logicalOp, TK_OP, booleanExpression, TK_CL});
    insert_production_rule(G, booleanExpression, 3, (int[]){var, relationalOp, var});
    insert_production_rule(G, booleanExpression, 4, (int[]){TK_NOT, TK_OP, booleanExpression, TK_CL});
    insert_production_rule(G, var, 1, (int[]){singleOrRecId});
    insert_production_rule(G, var, 1, (int[]){TK_NUM});
    insert_production_rule(G, var, 1, (int[]){TK_RNUM});
    insert_production_rule(G, logicalOp, 1, (int[]){TK_AND});
    insert_production_rule(G, logicalOp, 1, (int[]){TK_OR});
    insert_production_rule(G, relationalOp, 1, (int[]){TK_LT});
    insert_production_rule(G, relationalOp, 1, (int[]){TK_LE});
    insert_production_rule(G, relationalOp, 1, (int[]){TK_EQ});
    insert_production_rule(G, relationalOp, 1, (int[]){TK_GT});
    insert_production_rule(G, relationalOp, 1, (int[]){TK_GE});
    insert_production_rule(G, relationalOp, 1, (int[]){TK_NE});
    insert_production_rule(G, returnStmt, 3, (int[]){TK_RETURN, optionalReturn, TK_SEM});
    insert_production_rule(G, optionalReturn, 3, (int[]){TK_SQL, idList, TK_SQR});
    insert_production_rule(G, optionalReturn, 1, (int[]){TK_EPSILON});
    insert_production_rule(G, idList, 2, (int[]){TK_ID, more_ids});
    insert_production_rule(G, more_ids, 2, (int[]){TK_COMMA, idList});
    insert_production_rule(G, more_ids, 1, (int[]){TK_EPSILON});
    insert_production_rule(G, definetypestmt, 5, (int[]){TK_DEFINETYPE, A, TK_RUID, TK_AS, TK_RUID});
    insert_production_rule(G, A, 1, (int[]){TK_RECORD});
    insert_production_rule(G, A, 1, (int[]){TK_UNION});
}
