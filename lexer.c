/*
Group Number - 17
ID:	2022A7PS0173P			Name: Ameesh Sethi
ID:	2022A7PS0097P			Name: Aviral Gupta
ID:	2022A7PS0146P			Name: Yatharth Singh
ID:	2022A7PS1188P			Name: Armaan Sethi
ID:	2022A7PS0052P			Name: Utkarsh Tiwari
*/

#include "lexerDef.h"

static State states[MAX_STATES];
static trie look_up_table;

// Creates a new state transition with given character and next state
transition create_transition(char ch, state_id next_state)
{
    transition t = (transition)malloc(sizeof(Transition));
    t->next_state = &states[next_state];
    t->next_char = ch;
    return t;
}

// Helper function to create transitions for alphabets
transition *aToZExceptBToD(state_id next_state)
{
    int size = 23;
    transition *transitions = (transition *)malloc(size * sizeof(transition));
    int idx = 0;
    for (int i = 'a'; i <= 'z'; i++)
    {
        if (i != 'b' && i != 'c' && i != 'd')
        {
            transitions[idx++] = create_transition(i, next_state);
        }
    }
    return transitions;
}

// Creates transitions for all lowercase letters
transition *aToZ(state_id next_state)
{
    int size = 26;
    transition *transitions = (transition *)malloc(size * sizeof(transition));
    int idx = 0;
    for (int i = 'a'; i <= 'z'; i++)
    {
        transitions[idx++] = create_transition(i, next_state);
    }
    return transitions;
}

// Creates transitions for all letters (case insensitive)
transition *AToZ(state_id next_state)
{
    int size = 52;
    transition *transitions = (transition *)malloc(size * sizeof(transition));
    int idx = 0;
    for (int i = 'a'; i <= 'z'; i++)
    {
        transitions[idx++] = create_transition(i, next_state);
    }
    for (int i = 'A'; i <= 'Z'; i++)
    {
        transitions[idx++] = create_transition(i, next_state);
    }
    return transitions;
}

// Creates transitions for letters b through d
transition *bToD(state_id next_state)
{
    int size = 3;
    transition *transitions = (transition *)malloc(size * sizeof(transition));
    int idx = 0;
    for (int i = 'b'; i <= 'd'; i++)
    {
        transitions[idx++] = create_transition(i, next_state);
    }
    return transitions;
}

// Creates transitions for digits 0-9
transition *zeroToNine(state_id next_state)
{
    int size = 10;
    transition *transitions = (transition *)malloc(size * sizeof(transition));
    int idx = 0;
    for (int i = 0; i < 10; i++)
    {
        transitions[idx++] = create_transition(i + '0', next_state);
    }
    return transitions;
}

// Creates transitions for digits 2-7
transition *twoToSeven(state_id next_state)
{
    int size = 6;
    transition *transitions = (transition *)malloc(size * sizeof(transition));
    int idx = 0;
    for (int i = 0; i < 6; i++)
    {
        transitions[idx++] = create_transition(i + '2', next_state);
    }
    return transitions;
}

// Adds remaining transitions to complete the state graph
void theta(state_id curr_state, state_id next_state)
{
    int size = ALPHABET_SIZE - states[curr_state].length;
    transition *transitions = (transition *)malloc(size * sizeof(transition));
    int count = 0;
    int *char_used = (int *)calloc(128, sizeof(int));
    for (int j = 0; j < states[curr_state].length; j++)
    {
        char_used[states[curr_state].transitions[j]->next_char] = 1;
    }
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (char_used[ALPHABETS[i]] != 1)
        {
            transitions[count++] = create_transition(ALPHABETS[i], next_state);
        }
    }
    add_transition(curr_state, transitions, size);
    free(char_used);
}

// Creates a new token with given properties
token create_token(token_id id, int line_count, char *lexeme)
{
    token new_token = (token)malloc(sizeof(Token));
    new_token->tk = id;
    new_token->lc = line_count;
    new_token->lexeme = (char *)(malloc(30 * sizeof(char)));
    memset(new_token->lexeme, '\0', 30);
    strncpy(new_token->lexeme, lexeme, strlen(lexeme));
    return new_token;
}

// Adds transitions to the state
void add_transition(state_id state, transition *transitions, int transition_count)
{
    int new_length = states[state].length + transition_count;
    states[state].transitions = (transition *)realloc(states[state].transitions, new_length * sizeof(transition));
    for (int i = 0; i < transition_count; i++)
    {
        states[state].transitions[states[state].length + i] = transitions[i];
    }
    states[state].length = new_length;
}

// Initialize the lexical analyzer components
void initialize_lexer() {
    setupStates();
    setupTransitions();
    setupLookupTable();
}

// Initialize all states with default values and token mappings
void setupStates()
{
    // Set default values for all states
    for (int i = 0; i < MAX_STATES; i++)
    {
        states[i].length = 0;
        states[i].state_id = i;
        states[i].retract_count = 0;
        states[i].token = -1;
        states[i].transitions = NULL;
    }

    // Initialize special states
    states[INVALID].token = TK_INVALID;
    states[INVALID].retract_count = 1;

    // Initialize comment and whitespace states
    states[STATE_60].token = TK_COMMENT;
    states[STATE_61].token = TK_NOTOKEN;
    states[STATE_62].token = TK_NOTOKEN;

    // Initialize identifier states
    states[STATE_51].token = TK_NOTOKEN;
    states[STATE_54].token = TK_NOTOKEN;
    states[STATE_57].token = TK_ID;
    states[STATE_58].token = TK_FIELDID;

    // Initialize operator states
    states[STATE_19].token = TK_NOT;
    states[STATE_21].token = TK_EQ;
    states[STATE_23].token = TK_NE;
    states[STATE_18].token = TK_OR;
    states[STATE_15].token = TK_AND;
    states[STATE_31].token = TK_ASSIGNOP;

    // Initialize arithmetic operator states
    states[STATE_9].token = TK_PLUS;
    states[STATE_10].token = TK_MINUS;
    states[STATE_11].token = TK_MUL;
    states[STATE_12].token = TK_DIV;

    // Initialize relational operator states
    states[STATE_26].token = TK_GT;
    states[STATE_25].token = TK_GE;
    states[STATE_32].token = TK_LT;
    states[STATE_28].token = TK_LE;
    states[STATE_33].token = TK_LT;

    // Initialize delimiter states
    states[STATE_1].token = TK_SQL;
    states[STATE_2].token = TK_SQR;
    states[STATE_3].token = TK_COMMA;
    states[STATE_4].token = TK_SEM;
    states[STATE_5].token = TK_COLON;
    states[STATE_6].token = TK_DOT;
    states[STATE_7].token = TK_OP;
    states[STATE_8].token = TK_CL;

    // Initialize number states
    states[STATE_38].token = TK_NUM;
    states[STATE_40].token = TK_NUM;
    states[STATE_43].token = TK_RNUM;
    states[STATE_47].token = TK_RNUM;

    // Initialize special identifier states
    states[STATE_36].token = TK_RUID;

    // Set retract counts
    states[STATE_36].retract_count = 1;
    states[STATE_26].retract_count = 1;
    states[STATE_32].retract_count = 1;
    states[STATE_33].retract_count = 2;
    states[STATE_51].retract_count = 1;
    states[STATE_54].retract_count = 1;
    states[STATE_57].retract_count = 1;
    states[STATE_38].retract_count = 1;
    states[STATE_43].retract_count = 1;
    states[STATE_40].retract_count = 2;
    states[STATE_58].retract_count = 1;
}

// Initializes the transitions of the DFA
void setupTransitions()
{
    add_transition(START, (transition[]){create_transition('%', STATE_59), create_transition(' ', STATE_61), create_transition('\n', STATE_62), create_transition('~', STATE_19), create_transition('[', STATE_1), create_transition(']', STATE_2), create_transition(',', STATE_3), create_transition(';', STATE_4), create_transition(':', STATE_5), create_transition('.', STATE_6), create_transition('(', STATE_7), create_transition(')', STATE_8), create_transition('=', STATE_20), create_transition('+', STATE_9), create_transition('-', STATE_10), create_transition('!', STATE_22), create_transition('@', STATE_16), create_transition('&', STATE_13), create_transition('#', STATE_34), create_transition('*', STATE_11), create_transition('>', STATE_24), create_transition('<', STATE_27), create_transition('/', STATE_12), create_transition('_', STATE_48), create_transition('\t', STATE_61)}, 25);
    add_transition(START, bToD(STATE_52), 3);
    add_transition(START, zeroToNine(STATE_37), 10);
    add_transition(START, aToZExceptBToD(STATE_53), 23);
    add_transition(STATE_59, (transition[]){create_transition('\n', STATE_60)}, 1);
    theta(STATE_59, STATE_59);
    add_transition(STATE_20, (transition[]){create_transition('=', STATE_21)}, 1);
    add_transition(STATE_22, (transition[]){create_transition('=', STATE_23)}, 1);
    add_transition(STATE_16, (transition[]){create_transition('@', STATE_17)}, 1);
    add_transition(STATE_17, (transition[]){create_transition('@', STATE_18)}, 1);
    add_transition(STATE_13, (transition[]){create_transition('&', STATE_14)}, 1);
    add_transition(STATE_14, (transition[]){create_transition('&', STATE_15)}, 1);
    add_transition(STATE_34, aToZ(STATE_35), 26);
    add_transition(STATE_35, aToZ(STATE_35), 26);
    theta(STATE_35, STATE_36);
    add_transition(STATE_24, (transition[]){create_transition('=', STATE_25)}, 1);
    theta(STATE_24, STATE_26);
    add_transition(STATE_27, (transition[]){create_transition('=', STATE_28), create_transition('-', STATE_29)}, 2);
    theta(STATE_27, STATE_32);
    add_transition(STATE_29, (transition[]){create_transition('-', STATE_30)}, 1);
    theta(STATE_29, STATE_33);
    add_transition(STATE_30, (transition[]){create_transition('-', STATE_31)}, 1);
    add_transition(STATE_48, AToZ(STATE_49), 52);
    add_transition(STATE_49, AToZ(STATE_49), 52);
    add_transition(STATE_49, zeroToNine(STATE_50), 10);
    theta(STATE_49, STATE_51);
    add_transition(STATE_50, zeroToNine(STATE_50), 10);
    theta(STATE_50, STATE_51);
    add_transition(STATE_53, aToZ(STATE_53), 26);
    theta(STATE_53, STATE_54);
    add_transition(STATE_52, aToZ(STATE_53), 26);
    add_transition(STATE_52, twoToSeven(STATE_55), 6);
    add_transition(STATE_55, bToD(STATE_55), 3);
    add_transition(STATE_55, twoToSeven(STATE_56), 6);
    theta(STATE_55, STATE_57);
    add_transition(STATE_56, twoToSeven(STATE_56), 6);
    theta(STATE_56, STATE_57);
    add_transition(STATE_37, (transition[]){create_transition('.', STATE_39)}, 1);
    add_transition(STATE_37, zeroToNine(STATE_37), 10);
    theta(STATE_37, STATE_38);
    add_transition(STATE_39, zeroToNine(STATE_41), 10);
    theta(STATE_39, STATE_40);
    add_transition(STATE_41, zeroToNine(STATE_42), 10);
    add_transition(STATE_42, (transition[]){create_transition('E', STATE_44)}, 1);
    theta(STATE_42, STATE_43);
    add_transition(STATE_44, (transition[]){create_transition('+', STATE_45), create_transition('-', STATE_45)}, 2);
    add_transition(STATE_44, zeroToNine(STATE_46), 10);
    add_transition(STATE_46, zeroToNine(STATE_47), 10);
    add_transition(STATE_45, zeroToNine(STATE_46), 10);
    theta(STATE_52, STATE_58);
}

// Initialize the lookup table with language keywords
void setupLookupTable()
{
    look_up_table = getTrieNode();
    
    // Control flow keywords
    insert(look_up_table, "if", TK_IF);
    insert(look_up_table, "else", TK_ELSE);
    insert(look_up_table, "endif", TK_ENDIF);
    insert(look_up_table, "while", TK_WHILE);
    insert(look_up_table, "endwhile", TK_ENDWHILE);

    // Function and parameter keywords
    insert(look_up_table, "_main", TK_MAIN);
    insert(look_up_table, "call", TK_CALL);
    insert(look_up_table, "with", TK_WITH);
    insert(look_up_table, "parameters", TK_PARAMETERS);
    insert(look_up_table, "parameter", TK_PARAMETER);
    insert(look_up_table, "input", TK_INPUT);
    insert(look_up_table, "output", TK_OUTPUT);
    insert(look_up_table, "global", TK_GLOBAL);
    insert(look_up_table, "return", TK_RETURN);

    // Type-related keywords
    insert(look_up_table, "type", TK_TYPE);
    insert(look_up_table, "int", TK_INT);
    insert(look_up_table, "real", TK_REAL);
    insert(look_up_table, "list", TK_LIST);
    insert(look_up_table, "record", TK_RECORD);
    insert(look_up_table, "endrecord", TK_ENDRECORD);
    insert(look_up_table, "union", TK_UNION);
    insert(look_up_table, "endunion", TK_ENDUNION);
    insert(look_up_table, "definetype", TK_DEFINETYPE);
    insert(look_up_table, "as", TK_AS);

    // I/O keywords
    insert(look_up_table, "read", TK_READ);
    insert(look_up_table, "write", TK_WRITE);

    // Block keywords
    insert(look_up_table, "end", TK_END);
    insert(look_up_table, "then", TK_THEN);
}

// Returns the Lexical Token List of the file
vector getStream(FILE *fp)
{
    if (fp == NULL)
    {
        printf("Error: File not found\n");
        exit(1);
    }
    dualBuffer buffer = (dualBuffer)malloc(sizeof(struct DualBuffer));
    buffer->secondary_buffer_index = 0;
    buffer->line_count = 1;
    FILE *new_fp = fopen("lexical_tokens.salad", "w");
    if (new_fp == NULL)
    {
        printf("Error: Could not create a new file\n");
        exit(1);
    }
    fprintf(new_fp, "%-30s %-30s %s\n", "Line", "Lexeme", "Token");
    fprintf(new_fp, "%-30s %-30s %s\n", "----", "------", "-----");

    vector v = init_vector();
    while (fgets(buffer->primary_buffer, MAX_BUFFER_SIZE, fp) != NULL)
    {
        tokenInfo info = getNextToken(buffer);
        for (int i = 0; i < info->token_count; i++)
        {
            push_back(v, info->tokens[i]);
            fprintf(new_fp, "%-30d %-30s %s\n", info->tokens[i]->lc, info->tokens[i]->lexeme, TOKENS[info->tokens[i]->tk]);
        }
        free(info);
    }
    fclose(fp);
    fclose(new_fp);
    free(buffer);
    return v;
}

// Returns the list of next tokens from the dual-buffer
tokenInfo getNextToken(dualBuffer buffer)
{
    state curr_state = &states[START];
    state prev_state = NULL;
    buffer->primary_buffer_index = 0;

    tokenInfo tokens = (tokenInfo)malloc(sizeof(TokenInfo));
    tokens->tokens = (token *)malloc(MAX_BUFFER_SIZE * sizeof(token));
    tokens->token_count = 0;

    char *keyword = (char *)malloc(100 * sizeof(char));

    char invalid_token[30];

    int till = 0;

    int is_filled = buffer->secondary_buffer_index > 0;

    int token_len = 0;
    int token_count = 0;
    int next = 1;
    int err_type = 0;

    while (till < buffer->secondary_buffer_index)
    {

        curr_state = get_next_state(curr_state, buffer->secondary_buffer[till]);
        token_len++;
        if (buffer->secondary_buffer[till] == '\n' && curr_state->retract_count == 0)
        {
            buffer->line_count++;
        }
        if (curr_state->state_id == TK_FUNID && token_len > 30)
        {
            err_type = 3;
            curr_state = &states[INVALID];
        }
        else if (curr_state->token == TK_ID && token_len > 20)
        {
            err_type = 4;
            curr_state = &states[INVALID];
        }
        if (curr_state->token == TK_INVALID)
        {
            int next = (token_len == 1);
            till -= curr_state->retract_count;
            token_len -= curr_state->retract_count;
            memset(keyword, '\0', 100);
            memset(invalid_token, '\0', 30);
            if (err_type == 3)
            {
                strncpy(keyword, "Function Length Error", 22);
            }
            else if (err_type == 4)
            {
                strncpy(keyword, "Variable Length Error", 22);
            }
            else if (next)
            {
                strncpy(invalid_token, buffer->secondary_buffer + till - token_len + 1, token_len + next);
                sprintf(keyword, "Unknown Symbol <%s>", invalid_token);
            }
            else
            {
                strncpy(invalid_token, buffer->secondary_buffer + till - token_len + 1, token_len + next);
                if (token_len == 1)
                {
                    sprintf(keyword, "Unknown Symbol <%s>", invalid_token);
                }
                else
                {
                    sprintf(keyword, "Unknown Pattern <%s>", invalid_token);
                }
            }
            tokens->tokens[tokens->token_count] = create_token(curr_state->token, buffer->line_count, keyword);
            tokens->token_count++;
            token_len = 0;
            curr_state = &states[START];
            continue;
        }
        till -= curr_state->retract_count;
        token_len -= curr_state->retract_count;
        if (curr_state->token != -1)
        {
            memset(keyword, '\0', 100);
            if (curr_state->state_id == STATE_60){
                keyword[0] = '%';
            }
            else{
                strncpy(keyword, buffer->secondary_buffer + till - token_len + 1, token_len);
            }
            if (curr_state->state_id == STATE_51 || curr_state->state_id == STATE_54)
            {
                token_id tk = search(look_up_table, keyword);
                if (tk == TK_INVALID)
                {
                    if (keyword[0] == '_')
                    {
                        tk = TK_FUNID;
                    }
                    else
                    {
                        tk = TK_FIELDID;
                    }
                }
                tokens->tokens[tokens->token_count] = create_token(tk, buffer->line_count, keyword);
                tokens->token_count++;
            }
            if (curr_state->token != TK_NOTOKEN)
            {
                int lc = (curr_state->state_id == STATE_60) ? buffer->line_count - 1 : buffer->line_count;
                tokens->tokens[tokens->token_count] = create_token(curr_state->token, lc, keyword);
                tokens->token_count++;
            }
            curr_state = &states[START];
            token_len = 0;
        }
        till++;
        prev_state = curr_state;
    }
    // read from primary buffer
    while (buffer->primary_buffer_index < MAX_BUFFER_SIZE && buffer->primary_buffer[buffer->primary_buffer_index] != '\0')
    {

        curr_state = get_next_state(curr_state, buffer->primary_buffer[buffer->primary_buffer_index]);
        token_len++;
        if (buffer->primary_buffer[buffer->primary_buffer_index] == '\n' && curr_state->retract_count == 0)
        {
            buffer->line_count++;
        }
        if (curr_state->state_id == STATE_51 && token_len > 30)
        {

            err_type = 3;
            curr_state = &states[INVALID];
        }
        else if (curr_state->token == TK_ID && token_len > 20)
        {
            err_type = 4;
            curr_state = &states[INVALID];
        }
        if (curr_state->token == TK_INVALID)
        {
            int next = (token_len == 1);
            buffer->primary_buffer_index -= curr_state->retract_count;
            token_len -= curr_state->retract_count;
            memset(keyword, '\0', 100);
            memset(invalid_token, '\0', 30);
            if (err_type == 3)
            {
                strncpy(keyword, "Function Length Error", 22);
            }
            else if (err_type == 4)
            {
                strncpy(keyword, "Variable Length Error", 22);
            }
            else if (next)
            {
                strncpy(invalid_token, buffer->primary_buffer + buffer->primary_buffer_index - token_len + 1, token_len + next);
                sprintf(keyword, "Unknown Symbol <%s>", invalid_token);
            }
            else
            {
                strncpy(invalid_token, buffer->primary_buffer + buffer->primary_buffer_index - token_len + 1, token_len + next);
                if (token_len == 1)
                {
                    sprintf(keyword, "Unknown Symbol <%s>", invalid_token);
                }
                else
                {
                    sprintf(keyword, "Unknown Pattern <%s>", invalid_token);
                }
            }
            tokens->tokens[tokens->token_count] = create_token(curr_state->token, buffer->line_count, keyword);
            tokens->token_count++;
            buffer->primary_buffer_index++;
            buffer->primary_buffer_index += next;
            err_type = 0;
            token_len = 0;
            curr_state = &states[START];
            continue;
        }
        // s -> 1 -> 2 -> 3
        buffer->primary_buffer_index -= curr_state->retract_count;
        token_len -= curr_state->retract_count;
        if (curr_state->token != -1)
        {

            memset(keyword, '\0', 100);
            // comment
            if (curr_state->state_id == STATE_60){
                keyword[0] = '%';   
            }
            else {
                if (token_len > buffer->primary_buffer_index + 1)
                {
                    strncpy(keyword, buffer->secondary_buffer, token_len - buffer->primary_buffer_index - 1);
                    strncpy(keyword + token_len - buffer->primary_buffer_index - 1, buffer->primary_buffer, buffer->primary_buffer_index + 1);
                }
                else
                {
                    strncpy(keyword, buffer->primary_buffer + buffer->primary_buffer_index - token_len + 1, token_len);
                }
            }
            if (curr_state->state_id == STATE_51 || curr_state->state_id == STATE_54)
            {
                token_id tk = search(look_up_table, keyword);
                if (tk == TK_INVALID)
                {
                    if (keyword[0] == '_')
                    {
                        tk = TK_FUNID;
                    }
                    else
                    {
                        tk = TK_FIELDID;
                    }
                }
                tokens->tokens[tokens->token_count] = create_token(tk, buffer->line_count, keyword);
                tokens->token_count++;
            }
            if (curr_state->token != TK_NOTOKEN)
            {   
                int lc = (curr_state->state_id == STATE_60) ? buffer->line_count - 1 : buffer->line_count;
                tokens->tokens[tokens->token_count] = create_token(curr_state->token, lc, keyword);
                tokens->token_count++;
            }
            curr_state = &states[START];
            token_len = 0;
        }
        buffer->primary_buffer_index++;
    }

    if (is_filled)
    {
        memset(buffer->secondary_buffer, '\0', MAX_BUFFER_SIZE);
    }

    if (curr_state == &states[STATE_59])
    {
        strncpy(buffer->secondary_buffer, "%", 1);
        buffer->secondary_buffer_index = 1;
    }
    else if (curr_state != &states[START])
    {
        strncpy(buffer->secondary_buffer, buffer->primary_buffer + MAX_BUFFER_SIZE - 1 - token_len, token_len + (token_len == 0));
        buffer->secondary_buffer_index = token_len;
    }
    else
    {
        buffer->secondary_buffer_index = 0;
    }
    free(keyword);
    return tokens;
}

// Return Next State of the DFA given a Character
state get_next_state(state current_state, char next_char)
{
    bool flag = false;
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (ALPHABETS[i] == next_char)
        {
            flag = true;
            break;
        }
    }
    if (!flag)
    {
        return &states[INVALID];
    }
    for (int i = 0; i < current_state->length; i++)
    {
        if (current_state->transitions[i]->next_char == next_char)
        {
            return current_state->transitions[i]->next_state;
        }
    }
    return &states[INVALID];
}

// Removes the comments from the test-case file and puts remaining input
// into a clean file
void removeComments(char *testcaseFile, char *cleanFile)
{
    FILE *testcase_fp = fopen(testcaseFile, "r");
    FILE *clean_fp = fopen(cleanFile, "w");
    if (testcase_fp == NULL)
    {
        printf("Error: The Test Case File Input was invalid, File not found\n");
        exit(1);
    }
    if (clean_fp == NULL)
    {
        printf("Error: Could not create a new file\n");
        exit(1);
    }

    char interm = fgetc(testcase_fp);

    int comment = 0;

    while (interm != EOF)
    {
        if (interm == '%')
        {
            comment = 1;
        }
        if (comment == 1)
        {
            if (interm == '\n')
            {
                comment = 0;
            }
        }
        else
        {
            fputc(interm, clean_fp);
        }
        interm = fgetc(testcase_fp);
    }

    fclose(testcase_fp);
    fclose(clean_fp);
}
