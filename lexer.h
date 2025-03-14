/*
Group Number - 17
ID:	2022A7PS0173P			Name: Ameesh Sethi
ID:	2022A7PS0097P			Name: Aviral Gupta
ID:	2022A7PS0146P			Name: Yatharth Singh
ID:	2022A7PS1188P			Name: Armaan Sethi
ID:	2022A7PS0052P			Name: Utkarsh Tiwari
*/

#ifndef LEXER_H
#define LEXER_H
#include "utils.h"

void theta(state_id curr_state, state_id next_state); // Add remaining transitions to the graph
transition f(char ch, state_id next_state);           // create a transition
transition *aToZExceptBToD(state_id next_state);      // create a transition for a to z except b to d
transition *aToZ(state_id next_state);                // create a transition for a to z
transition *AToZ(state_id next_state);                // create a transition for A to Z
transition *bToD(state_id next_state);                // create a transition for b to d
transition *zeroToNine(state_id next_state);          // create a transition for 0 to 9
transition *twoToSeven(state_id next_state);          // create a transition for 2 to 7

void add_state(state_id state_num, transition *transitions, int transition_count); // Add a state to the graph

void initialize_states(); // Initialize the states of the graph

void initialize_transitions(); // Initialize the transitions of the graph

void initialize_lookup_table(); // Initialize the lookup table

state get_next_state(state current_state, char next_char); // Get the next state from the current state and the next character

void add_transition(state_id state, transition *transitions, int transition_count); // Add a transition to the graph

tokenInfo getNextToken(twinBuffer buffer); // Get the next token from the buffer

void removeComments(char *testcaseFile, char *cleanFile);

#ifndef VECTOR_INCOMPLETE
#include "vector.h"
vector getStream(FILE *fp);
#endif
#endif