/* Program to generate text based on the context provided by input prompts.

  Skeleton program written by Artem Polyvyanyy, http://polyvyanyy.com/,
  September 2023, with the intention that it be modified by students
  to add functionality, as required by the assignment specification.
  All included code is (c) Copyright University of Melbourne, 2023.

  Student Authorship Declaration:

  (1) I certify that except for the code provided in the initial skeleton file,
  the program contained in this submission is completely my own individual
  work, except where explicitly noted by further comments that provide details
  otherwise. I understand that work that has been developed by another student,
  or by me in collaboration with other students, or by non-students as a result
  of request, solicitation, or payment, may not be submitted for assessment in
  this subject. I understand that submitting for assessment work developed by
  or in collaboration with other students or non-students constitutes Academic
  Misconduct, and may be penalized by mark deductions, or by other penalties
  determined via the University of Melbourne Academic Honesty Policy, as
  described at https://academicintegrity.unimelb.edu.au.

  (2) I also certify that I have not provided a copy of this work in either
  softcopy or hardcopy or any other form to any other student, and nor will I
  do so until after the marks are released. I understand that providing my work
  to other students, regardless of my intention or any undertakings made to me
  by that other student, is also Academic Misconduct.

  (3) I further understand that providing a copy of the assignment specification
  to any form of code authoring or assignment tutoring service, or drawing the
  attention of others to such services and code that may have been made
  available via such a service, may be regarded as Student General Misconduct
  (interfering with the teaching activities of the University and/or inciting
  others to commit Academic Misconduct). I understand that an allegation of
  Student General Misconduct may arise regardless of whether or not I personally
  make use of such solutions or sought benefit from such actions.

  Signed by: Junchen Wei 1399443
  Dated:     28/9/2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

/* #DEFINE'S -----------------------------------------------------------------*/
#define SDELIM "==STAGE %d============================\n"   // stage delimiter
#define MDELIM "-------------------------------------\n"    // delimiter of -'s
#define THEEND "==THE END============================\n"    // end message
#define NOSFMT "Number of statements: %d\n"                 // no. of statements
#define NOCFMT "Number of characters: %d\n"                 // no. of chars
#define NPSFMT "Number of states: %d\n"                     // no. of states
#define TFQFMT "Total frequency: %d\n"                      // total frequency

#define CRTRNC '\r'                             // carriage return character
#define NEW_LINE '\n'
#define ELLIPSE '.'

#define TRUE  1
#define FALSE 0
#define ZERO  0
#define ONE   1
#define TWO   2
#define CHAR_STR_SIZE 1+1  // the size of a string of 1 char
#define STR_END '\0'
#define EQUAL 0
#define MAX_CHAR 37
#define ELLIPSE_SIZE 3

/* TYPE DEFINITIONS ----------------------------------------------------------*/
typedef struct state state_t;   // a state in an automaton
typedef struct node  node_t;    // a node in a linked list

struct node {                   // a node in a linked list of transitions has
    char*           str;        // ... a transition string
    state_t*        state;      // ... the state reached via the string, and
    node_t*         next;       // ... a link to the next node in the list.
};

typedef struct {                // a linked list consists of
    node_t*         head;       // ... a pointer to the first node and
    node_t*         tail;       // ... a pointer to the last node in the list.
} list_t;

struct state {                  // a state in an automaton is characterized by
    unsigned int    id;         // ... an identifier,
    unsigned int    freq;       // ... frequency of traversal,
    int             visited;    // ... visited status flag, and
    list_t*         outputs;    // ... a list of output states.
};

typedef struct {                // an automaton consists of
    state_t*        ini;        // ... the initial state, and
    unsigned int    nid;        // ... the identifier of the next new state.
} automaton_t;

/* USEFUL FUNCTIONS ----------------------------------------------------------*/
int mygetchar(void);            // getchar() that skips carriage returns

automaton_t* initialise_automaton();
list_t* make_empty_list(void);
int is_empty_list(list_t* list);
void do_stage_zero(automaton_t* automaton);
state_t* create_new_state(automaton_t* automaton);
list_t* insert_new_node(list_t* list, char* char_str, state_t* next_state);
char* create_char_str(char c);
state_t* search_char(char c, state_t* curr_state, int index);
node_t* find_next(node_t* list);
void print_stage_zero(automaton_t* automaton, int char_count);
void do_stage_one(automaton_t* automaton);
void print_header(int stage_num);
void process_prompts(automaton_t* automaton);
char* get_str(char* str, int limit);
void print_ellipses(int* count);
node_t* select_node(state_t* track_state);
void generate_chars(int* count, state_t* track_state);
int is_emptystr(char* str);
state_t* track_input(state_t* track_state, char* str, int* count, int index);
void do_stage_two(automaton_t* automaton);
void recursive_compress(state_t* curr_state, int* counter, int limit, int* freq);
int has_single_output(state_t* curr_state); 
char* combine_strs(char* prefix, char* string);
int is_all_visited(state_t* curr_state);
state_t* select_state(state_t* curr_state);
void print_stage_two(int freq, int num_states);
void free_automaton(automaton_t* automaton);
void recursive_free(state_t* state);

/* WHERE IT ALL HAPPENS ------------------------------------------------------*/
int main(int argc, char *argv[]) {
    // Message from Artem: The proposed in this skeleton file #define's,
    // typedef's, and struct's are the subsets of those from my sample solution
    // to this assignment. You can decide to use them in your program, or if
    // you find them confusing, you can remove them and implement your solution
    // from scratch. I will share my sample solution with you at the end of
    // the subject.
    automaton_t* automaton = initialise_automaton();
    do_stage_zero(automaton);
    do_stage_one(automaton);
    do_stage_two(automaton);
    free_automaton(automaton);
    return EXIT_SUCCESS;        // algorithms are fun!!!
}

/* USEFUL FUNCTIONS ----------------------------------------------------------*/

// An improved version of getchar(); skips carriage return characters.
// NB: Adapted version of the mygetchar() function by Alistair Moffat
int mygetchar() {
    int c;
    while ((c=getchar())==CRTRNC);
    return c;
}

/* initialise the automaton tree with an empty first state 
*/
automaton_t* 
initialise_automaton() {
    automaton_t* automaton;
    automaton = malloc(sizeof(*automaton));
    assert(automaton != NULL);
    automaton->nid = 0;  /* id of the next state*/
    /* create the initial state */
    state_t* initial_state = create_new_state(automaton);
    /* assign the initial state to the 'ini' of automaton */
    automaton->ini = initial_state;
    return automaton;
}

/* create a new state in the tree with initialised info 
*/
state_t* 
create_new_state(automaton_t* automaton) {
    state_t* new_state;
    new_state = (state_t*) malloc(sizeof(*new_state));
    assert(automaton != NULL && new_state != NULL);
    new_state->id = automaton->nid;  /* get the new id */
    automaton->nid++;  /* increment the id for the next new state*/
    new_state->freq = 0;  /* no strings have departed here yet */
    new_state->visited = FALSE;  /* initialise it to FALSE for stage 2 */
    new_state->outputs = make_empty_list();
    return new_state;
}

/* make an empty linked list ;
The source of the code for the function is from 'Programming, Problem Solving,
And Abstraction' by Alistair Moffat (the original author), P172, Figure 10.4 
*/
list_t* 
make_empty_list(void) {
    list_t* list;
    list = (list_t*) malloc(sizeof(*list));
    assert(list != NULL);
    list->head = list->tail = NULL;
    return list;
}

/* check is the linked list is empty
*/
int
is_empty_list(list_t *list) {
    assert(list != NULL);
    return (list->head == NULL && list->tail == NULL);
}

/* implement the stage zero by creating states corresponding single characters
 and link them together */
void  
do_stage_zero(automaton_t* automaton) {
    print_header(ZERO);
    int char_count = 0, new_line = FALSE, c;
    char* char_str;
    state_t* track_state = automaton->ini, *new_state, *next_state;
    while (!((c = mygetchar()) == NEW_LINE && new_line == TRUE)) {
        if (c == NEW_LINE) {  /* new input string is entered */
            track_state = automaton->ini;  /* go back to the initial state */
            new_line = TRUE;
        } else {
            new_line = FALSE;
            char_count++;
            track_state->freq++;
            /* search c in the 'output' of the current state to 
            determine if a new branch is needed */
            if((next_state = search_char(c, track_state, 0)) == NULL) {
                /* no match found, create a new state connected by a new node */
                char_str = create_char_str(c);
                new_state = create_new_state(automaton);
                track_state->outputs = insert_new_node(track_state->outputs, 
                char_str, new_state);
                track_state = new_state;
            } else {
                // found a matched pattern
                track_state = next_state;
            }
        }
    }
    print_stage_zero(automaton, char_count);
    return;
}

/* convert the single char to string 
*/
char*
create_char_str(char c) {
    char* char_str = (char*) malloc(sizeof(char) * CHAR_STR_SIZE);
    assert(char_str != NULL);
    char_str[0] = c;
    char_str[1] = STR_END;  // termination of the string
    return char_str;
}

/* Some of the code for the function is from 'Programming, Problem 
Solving, And Abstraction' by Alistair Moffat (the original author), P173, 
Figure 10.5 */
list_t*
insert_new_node(list_t* list, char* char_str, state_t* next_state) {
    node_t *new;
    new = (node_t*)malloc(sizeof(*new));
    assert(list != NULL && new != NULL);
    new->str = char_str;
    new->state = next_state;
    new->next = NULL;  // the existence of the next node is unknown
    if(list->tail == NULL) {
        /* this is the first insertion into the list */
        list->head = list->tail = new;
    } else {
        list->tail->next = new;
        list->tail = new;
    }
    return list;
}

/* move to the next node 
*/
node_t* 
find_next(node_t* curr_node) {
    assert(curr_node != NULL);
    curr_node = curr_node->next;
    return curr_node;
}

/* return the result of if the new input character are found in the 
current state's 'output' linked list. return a null pointer if no
match is found, otherwise return the corresponding state pointer 
*/
state_t*
search_char(char c, state_t* curr_state, int index) {
    int match;
    list_t* curr_lst = curr_state->outputs;
    char* cmpd_str;  // string to be compared with in the linked list
    node_t* curr_node = curr_lst->head;
    if(is_empty_list(curr_state->outputs)) {
        return NULL;
    }
    while (curr_node != NULL) {
        cmpd_str = curr_node->str;
        match = (c == cmpd_str[index]);
        if (match == TRUE) {
            return curr_node->state;
        }
        curr_node = find_next(curr_node);  // reach the next node in list
    }
    return NULL;
}


/* display all the required information at stage 0
*/
void 
print_stage_zero(automaton_t* automaton, int char_count) {
    printf(NOSFMT, automaton->ini->freq);  // number of statements
    printf(NOCFMT, char_count);  // number of characters
    printf(NPSFMT, automaton->nid);  // number of states
    return;
}

/* print the header for each stage
*/
void
print_header (int stage_num) {
    printf(SDELIM, stage_num);  // stage delimiter
    return;
}

/* implement stage 1 
*/
void 
do_stage_one(automaton_t *automaton) {
    print_header(ONE);  // print the header
    process_prompts(automaton);
    return;
}

void 
process_prompts(automaton_t* automaton) {
    int count, index;
    char* str = (char*) malloc(sizeof(char) * (MAX_CHAR+1));
    assert(str != NULL);
    state_t* track_state;
    /* deal with the user input line by line */
    while (!is_emptystr(str = get_str(str, MAX_CHAR))) {
        index = count = 0;
        track_state = automaton->ini;
        track_state = track_input(track_state, str, &count, index);
        print_ellipses(&count);  
        if (track_state != NULL && (track_state->freq) > 0) {
            generate_chars(&count, track_state);
        }
        putchar(NEW_LINE);
    }
    free(str);
}

/* Generate characters on the walk from the reached state to a leaf state 
by selecting the most appropriate node/state
*/
void 
generate_chars(int* count, state_t* track_state) {
    node_t* selected_node;
    char* selected_str;
    if (track_state->freq == 0) {
        return;
    }
    selected_node = select_node(track_state);
    selected_str = selected_node->str;
    for (int i = 0; i < (int) strlen(selected_str); i++) {
        if (*(count) >= MAX_CHAR) {
            return;
        }
        putchar(selected_str[i]);
    }
    track_state = selected_node->state;
    generate_chars(count, track_state);
}

/* select the node that leads to a state with the highest frequency,if two 
states are with equal frequency, select the one that is  ASCIIbetically greater
*/
node_t* 
select_node(state_t* track_state) {
    int cmp_result;
    list_t* track_list = track_state->outputs;
    /* compare the other nodes with the very first node */
    node_t* selected_node = track_list->head;
    unsigned int max_freq = selected_node->state->freq;
    node_t* curr_node = track_list->head->next;
    while (curr_node != NULL) {
        if (curr_node->state->freq > max_freq) {
            /* always prefer the one with greater frequency */
            selected_node = curr_node;
            max_freq = selected_node->state->freq;
        } else if (curr_node->state->freq == max_freq) {
            // check the one that is ASCIIbetically greater
            cmp_result = strcmp(curr_node->str, selected_node->str);
            if (cmp_result > EQUAL) {
                selected_node = curr_node;
                max_freq = selected_node->state->freq;
            }
        }
        curr_node = curr_node->next;
    }
    return selected_node;
}

state_t*
track_input(state_t* track_state, char* str, int* count, int index) {
    if (track_state == NULL) {
        return track_state;  // unmatched char found
    }
    if (str[index] == STR_END) {
        return track_state;  // reach the end, NULL state means no more outputs
    }
    putchar(str[index]);
    (*count)++;
    track_state = search_char(str[index], track_state, 0);
    return(track_input(track_state, str, count, ++index));
}

char* 
get_str(char* str, int limit) {
    int c, index = 0;
    while ((c = mygetchar()) != NEW_LINE) {
        if (index < limit) {
            str[index++] = c;
        }
    }
    str[index] = STR_END;
    return str;
}

/* return the result of if the given stirng is empty
*/
int 
is_emptystr(char* str) {
    if (str[0] == STR_END) {
        return TRUE;
    }
    return FALSE;
}

void 
print_ellipses(int* count) {
    for (int i = 0; i < ELLIPSE_SIZE; i++) { /* print ellipses */
        if ((*count) >= MAX_CHAR) {
            return;
        }
        putchar(ELLIPSE);
        (*count)++;
    }
}

void 
do_stage_two(automaton_t* automaton) {
    print_header(TWO);  // print the header
    int max_compression, tot_freq, counter;
    tot_freq = counter = 0;
    // get the number of required commpression
    scanf("%d", &max_compression);

    state_t* curr_state = automaton->ini;
    recursive_compress(curr_state, &counter, max_compression, &tot_freq);
    int num_states = automaton->nid - counter;
    print_stage_two(tot_freq,num_states);
    return;
}

void 
print_stage_two(int freq, int num_states) {
    printf(NPSFMT, num_states);
    printf(TFQFMT, freq);
    printf(MDELIM);
}

void 
recursive_compress(state_t* curr_state, int* counter, int limit, int* tot_freq){
    if (curr_state->freq == 0) { 
        // a leaf state is reached
        return;
    }
    if (is_all_visited(curr_state)) {
        return;  // all the children states are visited
    }
    if(curr_state->visited == FALSE) {
        (*tot_freq) += curr_state->freq;
    }
    state_t* next_state;
    char* prefix;
    node_t* track_node, *old_node;
    list_t* old_list;
    curr_state->visited = TRUE;
    // check if the current state has only 1 outgoing arc
    if (has_single_output(curr_state)) {
        next_state = curr_state->outputs->head->state;
        // check if the next state has one or more outgoing arcs
        if(next_state->freq > 0 && *counter < limit) {
            old_node = curr_state->outputs->head;
            prefix = old_node->str;
            old_list = curr_state->outputs;
            curr_state->outputs = next_state->outputs;
            track_node = curr_state->outputs->head;
            while (track_node != NULL) {
                // add the prefix to stored words in list
                track_node->str = combine_strs(prefix, track_node->str);
                track_node = track_node->next;  
            }
            free(next_state);
            free(old_node->str);
            free(old_node);
            free(old_list);
            (*counter)++;
            recursive_compress(curr_state, counter, limit, tot_freq);
        } else {
            recursive_compress(next_state, counter, limit, tot_freq);
        }
    } else {
        while (!is_all_visited(curr_state)) {
            next_state = select_state(curr_state);
            recursive_compress(next_state, counter, limit, tot_freq);
        }
    }
}

state_t* 
select_state(state_t* curr_state) {
    list_t* list = curr_state->outputs;
    node_t* track_node = list->head, *selected_node = NULL;
    while (track_node != NULL) {
        if (track_node->state->visited == TRUE) {
            track_node = track_node->next;
        } else {
            if (selected_node == NULL) {
                selected_node = track_node;
            } else if (strcmp(track_node->str, selected_node->str) < EQUAL) {
                selected_node = track_node;
            }
            track_node = track_node->next;
        }
    }
    return selected_node->state;
}

char* 
combine_strs(char* prefix, char* string) {
    char* combined_str;
    int new_size;
    new_size = strlen(prefix) + strlen(string) + 1;
    combined_str = malloc(sizeof(char)*new_size);
    strcpy(combined_str, prefix); // Copy the prefix into combined_str
    strcat(combined_str, string);
    // free the old memories called by previous realloc/malloc
    free(string);
    return combined_str;
}

int 
has_single_output(state_t* curr_state) {
    list_t* list = curr_state->outputs;
    if (list->head == list->tail && list->head != NULL) {
        return TRUE;
    }
    return FALSE;
}

int 
is_all_visited(state_t* curr_state) {
    int result = TRUE;
    node_t* track_node = curr_state->outputs->head;
    while (track_node != NULL) {
        if (track_node->state->visited == FALSE) {
            result = FALSE;
        }
        track_node = track_node->next;
    }
    return result;
}

void
free_automaton (automaton_t* automaton) {
    state_t* state = automaton->ini;
    // recursively free the branches (states/nodes) in the automaton
    recursive_free(state);
    free(automaton);
}

void 
recursive_free(state_t* state) {
    node_t* track_node; 
    node_t* old_node;
    if (state != NULL) {
        // free the nodes inside the linked list first
        track_node = state->outputs->head;
        while (track_node != NULL) {
            free(track_node->str);
            recursive_free(track_node->state);
            old_node = track_node;
            track_node = track_node->next;
            free(old_node);
        }
        // free the linked list now
        free(state->outputs);
        free(state);
    }
}
/* THE END -------------------------------------------------------------------*/
