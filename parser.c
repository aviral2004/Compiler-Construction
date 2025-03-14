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

FirstAndFollow ComputeFirstAndFollowSets(grammer G)
{
    FirstAndFollow F;
    F.no_of_terminals = G.no_of_terminals;
    F.no_of_variables = G.no_of_variables;
    // Initialize the first and follow sets
    for (int i = 0; i < G.no_of_variables; i++)
    {
        F.start_variable[i] = G.start_variable[i];
        F.elements[i].no_of_first = 0;
        F.elements[i].no_of_follow = 0;
    }
    // Copy the terminals
    for (int i = 0; i < G.no_of_terminals; i++)
    {
        F.terminals[i] = G.terminals[i];
    }

    // Compute First Sets
    bool computed[G.no_of_variables]; // to check if the first set of a variable is computed
    for (int i = 0; i < G.no_of_variables; i++)
    {
        computed[i] = false;
    }
    for (int i = 0; i < G.no_of_variables;)
    {
        if (computed[i] == false)
        {
            // Define a stack to store variables to be computed
            int queue[G.no_of_variables];
            int front = 0;
            int rear = 0;
            queue[rear] = i;
            rear++;
            // Check if all non terminals needed are computed
            while (front < rear)
            {
                int current = queue[front];
                front++;
                if (computed[current])
                {
                    continue;
                }
                variable *v = G.variables[current];
                bool is_computed = true;
                while (v != NULL)
                {
                    if (v->no_of_tokens > 0)
                    {
                        int k = 0;
                        bool is_TK_EPSILON = true;
                        // Iterate through all the ptokens of the rule
                        while (k < v->no_of_tokens && is_TK_EPSILON == true)
                        {
                            if (v->ptokens[k]->is_terminal == 1)
                            {
                                break;
                            }
                            else
                            {
                                // If the token is a variable
                                if (computed[v->ptokens[k]->name] == false && current != v->ptokens[k]->name)
                                {
                                    queue[rear] = v->ptokens[k]->name;
                                    rear++;
                                    is_computed = false;
                                }
                                else
                                {
                                    is_TK_EPSILON = false;
                                    // Check if TK_EPSILON is present in the first set of the variable
                                    for (int l = 0; l < F.elements[v->ptokens[k]->name].no_of_first; l++)
                                    {
                                        if (F.elements[v->ptokens[k]->name].first[l] == TK_EPSILON)
                                        {
                                            is_TK_EPSILON = true;
                                            break;
                                        }
                                    }
                                }
                            }
                            k++;
                        }
                    }
                    v = v->next;
                }
                // If first set can be computed
                if (is_computed == true)
                {
                    variable *v = G.variables[current];
                    while (v != NULL)
                    {
                        // Iterate through all the rules of the variable
                        if (v->no_of_tokens > 0)
                        {
                            int k = 0;
                            bool is_TK_EPSILON = true;
                            // Iterate through all the ptokens of the rule
                            while (k < v->no_of_tokens && is_TK_EPSILON == true)
                            {
                                if (v->ptokens[k]->is_terminal == 1)
                                {
                                    bool is_present = false;
                                    for (int l = 0; l < F.elements[current].no_of_first; l++)
                                    {
                                        if (F.elements[current].first[l] == v->ptokens[k]->name)
                                        {
                                            is_present = true;
                                            break;
                                        }
                                    }
                                    // If the terminal is not present in the first set of the variable then add it to the first set
                                    if (is_present == false)
                                    {
                                        F.elements[current].first[F.elements[current].no_of_first] = v->ptokens[k]->name;
                                        F.elements[current].first_rule[F.elements[current].no_of_first] = v->rule_no;
                                        F.elements[current].no_of_first++;
                                    }
                                    is_TK_EPSILON = false;
                                }
                                else
                                {
                                    is_TK_EPSILON = false;
                                    for (int l = 0; l < F.elements[v->ptokens[k]->name].no_of_first; l++)
                                    {
                                        // Check if TK_EPSILON is present in the first set of the variable
                                        if (F.elements[v->ptokens[k]->name].first[l] == TK_EPSILON)
                                        {
                                            is_TK_EPSILON = true;
                                            continue;
                                        }
                                        bool is_present = false;
                                        // Check if the terminal is already present in the first set of the variable
                                        for (int r = 0; r < F.elements[current].no_of_first; r++)
                                        {
                                            if (F.elements[current].first[r] == F.elements[v->ptokens[k]->name].first[l])
                                            {
                                                is_present = true;
                                                break;
                                            }
                                        }
                                        // If the terminal is not present in the first set of the variable then add it to the first set
                                        if (is_present == false)
                                        {
                                            F.elements[current].first[F.elements[current].no_of_first] = F.elements[v->ptokens[k]->name].first[l];
                                            F.elements[current].first_rule[F.elements[current].no_of_first] = v->rule_no;
                                            F.elements[current].no_of_first++;
                                        }
                                    }
                                }
                                k++;
                            }
                            // If TK_EPSILON is present in the first set of the variable then add it to the first set
                            if (is_TK_EPSILON == true)
                            {
                                bool is_present = false;
                                for (int l = 0; l < F.elements[current].no_of_first; l++)
                                {
                                    if (F.elements[current].first[l] == TK_EPSILON)
                                    {
                                        is_present = true;
                                        break;
                                    }
                                }
                                if (is_present == false)
                                {
                                    F.elements[current].first[F.elements[current].no_of_first] = TK_EPSILON;
                                    F.elements[current].first_rule[F.elements[current].no_of_first] = v->rule_no;
                                    F.elements[current].no_of_first++;
                                }
                            }
                        }
                        // Go to the next rule
                        v = v->next;
                    }
                    // Mark the variable as computed
                    computed[current] = true;
                }
            }
        }
        else
        {
            // If first set is already computed
            i++;
        }
    }

    // compute follow sets
    // Assumption 0 is the start variable and therefore adding $ in its follow set represented by TK_DOLLAR
    F.elements[0].follow[F.elements[0].no_of_follow] = TK_DOLLAR;
    F.elements[0].follow_rule[F.elements[0].no_of_follow] = 0;
    F.elements[0].no_of_follow++;
    for (int i = 0; i < G.no_of_variables; i++)
    {
        // iterate through all the starting variables
        variable *v = G.variables[i];
        while (v != NULL)
        {
            // iterate through all the rules of the variable
            if (v->no_of_tokens > 0)
            {
                for (int k = 0; k < v->no_of_tokens; k++)
                {
                    // iterate through all the ptokens of the rule
                    if (v->ptokens[k]->is_terminal == 0)
                    {
                        // if the token is a variable
                        int c = -2;
                        for (int i = 0; i < F.no_of_terminals; i++)
                        {
                            if (F.start_variable[i] == v->ptokens[k]->name)
                            {
                                c = i;
                                break;
                            }
                        }
                        if (k < v->no_of_tokens - 1)
                        {
                            // if the token is not the last token of the rule
                            if (v->ptokens[k + 1]->is_terminal == 1)
                            {
                                // if the next token is a terminal
                                // check if the terminal is already present in the follow set of the current token
                                bool is_present = false;
                                for (int l = 0; l < F.elements[c].no_of_follow; l++)
                                {
                                    if (F.elements[c].follow[l] == v->ptokens[k + 1]->name)
                                    {
                                        is_present = true;
                                        break;
                                    }
                                }
                                // if the terminal is not present in the follow set of the current token then add it to the follow set
                                if (is_present == false)
                                {
                                    F.elements[c].follow[F.elements[c].no_of_follow] = v->ptokens[k + 1]->name;
                                    F.elements[c].follow_rule[F.elements[c].no_of_follow] = v->rule_no;
                                    F.elements[c].no_of_follow++;
                                }
                            }
                            else
                            {
                                // if the next token is a variable
                                int l = k + 1;
                                bool is_TK_EPSILON = true;
                                while (l < v->no_of_tokens && is_TK_EPSILON == true)
                                {
                                    // iterate through all the ptokens after the token k
                                    if (v->ptokens[l]->is_terminal == 1)
                                    {
                                        // if the token is a terminal
                                        // check if the terminal is already present in the first set of the current token
                                        bool is_present = false;
                                        is_TK_EPSILON = false;
                                        for (int r = 0; r < F.elements[c].no_of_first; r++)
                                        {
                                            if (F.elements[c].first[r] == v->ptokens[l]->name)
                                            {
                                                is_present = true;
                                                break;
                                            }
                                        }
                                        // if the terminal is not present in the first set of the next token then add it to the follow set of the current token
                                        if (is_present == false)
                                        {
                                            F.elements[c].follow[F.elements[c].no_of_follow] = v->ptokens[l]->name;
                                            F.elements[c].follow_rule[F.elements[c].no_of_follow] = v->rule_no;
                                            F.elements[c].no_of_follow++;
                                        }
                                        is_TK_EPSILON = false;
                                    }
                                    else
                                    {
                                        // if the token is a variable
                                        int lindex = -2;
                                        for (int i = 0; i < F.no_of_terminals; i++)
                                        {
                                            if (F.start_variable[i] == v->ptokens[l]->name)
                                            {
                                                lindex = i;
                                                break;
                                            }
                                        }
                                        is_TK_EPSILON = false;
                                        for (int r = 0; r < F.elements[lindex].no_of_first; r++)
                                        {
                                            if (F.elements[lindex].first[r] == TK_EPSILON)
                                            {
                                                is_TK_EPSILON = true;
                                                continue;
                                            }
                                            bool is_present = false;
                                            for (int m = 0; m < F.elements[c].no_of_follow; m++)
                                            {
                                                if (F.elements[c].follow[m] == F.elements[lindex].first[r])
                                                {
                                                    is_present = true;
                                                    break;
                                                }
                                            }
                                            if (is_present == false)
                                            {
                                                F.elements[c].follow[F.elements[c].no_of_follow] = F.elements[lindex].first[r];
                                                F.elements[c].follow_rule[F.elements[c].no_of_follow] = v->rule_no;
                                                F.elements[c].no_of_follow++;
                                            }
                                        }
                                    }
                                    l++;
                                }
                                // if is_TK_EPSILON is true then add the lhs variable to the follow set of the current token which will be computed later
                                if (is_TK_EPSILON == true)
                                {
                                    bool is_present = false;
                                    for (int r = 0; r < F.elements[c].no_of_follow; r++)
                                    {
                                        if (F.elements[c].follow[r] == G.start_variable[i])
                                        {
                                            is_present = true;
                                            break;
                                        }
                                    }
                                    if (is_present == false)
                                    {
                                        F.elements[c].follow[F.elements[c].no_of_follow] = G.start_variable[i];
                                        F.elements[c].follow_rule[F.elements[c].no_of_follow] = v->rule_no;
                                        F.elements[c].no_of_follow++;
                                    }
                                }
                            }
                        }
                        // if the token is the last token of the rule
                        else
                        {
                            bool is_present = false;
                            for (int l = 0; l < F.elements[c].no_of_follow; l++)
                            {
                                if (F.elements[c].follow[l] == G.start_variable[i])
                                {
                                    is_present = true;
                                    break;
                                }
                            }
                            if (is_present == false)
                            {
                                F.elements[c].follow[F.elements[c].no_of_follow] = G.start_variable[i];
                                F.elements[c].follow_rule[F.elements[c].no_of_follow] = v->rule_no;
                                F.elements[c].no_of_follow++;
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
    for (int i = 0; i < G.no_of_terminals; i++)
    {
        computed[i] = false;
    }

    // iterate through all follow sets
    for (int i = 0; i < F.no_of_variables; i++)
    {
        if (computed[F.start_variable[i]] == false)
        {
            // if the follow set is not computed iterate through the follow set and check if there is a non-terminal
            for (int j = 0; j < F.elements[i].no_of_follow; j++)
            {
                bool check = false;
                for (int k = 0; k < F.no_of_variables; k++)
                {
                    if (F.elements[i].follow[j] == F.start_variable[k])
                    {
                        check = true;
                        break;
                    }
                }
                if (F.elements[i].follow[j] != TK_DOLLAR && check == true)
                {
                    // if there is a non-terminal then add the follow set of the non-terminal to the follow set of the current token
                    for (int k = 0; k < F.elements[F.elements[i].follow[j]].no_of_follow; k++)
                    {
                        bool is_present = false;
                        for (int l = 0; l < F.elements[i].no_of_follow; l++)
                        {
                            if (F.elements[i].follow[l] == F.elements[F.elements[i].follow[j]].follow[k])
                            {
                                is_present = true;
                                break;
                            }
                        }
                        if (is_present == false)
                        {
                            F.elements[i].follow[F.elements[i].no_of_follow] = F.elements[F.elements[i].follow[j]].follow[k];
                            F.elements[i].follow_rule[F.elements[i].no_of_follow] = F.elements[F.elements[i].follow[j]].follow_rule[k];
                            F.elements[i].no_of_follow++;
                        }
                    }
                }
            }
            computed[F.start_variable[i]] = true;
        }
    }
    // iterate through all the follow sets
    for (int i = 0; i < F.no_of_variables; i++)
    {
        // iterate through the follow set to check if a non_terminal is present
        for (int j = 0; j < F.elements[i].no_of_follow; j++)
        {
            if (F.elements[i].follow[j] == TK_DOLLAR)
                continue;

            bool check = false;
            for (int k = 0; k < F.no_of_variables; k++)
            {
                if (F.elements[i].follow[j] == F.start_variable[k])
                {
                    check = true;
                    break;
                }
            }
            if (check == true)
            {
                // if a non-terminal is present , remove it from the follow set
                for (int k = j; k < F.elements[i].no_of_follow - 1; k++)
                {
                    F.elements[i].follow[k] = F.elements[i].follow[k + 1];
                    F.elements[i].follow_rule[k] = F.elements[i].follow_rule[k + 1];
                }
                F.elements[i].no_of_follow--;
                j--;
            }
        }
    }
    return F;
}

bool createParseTable(FirstAndFollow F, table *T)
{
    // Find number of terminal ptokens and variables
    T->no_of_rows = F.no_of_variables;
    T->no_of_columns = F.no_of_terminals;
    // initialize the rows
    for (int i = 0; i < F.no_of_variables; i++)
    {
        T->row[i] = F.start_variable[i];
    }
    // initialize the columns
    for (int i = 0; i < F.no_of_terminals; i++)
    {
        T->column[i] = F.terminals[i];
    }
    // initialize the tables
    for (int i = 0; i < F.no_of_variables; i++)
    {
        for (int j = 0; j < F.no_of_terminals; j++)
        {
            T->table[i][j] = INT_MIN;
        }
    }

    // Heuristic
    for (int i = 0; i < F.no_of_variables; i++)
    {
        for (int j = 0; j < F.elements[i].no_of_follow; j++)
        {
            int c = -2;
            for (int z = 0; z < F.no_of_terminals; z++)
            {
                if (F.elements[i].follow[j] == T->column[z])
                {
                    c = z;
                    break;
                }
            }
            T->table[i][c] = INT_MAX;
        }
    }

    bool is_LL1 = true;
    // iterate through all the variables
    for (int i = 0; i < F.no_of_variables; i++)
    {
        // iterate through the first set of the variable
        for (int j = 0; j < F.elements[i].no_of_first; j++)
        {
            // if the first set contains TK_EPSILON then add the follow set of the variable to the parse table
            if (F.elements[i].first[j] == TK_EPSILON)
            {
                for (int k = 0; k < F.elements[i].no_of_follow; k++)
                {
                    int c = -2;
                    for (int z = 0; z < F.no_of_terminals; z++)
                    {
                        if (F.elements[i].follow[k] == T->column[z])
                        {
                            c = z;
                            break;
                        }
                    }
                    if (T->table[i][c] != INT_MIN)
                    {
                        is_LL1 = false;
                    }
                    T->table[i][c] = F.elements[i].first_rule[j];
                }
            }
            else
            {
                // if the first set contains a terminal then add the rule to the parse table
                if (T->table[i][F.elements[i].first[j] - 1] != INT_MIN)
                {
                    is_LL1 = false;
                }
                int c = -2;
                for (int z = 0; z < F.no_of_terminals; z++)
                {
                    if (F.elements[i].first[j] == T->column[z])
                    {
                        c = z;
                        break;
                    }
                }
                T->table[i][c] = F.elements[i].first_rule[j];
            }
        }
    }
    return is_LL1;
}

parseTree *parseInputSourceCode(table T, FirstAndFollow F, grammer *G, vector input)
{
    // parse the input using the parse table and print the parse tree
    // if the input is not valid then print "Input is not valid"

    vector filteredv = init_vector();
    for (int i = 0; i < input->size; i++){
        if (get(input, i)->tk != TK_COMMENT){
            push_back(filteredv, get(input, i));
        }
    }
    input = filteredv;
    
    // Define the stack
    int n = input->size;
    int stack[100];
    // Define the parse tree stack
    parseTree *ptree[100];
    int top = -1;
    int treetop = -1;
    // Initialize the stack and parse tree stack
    stack[++top] = TK_DOLLAR;
    stack[++top] = G->start_variable[0];
    ptree[++treetop] = (parseTree *)malloc(sizeof(parseTree));
    ptree[treetop]->t = (token)malloc(sizeof(Token));
    ptree[treetop]->t->tk = stack[top];
    ptree[treetop]->t->lexeme = NULL;
    ptree[treetop]->t->lc = -1;
    ptree[treetop]->parent = -1;
    ptree[treetop]->no_of_children = 0;
    parseTree *root = ptree[treetop];
    int i = 0;
    int erroc = 1, err = 0;
    // Iterate through the input
    while (i < n && top > 0)
    {
        int c = -2;
        // Find the column of the token in the parse table
        for (int j = 0; j < F.no_of_terminals; j++)
        {
            if (get(input, i)->tk == T.column[j])
            {
                c = j;
                break;
            }
        }
        int r = -2;
        // Find the row of the variable in the parse table
        for (int j = 0; j < F.no_of_variables; j++)
        {
            if (stack[top] == T.row[j])
            {
                r = j;
                break;
            }
        }
        int flag = 1;
        // Check if the token is valid
        if (get(input, i)->tk == TK_INVALID || c == -2)
        {
            red("Line %d Error: %s\n", get(input, i)->lc, get(input, i)->lexeme);
            i++;
            erroc = 0;
            err = 1;
            continue;
        }
        // Check if the token is valid
        if (r == -2)
        {
            if (erroc)
                red("Line %d Error: The token %s for lexeme %s does not math with the expected token %s\n", get(input, i)->lc, TOKENS[get(input, i)->tk], get(input, i)->lexeme, TOKENS[stack[top]]);
            top--;
            treetop--;
            erroc = 0;
            err = 1;
            flag = 0;
        }
        // Check if the token is valid
        if (T.table[r][c] == INT_MIN)
        {
            if (erroc)
                red("Line %d Error: Invalid token %s encountered with %s stack top %s\n", get(input, i)->lc, TOKENS[get(input, i)->tk], get(input, i)->lexeme, TOKENS[stack[top]]);
            i++;
            erroc = 0;
            err = 1;
            continue;
        }
        // Check if the token is valid
        if (T.table[r][c] == INT_MAX)
        {
            if (erroc)
                red("Line %d Error: Invalid token %s encountered with %s stack top %s\n", get(input, i)->lc, TOKENS[get(input, i)->tk], get(input, i)->lexeme, TOKENS[stack[top]]);
            top--;
            treetop--;
            erroc = 0;
            flag = 0;
            err = 1;
        }
        if (flag)
        {
            // Add the rule to the parse tree
            variable *v = G->variables[r];
            while (v != NULL)
            {
                // Retrieve the rule from the parse table
                if (v->rule_no == T.table[r][c])
                {
                    // Replace the variable with the rule in the stack
                    top--;
                    parseTree *temp = ptree[treetop];
                    treetop--;
                    for (int j = v->no_of_tokens - 1; j >= 0; j--)
                    {
                        top += 1;
                        treetop += 1;
                        stack[top] = v->ptokens[j]->name;
                        ptree[treetop] = (parseTree *)malloc(sizeof(parseTree));
                        ptree[treetop]->t = (token)malloc(sizeof(Token));
                        ptree[treetop]->t->tk = stack[top];
                        ptree[treetop]->t->lexeme = NULL;
                        ptree[treetop]->t->lc = -1;
                        ptree[treetop]->parent = temp->t->tk;
                        ptree[treetop]->no_of_children = 0;
                        temp->children[temp->no_of_children] = ptree[treetop];
                        temp->no_of_children++;
                    }

                    break;
                }
                v = v->next;
            }
        }
        // Check if the stack element is equal to the input token
        while (top > 0 && i < n && (stack[top] == (get(input, i)->tk) || stack[top] == TK_EPSILON))
        {
            ptree[treetop]->t->lexeme = get(input, i)->lexeme;
            ptree[treetop]->t->lc = get(input, i)->lc;
            if (stack[top] != TK_EPSILON)
                i++;
            top--;
            treetop--;
            erroc = 1;
        }
    }
    // Check if the stack is empty and the input is valid
    if (stack[top] == TK_DOLLAR && err == 0)
    {
        green("COMPILATION SUCCESSFUL \n");
    }
    else
    {
        red("COMPILATION ERROR\n");
    }
    return root;
}

void printParseTree(parseTree *ptree, FILE *fp)
{
    // print the parse tree in the file
    // if the file is NULL then print the parse tree in the console
    if (ptree == NULL)
    {
        return;
    }
    // If the node is not a leaf node then print the first child
    if (ptree->no_of_children!=0 && ptree->children[0] != NULL)
    {
        printParseTree(ptree->children[0], fp);
    }
    // Print the node
    if (fp != NULL)
    {
        // Print the lexeme
        if (ptree->t->lexeme != NULL)
        {
            fprintf(fp, "%-30s", ptree->t->lexeme);
        }
        else
        {
            fprintf(fp, "%-30s", "----");
        }
        // Print the line number
        if (ptree->t->lc != -1)
        {
            fprintf(fp, "%-30d", ptree->t->lc);
        }
        else
        {
            fprintf(fp, "%-30s", "----");
        }
        // Print tokenName
        fprintf(fp, "%-30d", ptree->t->tk);
        // Print ValueIfNumber
        if (ptree->t->tk == TK_NUM || ptree->t->tk == TK_RNUM)
        {
            fprintf(fp, "%-30s", ptree->t->lexeme);
        }
        else
        {
            fprintf(fp, "%-30s", "----");
        }
        // Print Parent Node Symbol
        if (ptree->parent != -1)
        {
            fprintf(fp, "%-30s", TOKENS[ptree->parent]);
        }
        else
        {
            fprintf(fp, "%-30s", "----");
        }
        // Print isLeaf
        if (ptree->no_of_children == 0)
        {
            fprintf(fp, "%-30s", "YES");
        }
        else
        {
            fprintf(fp, "%-30s", "NO");
        }
        // Print NodeSymbol
        fprintf(fp, "%s", TOKENS[ptree->t->tk]);
        fprintf(fp, "\n");
    }
    // If the node is not a leaf node then print the rest of the children
    for (int i = 1; i < ptree->no_of_children; i++)
    {
        if(ptree->children[i]!=NULL)
        {
            printParseTree(ptree->children[i], fp);
        }
    }
}

void add_grammer_rule(grammer *G, int start_variable, int no_of_tokens, int *ptokens)
{
    // finding the index of the lhs variable
    int index;
    for (int i = 0; i < G->no_of_variables; i++)
    {
        index = i;
        if (G->start_variable[i] == start_variable)
        {
            break;
        }
    }
    // iterate through the rules of the variable to reach the end of the linked list
    variable *v;
    if (G->variables[index] == NULL)
    {
        G->variables[index] = (variable *)malloc(sizeof(variable));
        v = G->variables[index];
    }
    else
    {
        v = G->variables[index];
        while (v->next != NULL)
        {
            v = v->next;
        }
        v->next = (variable *)malloc(sizeof(variable));
        v = v->next;
    }
    // add the rule to the grammer
    v->no_of_tokens = no_of_tokens;
    v->rule_no = G->no_of_rules + 1;
    for (int i = 0; i < no_of_tokens; i++)
    {

        v->ptokens[i] = (ptoken *)malloc(sizeof(ptoken));
        v->ptokens[i]->name = ptokens[i];
        v->ptokens[i]->is_terminal = 0;
        for (int j = 0; j < G->no_of_terminals; j++)
        {
            if (G->terminals[j] == ptokens[i])
            {
                v->ptokens[i]->is_terminal = 1;
                break;
            }
        }
    }
    // print the rule
    v->next = NULL;
    G->no_of_rules++;
}
void grammer_init(grammer *G)
{
    // Initialize the grammer
    G->no_of_variables = 53;
    G->no_of_terminals = 61;
    G->no_of_rules = 0;

    // Define the start variables
    G->start_variable[0] = program;
    G->start_variable[1] = mainFunction;
    G->start_variable[2] = otherFunctions;
    G->start_variable[3] = function;
    G->start_variable[4] = input_par;
    G->start_variable[5] = output_par;
    G->start_variable[6] = parameter_list;
    G->start_variable[7] = dataType;
    G->start_variable[8] = primitiveDatatype;
    G->start_variable[9] = constructedDatatype;
    G->start_variable[10] = remaining_list;
    G->start_variable[11] = stmts;
    G->start_variable[12] = typeDefinitions;
    G->start_variable[13] = typeDefinition;
    G->start_variable[14] = actualOrRedefined;
    G->start_variable[15] = fieldDefinitions;
    G->start_variable[16] = fieldDefinition;
    G->start_variable[17] = fieldType;
    G->start_variable[18] = moreFields;
    G->start_variable[19] = declarations;
    G->start_variable[20] = declaration;
    G->start_variable[21] = global_or_not;
    G->start_variable[22] = otherStmts;
    G->start_variable[23] = stmt;
    G->start_variable[24] = assignmentStmt;
    G->start_variable[25] = singleOrRecId;
    G->start_variable[26] = option_single_constructed;
    G->start_variable[27] = oneExpansion;
    G->start_variable[28] = moreExpansions;
    G->start_variable[29] = funCallStmt;
    G->start_variable[30] = outputParameters;
    G->start_variable[31] = inputParameters;
    G->start_variable[32] = iterativeStmt;
    G->start_variable[33] = conditionalStmt;
    G->start_variable[34] = elsePart;
    G->start_variable[35] = ioStmt;
    G->start_variable[36] = arithmeticExpression;
    G->start_variable[37] = expPrime;
    G->start_variable[38] = termPrime;
    G->start_variable[39] = term;
    G->start_variable[40] = factor;
    G->start_variable[41] = lowPrecedenceOperators;
    G->start_variable[42] = highPrecedenceOperators;
    G->start_variable[43] = booleanExpression;
    G->start_variable[44] = var;
    G->start_variable[45] = logicalOp;
    G->start_variable[46] = relationalOp;
    G->start_variable[47] = returnStmt;
    G->start_variable[48] = optionalReturn;
    G->start_variable[49] = idList;
    G->start_variable[50] = more_ids;
    G->start_variable[51] = definetypestmt;
    G->start_variable[52] = A;

    // Define the terminals
    G->terminals[0] = TK_NOTOKEN;
    G->terminals[1] = TK_INVALID;
    G->terminals[2] = TK_ASSIGNOP;
    G->terminals[3] = TK_COMMENT;
    G->terminals[4] = TK_FIELDID;
    G->terminals[5] = TK_ID;
    G->terminals[6] = TK_NUM;
    G->terminals[7] = TK_RNUM;
    G->terminals[8] = TK_FUNID;
    G->terminals[9] = TK_RUID;
    G->terminals[10] = TK_WITH;
    G->terminals[11] = TK_PARAMETERS;
    G->terminals[12] = TK_END;
    G->terminals[13] = TK_WHILE;
    G->terminals[14] = TK_UNION;
    G->terminals[15] = TK_ENDUNION;
    G->terminals[16] = TK_DEFINETYPE;
    G->terminals[17] = TK_AS;
    G->terminals[18] = TK_TYPE;
    G->terminals[19] = TK_MAIN;
    G->terminals[20] = TK_GLOBAL;
    G->terminals[21] = TK_PARAMETER;
    G->terminals[22] = TK_LIST;
    G->terminals[23] = TK_SQL;
    G->terminals[24] = TK_SQR;
    G->terminals[25] = TK_INPUT;
    G->terminals[26] = TK_OUTPUT;
    G->terminals[27] = TK_INT;
    G->terminals[28] = TK_REAL;
    G->terminals[29] = TK_COMMA;
    G->terminals[30] = TK_SEM;
    G->terminals[31] = TK_COLON;
    G->terminals[32] = TK_DOT;
    G->terminals[33] = TK_ENDWHILE;
    G->terminals[34] = TK_OP;
    G->terminals[35] = TK_CL;
    G->terminals[36] = TK_IF;
    G->terminals[37] = TK_THEN;
    G->terminals[38] = TK_ENDIF;
    G->terminals[39] = TK_READ;
    G->terminals[40] = TK_WRITE;
    G->terminals[41] = TK_RETURN;
    G->terminals[42] = TK_PLUS;
    G->terminals[43] = TK_MINUS;
    G->terminals[44] = TK_MUL;
    G->terminals[45] = TK_DIV;
    G->terminals[46] = TK_CALL;
    G->terminals[47] = TK_RECORD;
    G->terminals[48] = TK_ENDRECORD;
    G->terminals[49] = TK_ELSE;
    G->terminals[50] = TK_AND;
    G->terminals[51] = TK_OR;
    G->terminals[52] = TK_NOT;
    G->terminals[53] = TK_LT;
    G->terminals[54] = TK_LE;
    G->terminals[55] = TK_EQ;
    G->terminals[56] = TK_GT;
    G->terminals[57] = TK_GE;
    G->terminals[58] = TK_NE;
    G->terminals[59] = TK_DOLLAR;
    G->terminals[60] = TK_EPSILON;

    // Initialize the variables with NULL
    for (int i = 0; i < G->no_of_variables; i++)
    {
        G->variables[i] = NULL;
    }

    // Define the rules
    add_grammer_rule(G, program, 2, (int[]){otherFunctions, mainFunction});
    add_grammer_rule(G, mainFunction, 3, (int[]){TK_MAIN, stmts, TK_END});
    add_grammer_rule(G, otherFunctions, 2, (int[]){function, otherFunctions});
    add_grammer_rule(G, otherFunctions, 1, (int[]){TK_EPSILON});
    add_grammer_rule(G, function, 6, (int[]){TK_FUNID, input_par, output_par, TK_SEM, stmts, TK_END});
    add_grammer_rule(G, input_par, 6, (int[]){TK_INPUT, TK_PARAMETER, TK_LIST, TK_SQL, parameter_list, TK_SQR});
    add_grammer_rule(G, output_par, 6, (int[]){TK_OUTPUT, TK_PARAMETER, TK_LIST, TK_SQL, parameter_list, TK_SQR});
    add_grammer_rule(G, output_par, 1, (int[]){TK_EPSILON});
    add_grammer_rule(G, parameter_list, 3, (int[]){dataType, TK_ID, remaining_list});
    add_grammer_rule(G, dataType, 1, (int[]){primitiveDatatype});
    add_grammer_rule(G, dataType, 1, (int[]){constructedDatatype});
    add_grammer_rule(G, primitiveDatatype, 1, (int[]){TK_INT});
    add_grammer_rule(G, primitiveDatatype, 1, (int[]){TK_REAL});
    add_grammer_rule(G, constructedDatatype, 2, (int[]){TK_RECORD, TK_RUID});
    add_grammer_rule(G, constructedDatatype, 2, (int[]){TK_UNION, TK_RUID});
    add_grammer_rule(G, constructedDatatype, 1, (int[]){TK_RUID});
    add_grammer_rule(G, remaining_list, 2, (int[]){TK_COMMA, parameter_list});
    add_grammer_rule(G, remaining_list, 1, (int[]){TK_EPSILON});
    add_grammer_rule(G, stmts, 4, (int[]){typeDefinitions, declarations, otherStmts, returnStmt});
    add_grammer_rule(G, typeDefinitions, 2, (int[]){actualOrRedefined, typeDefinitions});
    add_grammer_rule(G, typeDefinitions, 1, (int[]){TK_EPSILON});
    add_grammer_rule(G, actualOrRedefined, 1, (int[]){typeDefinition});
    add_grammer_rule(G, actualOrRedefined, 1, (int[]){definetypestmt});
    add_grammer_rule(G, typeDefinition, 4, (int[]){TK_RECORD, TK_RUID, fieldDefinitions, TK_ENDRECORD});
    add_grammer_rule(G, typeDefinition, 4, (int[]){TK_UNION, TK_RUID, fieldDefinitions, TK_ENDUNION});
    add_grammer_rule(G, fieldDefinitions, 3, (int[]){fieldDefinition, fieldDefinition, moreFields});
    add_grammer_rule(G, fieldDefinition, 5, (int[]){TK_TYPE, fieldType, TK_COLON, TK_FIELDID, TK_SEM});
    add_grammer_rule(G, fieldType, 1, (int[]){primitiveDatatype});
    add_grammer_rule(G, fieldType, 1, (int[]){constructedDatatype});
    add_grammer_rule(G, moreFields, 2, (int[]){fieldDefinition, moreFields});
    add_grammer_rule(G, moreFields, 1, (int[]){TK_EPSILON});
    add_grammer_rule(G, declarations, 2, (int[]){declaration, declarations});
    add_grammer_rule(G, declarations, 1, (int[]){TK_EPSILON});
    add_grammer_rule(G, declaration, 6, (int[]){TK_TYPE, dataType, TK_COLON, TK_ID, global_or_not, TK_SEM});
    add_grammer_rule(G, global_or_not, 2, (int[]){TK_COLON, TK_GLOBAL});
    add_grammer_rule(G, global_or_not, 1, (int[]){TK_EPSILON});
    add_grammer_rule(G, otherStmts, 2, (int[]){stmt, otherStmts});
    add_grammer_rule(G, otherStmts, 1, (int[]){TK_EPSILON});
    add_grammer_rule(G, stmt, 1, (int[]){assignmentStmt});
    add_grammer_rule(G, stmt, 1, (int[]){iterativeStmt});
    add_grammer_rule(G, stmt, 1, (int[]){conditionalStmt});
    add_grammer_rule(G, stmt, 1, (int[]){ioStmt});
    add_grammer_rule(G, stmt, 1, (int[]){funCallStmt});
    add_grammer_rule(G, assignmentStmt, 4, (int[]){singleOrRecId, TK_ASSIGNOP, arithmeticExpression, TK_SEM});
    add_grammer_rule(G, singleOrRecId, 2, (int[]){TK_ID, option_single_constructed});
    add_grammer_rule(G, option_single_constructed, 1, (int[]){TK_EPSILON});
    add_grammer_rule(G, option_single_constructed, 2, (int[]){oneExpansion, moreExpansions});
    add_grammer_rule(G, oneExpansion, 2, (int[]){TK_DOT, TK_FIELDID});
    add_grammer_rule(G, moreExpansions, 2, (int[]){oneExpansion, moreExpansions});
    add_grammer_rule(G, moreExpansions, 1, (int[]){TK_EPSILON});
    add_grammer_rule(G, funCallStmt, 7, (int[]){outputParameters, TK_CALL, TK_FUNID, TK_WITH, TK_PARAMETERS, inputParameters, TK_SEM});
    add_grammer_rule(G, outputParameters, 4, (int[]){TK_SQL, idList, TK_SQR, TK_ASSIGNOP});
    add_grammer_rule(G, outputParameters, 1, (int[]){TK_EPSILON});
    add_grammer_rule(G, inputParameters, 3, (int[]){TK_SQL, idList, TK_SQR});
    add_grammer_rule(G, iterativeStmt, 7, (int[]){TK_WHILE, TK_OP, booleanExpression, TK_CL, stmt, otherStmts, TK_ENDWHILE});
    add_grammer_rule(G, conditionalStmt, 8, (int[]){TK_IF, TK_OP, booleanExpression, TK_CL, TK_THEN, stmt, otherStmts, elsePart});
    add_grammer_rule(G, elsePart, 4, (int[]){TK_ELSE, stmt, otherStmts, TK_ENDIF});
    add_grammer_rule(G, elsePart, 1, (int[]){TK_ENDIF});
    add_grammer_rule(G, ioStmt, 5, (int[]){TK_READ, TK_OP, var, TK_CL, TK_SEM});
    add_grammer_rule(G, ioStmt, 5, (int[]){TK_WRITE, TK_OP, var, TK_CL, TK_SEM});
    add_grammer_rule(G, arithmeticExpression, 2, (int[]){term, expPrime});
    add_grammer_rule(G, expPrime, 3, (int[]){lowPrecedenceOperators, term, expPrime});
    add_grammer_rule(G, expPrime, 1, (int[]){TK_EPSILON});
    add_grammer_rule(G, term, 2, (int[]){factor, termPrime});
    add_grammer_rule(G, termPrime, 3, (int[]){highPrecedenceOperators, factor, termPrime});
    add_grammer_rule(G, termPrime, 1, (int[]){TK_EPSILON});
    add_grammer_rule(G, factor, 3, (int[]){TK_OP, arithmeticExpression, TK_CL});
    add_grammer_rule(G, factor, 1, (int[]){var});
    add_grammer_rule(G, highPrecedenceOperators, 1, (int[]){TK_MUL});
    add_grammer_rule(G, highPrecedenceOperators, 1, (int[]){TK_DIV});
    add_grammer_rule(G, lowPrecedenceOperators, 1, (int[]){TK_PLUS});
    add_grammer_rule(G, lowPrecedenceOperators, 1, (int[]){TK_MINUS});
    add_grammer_rule(G, booleanExpression, 7, (int[]){TK_OP, booleanExpression, TK_CL, logicalOp, TK_OP, booleanExpression, TK_CL});
    add_grammer_rule(G, booleanExpression, 3, (int[]){var, relationalOp, var});
    add_grammer_rule(G, booleanExpression, 4, (int[]){TK_NOT, TK_OP, booleanExpression, TK_CL});
    add_grammer_rule(G, var, 1, (int[]){singleOrRecId});
    add_grammer_rule(G, var, 1, (int[]){TK_NUM});
    add_grammer_rule(G, var, 1, (int[]){TK_RNUM});
    add_grammer_rule(G, logicalOp, 1, (int[]){TK_AND});
    add_grammer_rule(G, logicalOp, 1, (int[]){TK_OR});
    add_grammer_rule(G, relationalOp, 1, (int[]){TK_LT});
    add_grammer_rule(G, relationalOp, 1, (int[]){TK_LE});
    add_grammer_rule(G, relationalOp, 1, (int[]){TK_EQ});
    add_grammer_rule(G, relationalOp, 1, (int[]){TK_GT});
    add_grammer_rule(G, relationalOp, 1, (int[]){TK_GE});
    add_grammer_rule(G, relationalOp, 1, (int[]){TK_NE});
    add_grammer_rule(G, returnStmt, 3, (int[]){TK_RETURN, optionalReturn, TK_SEM});
    add_grammer_rule(G, optionalReturn, 3, (int[]){TK_SQL, idList, TK_SQR});
    add_grammer_rule(G, optionalReturn, 1, (int[]){TK_EPSILON});
    add_grammer_rule(G, idList, 2, (int[]){TK_ID, more_ids});
    add_grammer_rule(G, more_ids, 2, (int[]){TK_COMMA, idList});
    add_grammer_rule(G, more_ids, 1, (int[]){TK_EPSILON});
    add_grammer_rule(G, definetypestmt, 5, (int[]){TK_DEFINETYPE, A, TK_RUID, TK_AS, TK_RUID});
    add_grammer_rule(G, A, 1, (int[]){TK_RECORD});
    add_grammer_rule(G, A, 1, (int[]){TK_UNION});
}