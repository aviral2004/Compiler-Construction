/*
Group Number - 10
ID:	2021A7PS1463P			Name: Dhyey Italiya
ID:	2021A7PS2434P			Name: LAKSHIT SETHI
ID:	2021A7PS0523P			Name: Abir Abhyankar
ID:	2021A7PS2414P			Name: Saksham Verma
ID:	2021A7PS2412P			Name: Saurabh Bhandari
*/

#include "lexerDef.h"

static State states[MAX_STATES];
static trie look_up_table;

// create a new transition
transition f(char ch, state_id next_state)
{
    transition t = (transition)malloc(sizeof(Transition));
    t->next_state = &states[next_state];
    t->next_char = ch;
    return t;
}

// create a transition for a to z except b to d
transition *aToZExceptBToD(state_id next_state)
{
    int size = 23;
    transition *transitions = (transition *)malloc(size * sizeof(transition));
    int j = 0;
    for (int i = 'a'; i <= 'z'; i++)
    {
        if (i != 'b' && i != 'c' && i != 'd')
        {
            transitions[j++] = f(i, next_state);
        }
    }
    return transitions;
}

// create a transition for a to z
transition *aToZ(state_id next_state)
{
    int size = 26;
    transition *transitions = (transition *)malloc(size * sizeof(transition));
    int j = 0;
    for (int i = 'a'; i <= 'z'; i++)
    {
        transitions[j++] = f(i, next_state);
    }
    return transitions;
}

// create a transition for A to Z
transition *AToZ(state_id next_state)
{
    int size = 52;
    transition *transitions = (transition *)malloc(size * sizeof(transition));
    int j = 0;
    for (int i = 'a'; i <= 'z'; i++)
    {
        transitions[j++] = f(i, next_state);
    }
    for (int i = 'A'; i <= 'Z'; i++)
    {
        transitions[j++] = f(i, next_state);
    }
    return transitions;
}

// create a transition for b to d
transition *bToD(state_id next_state)
{
    int size = 3;
    transition *transitions = (transition *)malloc(size * sizeof(transition));
    int j = 0;
    for (int i = 'b'; i <= 'd'; i++)
    {
        transitions[j++] = f(i, next_state);
    }
    return transitions;
}

// create a transition for 0 to 9
transition *zeroToNine(state_id next_state)
{
    int size = 10;
    transition *transitions = (transition *)malloc(size * sizeof(transition));
    int j = 0;
    for (int i = 0; i < 10; i++)
    {
        transitions[j++] = f(i + '0', next_state);
    }
    return transitions;
}

// create a transition for 2 to 7
transition *twoToSeven(state_id next_state)
{
    int size = 6;
    transition *transitions = (transition *)malloc(size * sizeof(transition));
    int j = 0;
    for (int i = 0; i < 6; i++)
    {
        transitions[j++] = f(i + '2', next_state);
    }
    return transitions;
}

// Add remaining transitions to the graph
void theta(state_id curr_state, state_id next_state)
{
    int size = ALPHABET_SIZE - states[curr_state].length;
    transition *transitions = (transition *)malloc(size * sizeof(transition));
    int count = 0;
    int *freq = (int *)calloc(128, sizeof(int));
    for (int j = 0; j < states[curr_state].length; j++)
    {
        freq[states[curr_state].transitions[j]->next_char] = 1;
    }
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (freq[ALPHABETS[i]] != 1)
        {
            transitions[count++] = f(ALPHABETS[i], next_state);
        }
    }
    add_transition(curr_state, transitions, size);
}


// Returns a New Token with a given token_id, line count and lexeme
token getNewToken(token_id id, int lc, char *lexeme)
{
    token newToken = (token)malloc(sizeof(Token));
    newToken->tk = id;
    newToken->lc = lc;
    newToken->lexeme = (char *)(malloc(30 * sizeof(char)));
    memset(newToken->lexeme, '\0', 30);
    strncpy(newToken->lexeme, lexeme, strlen(lexeme));
    return newToken;
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

// Initializes the states of the DFA
void initialize_states()
{
    for (int i = 0; i < MAX_STATES; i++)
    {
        states[i].length = 0;
        states[i].state_id = i;
        states[i].retract_count = 0;
        states[i].token = -1;
        states[i].transitions = NULL;
    }
    states[INVALID].token = TK_INVALID;
    states[RAND_60].token = TK_COMMENT;
    states[RAND_61].token = TK_NOTOKEN;
    states[RAND_62].token = TK_NOTOKEN;
    states[RAND_51].token = TK_NOTOKEN;
    states[RAND_54].token = TK_NOTOKEN;
    states[RAND_61].token = TK_NOTOKEN;
    states[RAND_19].token = TK_NOT;
    states[RAND_1].token = TK_SQL;
    states[RAND_2].token = TK_SQR;
    states[RAND_3].token = TK_COMMA;
    states[RAND_4].token = TK_SEM;
    states[RAND_5].token = TK_COLON;
    states[RAND_6].token = TK_DOT;
    states[RAND_7].token = TK_OP;
    states[RAND_8].token = TK_CL;
    states[RAND_21].token = TK_EQ;
    states[RAND_9].token = TK_PLUS;
    states[RAND_10].token = TK_MINUS;
    states[RAND_23].token = TK_NE;
    states[RAND_18].token = TK_OR;
    states[RAND_15].token = TK_AND;
    states[RAND_36].token = TK_RUID;
    states[RAND_11].token = TK_MUL;
    states[RAND_26].token = TK_GT;
    states[RAND_25].token = TK_GE;
    states[RAND_32].token = TK_LT;
    states[RAND_28].token = TK_LE;
    states[RAND_33].token = TK_LT;
    states[RAND_31].token = TK_ASSIGNOP;
    states[RAND_12].token = TK_DIV;
    states[RAND_57].token = TK_ID;
    states[RAND_38].token = TK_NUM;
    states[RAND_43].token = TK_RNUM;
    states[RAND_47].token = TK_RNUM;
    states[RAND_40].token = TK_NUM;
    states[RAND_58].token = TK_FIELDID;
    states[RAND_36].retract_count = 1;
    states[RAND_26].retract_count = 1;
    states[RAND_32].retract_count = 1;
    states[RAND_33].retract_count = 2;
    states[RAND_51].retract_count = 1;
    states[RAND_54].retract_count = 1;
    states[RAND_57].retract_count = 1;
    states[RAND_38].retract_count = 1;
    states[RAND_43].retract_count = 1;
    states[RAND_40].retract_count = 2;
    states[RAND_58].retract_count = 1;
    states[INVALID].retract_count = 1;
}

// Initializes the transitions of the DFA
void initialize_transitions()
{
    add_transition(START, (transition[]){f('%', RAND_59), f(' ', RAND_61), f('\n', RAND_62), f('~', RAND_19), f('[', RAND_1), f(']', RAND_2), f(',', RAND_3), f(';', RAND_4), f(':', RAND_5), f('.', RAND_6), f('(', RAND_7), f(')', RAND_8), f('=', RAND_20), f('+', RAND_9), f('-', RAND_10), f('!', RAND_22), f('@', RAND_16), f('&', RAND_13), f('#', RAND_34), f('*', RAND_11), f('>', RAND_24), f('<', RAND_27), f('/', RAND_12), f('_', RAND_48), f('\t', RAND_61)}, 25);
    add_transition(START, bToD(RAND_52), 3);
    add_transition(START, zeroToNine(RAND_37), 10);
    add_transition(START, aToZExceptBToD(RAND_53), 23);
    add_transition(RAND_59, (transition[]){f('\n', RAND_60)}, 1);
    theta(RAND_59, RAND_59);
    add_transition(RAND_20, (transition[]){f('=', RAND_21)}, 1);
    add_transition(RAND_22, (transition[]){f('=', RAND_23)}, 1);
    add_transition(RAND_16, (transition[]){f('@', RAND_17)}, 1);
    add_transition(RAND_17, (transition[]){f('@', RAND_18)}, 1);
    add_transition(RAND_13, (transition[]){f('&', RAND_14)}, 1);
    add_transition(RAND_14, (transition[]){f('&', RAND_15)}, 1);
    add_transition(RAND_34, aToZ(RAND_35), 26);
    add_transition(RAND_35, aToZ(RAND_35), 26);
    theta(RAND_35, RAND_36);
    add_transition(RAND_24, (transition[]){f('=', RAND_25)}, 1);
    theta(RAND_24, RAND_26);
    add_transition(RAND_27, (transition[]){f('=', RAND_28), f('-', RAND_29)}, 2);
    theta(RAND_27, RAND_32);
    add_transition(RAND_29, (transition[]){f('-', RAND_30)}, 1);
    theta(RAND_29, RAND_33);
    add_transition(RAND_30, (transition[]){f('-', RAND_31)}, 1);
    add_transition(RAND_48, AToZ(RAND_49), 52);
    add_transition(RAND_49, AToZ(RAND_49), 52);
    add_transition(RAND_49, zeroToNine(RAND_50), 10);
    theta(RAND_49, RAND_51);
    add_transition(RAND_50, zeroToNine(RAND_50), 10);
    theta(RAND_50, RAND_51);
    add_transition(RAND_53, aToZ(RAND_53), 26);
    theta(RAND_53, RAND_54);
    add_transition(RAND_52, aToZ(RAND_53), 26);
    add_transition(RAND_52, twoToSeven(RAND_55), 6);
    add_transition(RAND_55, bToD(RAND_55), 3);
    add_transition(RAND_55, twoToSeven(RAND_56), 6);
    theta(RAND_55, RAND_57);
    add_transition(RAND_56, twoToSeven(RAND_56), 6);
    theta(RAND_56, RAND_57);
    add_transition(RAND_37, (transition[]){f('.', RAND_39)}, 1);
    add_transition(RAND_37, zeroToNine(RAND_37), 10);
    theta(RAND_37, RAND_38);
    add_transition(RAND_39, zeroToNine(RAND_41), 10);
    theta(RAND_39, RAND_40);
    add_transition(RAND_41, zeroToNine(RAND_42), 10);
    add_transition(RAND_42, (transition[]){f('E', RAND_44)}, 1);
    theta(RAND_42, RAND_43);
    add_transition(RAND_44, (transition[]){f('+', RAND_45), f('-', RAND_45)}, 2);
    add_transition(RAND_44, zeroToNine(RAND_46), 10);
    add_transition(RAND_46, zeroToNine(RAND_47), 10);
    add_transition(RAND_45, zeroToNine(RAND_46), 10);
    theta(RAND_52, RAND_58);
}

// Initializes the lookup table with the keywords and their respective tokens
void initialize_lookup_table()
{
    look_up_table = getTrieNode();
    insert(look_up_table, "with", TK_WITH);
    insert(look_up_table, "parameters", TK_PARAMETERS);
    insert(look_up_table, "end", TK_END);
    insert(look_up_table, "while", TK_WHILE);
    insert(look_up_table, "union", TK_UNION);
    insert(look_up_table, "endunion", TK_ENDUNION);
    insert(look_up_table, "definetype", TK_DEFINETYPE);
    insert(look_up_table, "as", TK_AS);
    insert(look_up_table, "type", TK_TYPE);
    insert(look_up_table, "_main", TK_MAIN);
    insert(look_up_table, "global", TK_GLOBAL);
    insert(look_up_table, "parameter", TK_PARAMETER);
    insert(look_up_table, "list", TK_LIST);
    insert(look_up_table, "input", TK_INPUT);
    insert(look_up_table, "output", TK_OUTPUT);
    insert(look_up_table, "int", TK_INT);
    insert(look_up_table, "real", TK_REAL);
    insert(look_up_table, "endwhile", TK_ENDWHILE);
    insert(look_up_table, "if", TK_IF);
    insert(look_up_table, "then", TK_THEN);
    insert(look_up_table, "endif", TK_ENDIF);
    insert(look_up_table, "read", TK_READ);
    insert(look_up_table, "write", TK_WRITE);
    insert(look_up_table, "return", TK_RETURN);
    insert(look_up_table, "call", TK_CALL);
    insert(look_up_table, "record", TK_RECORD);
    insert(look_up_table, "endrecord", TK_ENDRECORD);
    insert(look_up_table, "else", TK_ELSE);
}

// Returns the Lexical Token List of the file
vector getStream(FILE *fp)
{
    if (fp == NULL)
    {
        printf("Error: File not found\n");
        exit(1);
    }
    twinBuffer buffer = (twinBuffer)malloc(sizeof(struct TwinBuffer));
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

// Returns the list of next tokens from the twin-buffer
tokenInfo getNextToken(twinBuffer buffer)
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
        if (curr_state->token == RAND_51 && token_len > 30)
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
            tokens->tokens[tokens->token_count] = getNewToken(curr_state->token, buffer->line_count, keyword);
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
            if (curr_state->state_id == RAND_60){
                keyword[0] = '%';
            }
            else{
                strncpy(keyword, buffer->secondary_buffer + till - token_len + 1, token_len);
            }
            if (curr_state->state_id == RAND_51 || curr_state->state_id == RAND_54)
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
                tokens->tokens[tokens->token_count] = getNewToken(tk, buffer->line_count, keyword);
                tokens->token_count++;
            }
            if (curr_state->token != TK_NOTOKEN)
            {
                int lc = (curr_state->state_id == RAND_60) ? buffer->line_count - 1 : buffer->line_count;
                tokens->tokens[tokens->token_count] = getNewToken(curr_state->token, lc, keyword);
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
        if (curr_state->token == RAND_51 && token_len > 30)
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
            tokens->tokens[tokens->token_count] = getNewToken(curr_state->token, buffer->line_count, keyword);
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
            if (curr_state->state_id == RAND_60){
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
            if (curr_state->state_id == RAND_51 || curr_state->state_id == RAND_54)
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
                tokens->tokens[tokens->token_count] = getNewToken(tk, buffer->line_count, keyword);
                tokens->token_count++;
            }
            if (curr_state->token != TK_NOTOKEN)
            {   
                int lc = (curr_state->state_id == RAND_60) ? buffer->line_count - 1 : buffer->line_count;
                tokens->tokens[tokens->token_count] = getNewToken(curr_state->token, lc, keyword);
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

    if (curr_state == &states[RAND_59])
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
        printf("Error: Test Case File not found\n");
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
