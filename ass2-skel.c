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
#include <ctype.h> 

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
#define INT_TEN 10
#define CHAR_STR_SIZE 1+1  // the size of a string of 1 char
#define STR_END '\0'
#define EQUAL 0
#define MAX_CHAR 37
#define ELLIPSE_SIZE 3
#define ASCII_DIFF 48  // difference between the num and its ASCII value
#define LEAF_FREQ 0  // frequency of the leaf state

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
void print_stage_zero(automaton_t* automaton, int char_count);
void do_stage_one(automaton_t* automaton);
void print_header(int stage_num);
void process_prompts(automaton_t* automaton);
char* get_str(char* str, int limit);
void print_ellipses(int* count);
node_t* select_node(state_t* track_state);
void generate_chars(int* count, state_t* track_state);
int is_emptystr(char* str);
state_t* track_input(state_t* track_state, char* str, int* count, int index, 
int inner_index);
void do_stage_two(automaton_t* automaton);
void recursive_compress(state_t* curr_state, int* counter, int limit, 
int* freq);
int has_single_output(state_t* curr_state); 
char* combine_strs(char* prefix, char* string);
int is_all_visited(state_t* curr_state);
state_t* select_state(state_t* curr_state);
void print_stage_two(int freq, int num_states);
void free_automaton(automaton_t* automaton);
void recursive_free(state_t* state);
int get_int(void);
state_t* finish_print(int* count, int index, state_t* track_state);
state_t* finish_str(node_t* selected_node, int index, int* count);

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

/* initialise the automaton tree with the very initial state 
*/
automaton_t* 
initialise_automaton() {
    automaton_t* automaton;
    automaton = malloc(sizeof(*automaton));
    assert(automaton != NULL);
    automaton->nid = 0;  /* id of the initial state*/
    state_t* initial_state = create_new_state(automaton);
    /* store the initial state to the 'ini' of automaton */
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
    new_state->freq = 0;  /* no strings have departured here yet */
    new_state->visited = FALSE;  /* initialise it to FALSE for stage 2 */
    new_state->outputs = make_empty_list();
    return new_state;
}

/* make an empty linked list ;
The majority the code of the function 'make_empty_list' is from 'Programming, 
Problem Solving, And Abstraction' by Alistair Moffat (the original author), 
P172, Figure 10.4  
*/
list_t* 
make_empty_list(void) {
    list_t* list;
    list = (list_t*) malloc(sizeof(*list));
    assert(list != NULL);
    list->head = list->tail = NULL;  /* fill the list with NULLs */
    return list;
}

/* check if the linked list is empty
*/
int
is_empty_list(list_t *list) {
    assert(list != NULL);
    return (list->head == NULL && list->tail == NULL);
}

/* implement the stage zero by creating states corresponding to each single 
 character and link them together. Go back to the inital state and track the 
 user input whenever a new line chracter is entered. 
*/
void  
do_stage_zero(automaton_t* automaton) {
    print_header(ZERO);
    int char_count = 0, new_line = FALSE, c, index = 0;
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
            /* search c in the 'outputs' of the current state to 
            determine if a new branch is needed */
            if((next_state = search_char(c, track_state, index)) == NULL) {
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
}

/* convert a single char to string 
*/
char*
create_char_str(char c) {
    char* char_str = (char*) malloc(sizeof(char) * CHAR_STR_SIZE);
    assert(char_str != NULL);
    char_str[0] = c;
    char_str[1] = STR_END;  // termination of the string
    return char_str;
}

/* insert new node at the current linked list's tail.
Some of the code for the function 'insert_new_node' is from 'Programming, 
Problem Solving, And Abstraction' by Alistair Moffat (the original author), 
P173, Figure 10.5 
*/
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

/* return the result of if the new input character is found in the 
current state's 'outputs' linked list. return a NULL pointer if no
match is found, otherwise return the corresponding state pointer 
*/
state_t*
search_char(char c, state_t* curr_state, int index) {
    int match;
    list_t* curr_lst = curr_state->outputs;
    char* cmpd_str;  // string to be compared with in the linked list
    node_t* curr_node = curr_lst->head;
    if(is_empty_list(curr_state->outputs)) {
        return NULL;  /* no contents in the linked list */
    }
    while (curr_node != NULL) {
        cmpd_str = curr_node->str;
        match = (c == cmpd_str[index]);
        if (match == TRUE) {
            if (cmpd_str[++index] != STR_END) {
                return curr_state;  // the string has multiple chars
            }
            return curr_node->state;
        }
        curr_node = curr_node->next;  // reach the next node in list
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
}

/* print the header for each stage
*/
void
print_header (int stage_num) {
    printf(SDELIM, stage_num);  // stage delimiter
}

/* implement stage 1 starting with its header
*/
void 
do_stage_one(automaton_t *automaton) {
    print_header(ONE);  // print the header
    process_prompts(automaton);
}

/* Replay the prompt unitl a blank line is encountered. Generate the 
expected characters after the matched part
*/
void 
process_prompts(automaton_t* automaton) {
    int count, index, inner_index;
    char* str = (char*) malloc(sizeof(char) * (MAX_CHAR+1));
    assert(str != NULL);
    state_t* track_state;
    /* deal with the user input line by line */
    while (!is_emptystr(str = get_str(str, MAX_CHAR))) {
        index = count = inner_index = 0;
        track_state = automaton->ini;
        track_state = track_input(track_state, str, &count, index, 
        inner_index);  // replay the prompt
        if (track_state != NULL && (track_state->freq) > 0) {
            generate_chars(&count, track_state);
        }  // generate characters
        putchar(NEW_LINE);
    }
    free(str);
}

/* Recursively generate characters on the walk from the reached state to a 
leaf state by selecting the most appropriate node/state
*/
void 
generate_chars(int* count, state_t* track_state) {
    node_t* selected_node;
    char* selected_str;
    if (track_state->freq == LEAF_FREQ) {
        return;  // stop generating when a leaf state is reached
    }
    selected_node = select_node(track_state);
    selected_str = selected_node->str;
    for (int i = 0; i < (int) strlen(selected_str); i++) {
        if (*(count) >= MAX_CHAR) {
            /* stop the generation whenver the 37 char limit is reached */
            return;
        }
        putchar(selected_str[i]);
        (*count)++;
    }
    track_state = selected_node->state;
    generate_chars(count, track_state);
}

/* select the node that leads to a state with the highest frequency,if two 
states are with equal frequency, select the one that is  ASCIIbetically greater.
Return the selected node at the end. 
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
            // select the one that is ASCIIbetically greater
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

/* replay the user input and track the corresponding states until the 
user input string finishes or an unmatch is found. Print the ellipses 
after the matching characters and finish printing the matched string 
(if there is an unfinished matched string on that state). 
*/
state_t*
track_input(state_t* track_state, char* str, int* count, int index, 
int inner_index) {
    state_t* result_state;
    if (track_state == NULL || str[index] == STR_END) {
        print_ellipses(count); 
        if (track_state != NULL && inner_index > 0) { /* unfinished string! */
            track_state = finish_print(count, inner_index, track_state);
        }
        return track_state;
    }
    putchar(str[index]);
    (*count)++;  
    result_state = search_char(str[index], track_state, inner_index);
    if (result_state == track_state) {  // string with more than 1 character
        inner_index++;  // move to the next character in the current string
    } else {
        inner_index = 0;  // track from the first character in the next string
    }
    return(track_input(result_state, str, count, ++index, inner_index));
}

/* Given that the last bit of user input matches the prefix of strings but 
hasn't finished printing all the chracters there. Make choices of the matched
node and call 'finish_str' to finish printing the content. 
*/
state_t*
finish_print (int* count, int index, state_t* track_state) {
    node_t* selected_node;
    state_t* next_state;
    if (has_single_output(track_state)) { 
        selected_node = track_state->outputs->head;
    } else {
        selected_node = select_node(track_state);
    }
    /* update the state that we are tracking */
    next_state = finish_str(selected_node, index, count);
    return next_state;
}

/* Given that there is unfinished string (with matched characters followed 
by characters that haven't been compareed due to the stop of replay), finish
printing the charaters of them, them move to the next state
*/
state_t*
finish_str(node_t* selected_node, int index, int* count) {
    char* str = selected_node->str;
    for (int i = index; str[i] != STR_END; i++) {
        if (*count >= MAX_CHAR) {
            /* the upper limit of chars is reached, return a NULL pointer 
            to indicate that no more char should be generated */
            return NULL;  
        }
        putchar(str[i]);
        (*count)++;
    }
    return selected_node->state;
 }

/* convert the user input in each line into a string and return it 
*/
char* 
get_str(char* str, int limit) {
    int c, index = 0;
    while ((c = mygetchar()) != NEW_LINE && c != EOF) {
        if (index < limit) { // cannot print more than 37 chars
            str[index++] = c;
        }
    }
    str[index] = STR_END;  // terminates the string
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

/* print the ellipses after the matched characters (+ the first
unmatched char) while tracking the output size. Stop printing
whenever the upper limit of size is encountered. 
*/
void 
print_ellipses(int* count) {
    for (int i = 0; i < ELLIPSE_SIZE; i++) { 
        if ((*count) >= MAX_CHAR) {
            return;  // upper limit of # of chars is reached, no more printing
        }
        putchar(ELLIPSE);
        (*count)++;
    }
}

/* implement stage two. Get the number of required number of compression 
steps andcall a recursive function 'recursive_compress'to compress the automaton
tree. Then replay the prompt and generate required charcters. 
*/
void 
do_stage_two(automaton_t* automaton) {
    print_header(TWO);  // print the header
    int max_compression, tot_freq, counter;
    tot_freq = counter = 0;
    // get the number of required commpression
    max_compression = get_int();
    state_t* curr_state = automaton->ini;
    recursive_compress(curr_state, &counter, max_compression, &tot_freq);
    // num_states = number of total stages - # deleted stages
    int num_states = automaton->nid - counter;
    print_stage_two(tot_freq,num_states);
    process_prompts(automaton);
    printf(THEEND);
}

/* convert the user input of the number of compression into a 
single integer number
*/
int 
get_int(void) {
    int num = 0, c; 
    while ((c = mygetchar()) != NEW_LINE) {
        if (isdigit(c)) {
            num = INT_TEN * num + (c - ASCII_DIFF);
        }
    }
    return num;
}

/* print the required information to display afte the compression
*/
void 
print_stage_two(int freq, int num_states) {
    printf(NPSFMT, num_states);  // number of states
    printf(TFQFMT, freq);  // total frequency
    printf(MDELIM);  //delimeter of -s
}

/* Compress the automaton recursively by following the depth-
first order and free the abandoned components in the automaton.  
*/
void 
recursive_compress(state_t* curr_state, int* counter, int limit, 
int* tot_freq) {
    if (curr_state->freq == LEAF_FREQ || is_all_visited(curr_state)) { 
        return;
    }
    if(curr_state->visited == FALSE) {  // only consider the new frequencies
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
        if(next_state->freq > LEAF_FREQ && *counter < limit) {
            old_node = curr_state->outputs->head;
            prefix = old_node->str;
            old_list = curr_state->outputs;
            /* replace the curent state's outputs */
            curr_state->outputs = next_state->outputs;
            track_node = curr_state->outputs->head;
            while (track_node != NULL) {
                // add the prefix to stored words in list
                track_node->str = combine_strs(prefix, track_node->str);
                track_node = track_node->next;  
            }
            free(old_node->str);
            free(old_node);  // free the contents in the old linked list
            free(old_list);  // free the old linked list itself
            free(next_state); // free state pointer only，keep the list/nodes
            (*counter)++;
            /* stay at the current state to see if compressions can occur
            between the states after the deleted one and the current state */
            recursive_compress(curr_state, counter, limit, tot_freq);
        } else {
            // move to the next state
            recursive_compress(next_state, counter, limit, tot_freq);
        }
    } else {
        while (!is_all_visited(curr_state)) {
            // follow the depth-first order
            next_state = select_state(curr_state);
            recursive_compress(next_state, counter, limit, tot_freq);
        }
    }
}

/* select the state for the next recursive compression based on the requirements
for the impletation of the first-depth order. Select and return the unvisited 
state led by the node of string with smaller ASCII label.
*/
state_t* 
select_state(state_t* curr_state) {
    list_t* list = curr_state->outputs;
    /* track nodes since they can provide us with more info */
    node_t* track_node = list->head, *selected_node = NULL;
    while (track_node != NULL) {
        if (track_node->state->visited == TRUE) {
            track_node = track_node->next;  // visited states are unwanted
        } else {
            if (selected_node == NULL) {
                selected_node = track_node;  // the first candidate
            } else if (strcmp(track_node->str, selected_node->str) < EQUAL) {
                /* replace the selected node with smaller ASCII label */
                selected_node = track_node;  
            }
            track_node = track_node->next;
        }
    }
    return selected_node->state;
}

/* return the combined string that is the result of prefix + string
*/
char* 
combine_strs(char* prefix, char* string) {
    char* combined_str;
    int new_size; // size of the result of the combined string
    new_size = strlen(prefix) + strlen(string) + 1;
    combined_str = malloc(sizeof(char)*new_size);
    assert(combined_str != NULL);
    strcpy(combined_str, prefix); // Copy the prefix into combined_str
    strcat(combined_str, string); // Append the string to combined_str
    // free the old memories called by previous malloc
    free(string);
    return combined_str;
}

/* return the result of the given state has only one output
(if it only leads to one state ). 
*/
int 
has_single_output(state_t* curr_state) {
    list_t* list = curr_state->outputs;
    if (list->head == list->tail && list->head != NULL) {
        return TRUE;
    }
    return FALSE;
}

/* resturn the result of if the states departured from the given 
state have all been visited 
*/
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

/* free the automaton by calling a recurive function 'recursive_free'
to free each state and components inside that are generated from
malloc. Start the reursion from the initial state
*/
void
free_automaton (automaton_t* automaton) {
    state_t* state = automaton->ini;
    // recursively free the branches (states/nodes) in the automaton
    recursive_free(state);
    free(automaton);
}

/* recursively free each state and their contents in the 
automaton tree. 
*/
void 
recursive_free(state_t* state) {
    node_t* track_node; 
    node_t* old_node;
    if (state != NULL) {
        // free all the nodes inside the linked list first
        track_node = state->outputs->head;
        while (track_node != NULL) {
            free(track_node->str);
            recursive_free(track_node->state);
            old_node = track_node;
            track_node = track_node->next;
            free(old_node);
        }
        // free the linked list and state now
        free(state->outputs);
        free(state);
    }
}
/* THE END -------------------------------------------------------------------*/
// Algorithms Are Fun ????? :D