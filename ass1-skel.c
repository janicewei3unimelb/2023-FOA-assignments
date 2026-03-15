/* Program to perform multi-digit integer arithmetic.

   Skeleton program written by Alistair Moffat, ammoffat@unimelb.edu.au,
   August 2023, with the intention that it be modified by students
   to add functionality, as required by the assignment specification.
   All included code is (c) Copyright University of Melbourne, 2023

   Student Authorship Declaration:

   (1) I certify that except for the code provided in the initial skeleton
   file, the  program contained in this submission is completely my own
   individual work, except where explicitly noted by further comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students, or by
   non-students as a result of request, solicitation, or payment, may not be
   submitted for assessment in this subject.  I understand that submitting for
   assessment work developed by or in collaboration with other students or
   non-students constitutes Academic Misconduct, and may be penalized by mark
   deductions, or by other penalties determined via the University of
   Melbourne Academic Honesty Policy, as described at
   https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will I
   do so until after the marks are released. I understand that providing my
   work to other students, regardless of my intention or any undertakings made
   to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring service,
   or drawing the attention of others to such services and code that may have
   been made available via such a service, may be regarded as Student General
   Misconduct (interfering with the teaching activities of the University
   and/or inciting others to commit Academic Misconduct).  I understand that
   an allegation of Student General Misconduct may arise regardless of whether
   or not I personally make use of such solutions or sought benefit from such
   actions.

   Signed by: Junchen Wei 1399443
   Dated:     3/9/2023

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <assert.h>
#include <unistd.h>

/* All necessary #defines provided as part of the initial skeleton */

#define INTSIZE	500	/* max number of digits per integer value */
#define ELESIZE INTSIZE+1 /* max number of elements per interger value*/
#define LINELEN	999	/* maximum length of any input line */
#define NVARS	26	/* number of different variables */

#define CH_A     'a'    /* character 'a', first variable name */

#define ERROR	(-1)	/* error return value from some functions */
#define PROMPT	"> "	/* the prompt string for interactive input */

#define PRINT	'?'	/* the print operator */
#define ASSIGN	'='	/* the assignment operator */
#define PLUS	'+'	/* the addition operator */
#define MULT	'*'	/* the multiplication operator */
#define POWR	'^'	/* the power-of operator */
#define DIVS	'/'	/* the division operator */
#define ALLOPS  "?=+*^/"

#define CH_ZERO  '0'    /* character zero */
#define CH_ONE   '1'    /* character one */
#define CH_NINE  '9'    /* character nine */
#define TERMINATION '\0'   /* terminate a string with a null char*/

#define CH_COM   ','    /* character ',' */
#define PUT_COMMAS 3    /* interval between commas in output values */

#define INT_ZERO 0	/* integer 0 */
#define INT_TEN  10	/* integer 10 */
#define INT_ONE  1  /* integer 1 */
#define MAX_POWER 1661 /* upper limit of power given by base 2*/
#define MAX_POW_DIG 4 /* max number of digits for power */

#define SMALLER -1
#define LARGER 1
#define EQUAL 0

/* Placeholder typedef for skeleton code
 */
typedef int longint_t[ELESIZE];
#define LONG_ZERO {0}
#define LONG_ONE  {1, 1}


/****************************************************************/

/* A "magic" additional function needing explicit declaration */
int fileno(FILE *);

/* Skeleton program function prototypes */

void print_prompt(void);
void print_tadaa();
void print_error(char *message);
int  read_line(char *line, int maxlen);
void process_line(longint_t vars[], char *line);
int  get_second_value(longint_t vars[], char *rhsarg,
                     longint_t second_value);
int  to_varnum(char ident);
void do_print(int varnum, longint_t var);
void do_assign(longint_t var1, longint_t var2);
void do_plus(longint_t var1, longint_t var2);
void zero_vars(longint_t vars[]);
void parse_num(char* rhs, longint_t converted_value);
void exit_if_overflow(int condition);
void do_multiply(longint_t var1, longint_t var2);
void tmp_product(longint_t to_plus, longint_t var1, int index, int val);
void do_power(longint_t var1, longint_t var2);
int get_power(longint_t var2, int len);
void do_divide(longint_t var1, longint_t var2);
int do_compare(longint_t var1, longint_t var2);
void exit_if_undefined(int condition);
void do_subtract(longint_t var1, longint_t var2);
void get_valid_len(longint_t var1, int len);
void multiply_tens(longint_t var, int num);
void divide_ten(longint_t var);

/****************************************************************/

/* Main program controls all the action
 */
int
main(int argc, char *argv[]) {
  char line[LINELEN+1] = {0};
  longint_t vars[NVARS];

  zero_vars(vars);
  print_prompt();
  while (read_line(line, LINELEN)) {
    if (strlen(line) > 0) {
      /* non empty line, so process it */
      process_line(vars, line);
    }
    print_prompt();
  }

  print_tadaa();
  return 0;
}

/****************************************************************/

/* Prints the prompt indicating ready for input, but only if it
   can be confirmed that the input is coming from a terminal.
   Plus, output might be going to a file, that's why the prompt,
   if required, is written to stderr and not stdout
*/
void
print_prompt(void) {
  if (isatty(fileno(stdin))) {
    fprintf(stderr, "> ");
    fflush(stderr);
  }
}

void
print_tadaa() {
  /* all done, so pack up bat and ball and head home,
     getting the exact final lines right is a bit tedious,
     because input might be coming from a file and output
     might be going to a file */
  if (isatty(fileno(stdin)) && isatty(fileno(stdout))) {
    printf("\n");
  }
  printf("ta daa!!!\n");
  if (isatty(fileno(stdin)) && !isatty(fileno(stdout))) {
    fprintf(stderr, "\n");
  }
}

void
print_error(char *message) {
  /* need to write an error message to the right place(s)
   */
  if (isatty(fileno(stdin)) || isatty(fileno(stdout))) {
    fprintf(stderr, "%s\n", message);
    fflush(stderr);
  }
  if (!isatty(fileno(stdout))) {
    printf("%s\n", message);
  }
}

/****************************************************************/

/* Reads a line of input into the array passed as argument,
   returns false if there is no input available.
   All whitespace characters are removed on the way through.
*/
int
read_line(char *line, int maxlen) {
  int i=0, c;
  while (((c=getchar())!=EOF) && (c!='\n')) {
    if (i<maxlen && !isspace(c)) {
      line[i++] = c;
    }
  }
  line[i] = '\0';
  /* then, if the input is coming from a file or the output
     is going to a file, it is helpful to echo the input line
     and record what the command was */
  if (!isatty(fileno(stdin)) || !isatty(fileno(stdout))) {
    printf("%s%s\n", PROMPT, line);
  }
  return ((i>0) || (c!=EOF));
}

/****************************************************************/

/* Process a command by parsing the input line into parts
 */
void
process_line(longint_t vars[], char *line) {
  int varnum, optype, status;
  longint_t second_value;

  /* determine the LHS variable, it
     must be first character in compacted line
  */
  varnum = to_varnum(line[0]);
  if (varnum==ERROR) {
    print_error("invalid LHS variable");
    return;
  }

  /* more testing for validity 
   */
  if (strlen(line)<2) {
    print_error("no operator supplied");
    return;
  }

  /* determine the operation to be performed, it
     must be second character of compacted line
  */
  optype = line[1];
  if (strchr(ALLOPS, optype) == NULL) {
    print_error("unknown operator\n");
    return;
  }

  /* determine the RHS argument (if one is required),
     it must start in the third character of compacted line
  */
  if (optype != PRINT) {
    if (strlen(line)<3) {
      print_error("no RHS supplied");
      return;
    }
    status = get_second_value(vars, line+2, second_value);
    if (status==ERROR) {
      print_error("RHS argument is invalid");
      return;
    }
  }

  /* finally, do the actual operation */
  if (optype == PRINT) {
    do_print(varnum, vars[varnum]);
  } else if (optype == ASSIGN) {
    do_assign(vars[varnum], second_value);
  } else if (optype == PLUS) {
    do_plus(vars[varnum], second_value);
  } else if (optype == MULT) {
    do_multiply(vars[varnum], second_value);
  } else if (optype == POWR) {
    do_power(vars[varnum], second_value);
  } else if (optype == DIVS) {
    do_divide(vars[varnum], second_value);
  }else {
    print_error("operation not available yet");
    return;
  }
  return;
}

/****************************************************************/

/* Convert a character variable identifier to a variable number
 */
int
to_varnum(char ident) {
  int varnum;
  varnum = ident - CH_A;
  if (0<=varnum && varnum<NVARS) {
    return varnum;
  } else {
    return ERROR;
  }
}

/****************************************************************/

/* Process the input line to extract the RHS argument, which
   should start at the pointer that is passed
*/
int
get_second_value(longint_t vars[], char *rhsarg,
                 longint_t second_value) {
  char *p;
  int varnum2;
  if (isdigit(*rhsarg)) {
    /* first character is a digit, so RHS is a number
       now check the rest of RHS for validity */
    for (p=rhsarg+1; *p; p++) {
      if (!isdigit(*p)) {
        /* nope, found an illegal character */
        return ERROR;
      }
    }
    /* nothing wrong, ok to convert */
    parse_num(rhsarg, second_value);
    return !ERROR;
  } else {
    /* argument is not a number, so should be a variable */
    varnum2 = to_varnum(*rhsarg);
    if (varnum2==ERROR || strlen(rhsarg)!=1) {
      /* nope, not a variable either */
      return ERROR;
    }
    /* and finally, get that variable's value */
    do_assign(second_value, vars[varnum2]);
    return !ERROR;
  }
  return ERROR;
}

/* Set the vars array to all zero values
 */
void
zero_vars(longint_t vars[]) {
  int i;
  longint_t zero = LONG_ZERO;
  for (i=0; i<NVARS; i++) {
    do_assign(vars[i], zero);
  }
  return;
}

/*****************************************************************
******************************************************************

Your answer to the assignment should start here, using your new
typedef defined at the top of the program. The next few functions
will require modifications because of the change of structure
used for a long_int, and then you'll need to start adding whole
new functions after you get these first ones working properly.
Try and make the new functions fit the style and naming pattern
of the existing ones, ok?

******************************************************************
*****************************************************************/

/* Create an internal-format number out of a character
 */
void
parse_num(char* rhs, longint_t converted_value) {
  int len = strlen(rhs), i;
  exit_if_overflow(len > INTSIZE);

  for (i = 1; i <= len; i++) {
    /* record the numbers reversely */
    converted_value[i] = rhs[len - i] - CH_ZERO;
  }
  /* adjust the len */
  get_valid_len(converted_value, len);  
}

/****************************************************************/

/* Print out a longint value
 */
void
do_print(int varnum, longint_t var) {
  int i, counter = 0, remainder;
  /* get the number of digits before the first comma */ 
  remainder = var[0] % PUT_COMMAS;  
  printf("register %c: ", varnum+CH_A);

  if (var[0] == INT_ZERO) {
    printf("%d\n", INT_ZERO);  /* number is 0 */
  } else { 
    for (i = var[0]; i > 0; i--) {
      counter++;
      /* put commas: (1) when there are more than 3 digits. 
		     (2) after at least 1 digit, obey intervals of 3. */
      if (var[0] > PUT_COMMAS && counter != 1 &&
         (counter - remainder) % PUT_COMMAS == 1) {
        printf("%c" ,CH_COM);
      }
      printf("%d", var[i]);
    }
    printf("\n");
  }
}

/****************************************************************/

/* Assign a longint value, could do this with just an assignment
   statement, because structs can be assigned, but this is more
   elegant, and only copies over the array elements (digits) that
   are currently in use: var1 = var2

*/
void
do_assign(longint_t var1, longint_t var2) {
  int i, len = var2[0];
  if (len == INT_ZERO) {
    //make it to 0s / initialisation
    var1[0] = INT_ZERO;
  } else {
    // get the value stored in an array
    for (i = 0; i <= len; i++) {
      var1[i] = var2[i];
    }
  }
}

/****************************************************************/

/* Update the indicated variable var1 by doing an addition
   using var2 to compute var1 = var1 + var2
*/
void
do_plus(longint_t var1, longint_t var2) {
  int carry = 0, i, sum;
  /* use var2 to track the new len */
  int len = var2[0], var1_len = var1[0]; 

  /* nothing to add */
  if (len == INT_ZERO) {
    return;
  }
  /* doing addings now! */
  for (i = 1; i <= len; i++) {  /* skip the first element */      
    if (i <= var1_len) {
      sum = var1[i] + var2[i] + carry;  
    } else {
      sum = var2[i] + carry; /* in case there is a long var2 */
    }
    carry = sum / INT_TEN;
    var1[i] = sum % INT_TEN; /* store the number on ones */
  }

  /* dealing with left carries */
  while (carry != 0) {
    len++;
    exit_if_overflow(len > INTSIZE);
    if (len <= var1_len) {  /* in case there is a long var1 */
      sum = var1[len] + carry;
    } else {
      sum = carry;
    }
    carry = sum / INT_TEN;
    var1[len] = sum % INT_TEN;  
  }
  if (len > var1_len) {
    var1[0] = len;  /* update the number of digits */
  }
}

/*****************************************************************
******************************************************************

Put your new functions below this line. Make sure you add suitable
prototypes at the top of the program.

******************************************************************
*****************************************************************/

/* Exits with an error message if there is an overflow 
*/
void 
exit_if_overflow(int condition) {
  if (condition) {
    printf("Too many digits! Overflow Occurred!\n");
    exit(EXIT_FAILURE);
  }
}

/****************************************************************/

/* count the valid length the longint number and store it in the
array's first element 
*/
void 
get_valid_len(longint_t var1, int len) {
  /* count useful digits and useless 0s */
  int count = 0, usls_0_count = 0, i;
  for (i = 1; i <= len; i++) {
    if(var1[i] == INT_ZERO) {
      usls_0_count++;  /* found potentially useless 0 */
    } else {
      count = count + usls_0_count + 1;
      usls_0_count = 0;  /* the 0s before are meaningful :)*/
    }
  }
  var1[0] = count;
}

/****************************************************************/

/* Update the indicated variable var1 by doing a multiplication
   using var2 to compute var1 = var1 * var2 
*/
void 
do_multiply(longint_t var1, longint_t var2) {
  int i;
  /* 2 extra longint arrays are needed to track the original 
  var1 value and add up all the temporary products */
  longint_t to_assign = LONG_ZERO, to_plus = LONG_ZERO;

  /* multiplication with 0 */
  if (var2[0] == INT_ZERO) {
    var1[0] = INT_ZERO;
    return;
  } else if (var1[0] == INT_ZERO) {
    return; 
  }
  /* multiply non-zero numbers now */
  for (i=1; i <= var2[0]; i++) {
    tmp_product(to_plus, var1, i, var2[i]);
    /* add all the temporary products together */
    do_plus(to_assign, to_plus);
  }
  do_assign(var1, to_assign);  /* assign the result to var1 */
}

/****************************************************************/

/* get the temporary products of var1 and the digit pointed by 'index' 
in the var2 array in order to find the real product of longints
*/
void 
tmp_product(longint_t to_plus, longint_t var1, int index, int val) {
  int j, carry = 0, product = 0;
  int diff = index - 1;  /* determines the number of 0s added at the end */
  int len = var1[0] + diff;  /* extend the len */
  exit_if_overflow(len > INTSIZE);
  /* multiply with all digits in var1*/
  for (j = 1; j <= len; j++) {  
    if (j < index) {
      to_plus[j] = INT_ZERO;  /* fill 0s */
    } else {
      product = var1[j - diff] * val + carry;
      carry = product / INT_TEN;
      to_plus[j] = product % INT_TEN;
    }
  }
  /* only one potential extra digit for multiplication */
  if (carry != 0) {
    len++;
    exit_if_overflow(len > INTSIZE);
    to_plus[len] = carry;
  }
  to_plus[0] = len;  /* update the length */
}

/****************************************************************/

/* Update the indicated variable var1 by doing a power operation
   using var2 to compute var1 = var1 ^ var2 
*/
void 
do_power(longint_t var1, longint_t var2) {
  int power;
  longint_t one = LONG_ONE, record = LONG_ONE, square;

  /* check special cases with base 1 or 0; and power of 0 */
  if (var1[0] == INT_ZERO || (var1[0] == INT_ONE && var1[1] == INT_ONE)) {
    return; 
  }
  if (var2[0] == INT_ZERO) {
    do_assign(var1, one);
    return; 
  }
  /* check the upper limit of power */
  exit_if_overflow(var2[0] > MAX_POW_DIG);
  power = get_power(var2, var2[0]);
  exit_if_overflow(power >= MAX_POWER);

  /* get the square of var1 to improve overall efficiency */
  do_assign(square, var1);
  do_multiply(square, square);

  while (power > 0) {
    if (power == 1) {
      do_multiply(record, var1);
      power--;
    } else {
      do_multiply(record, square);
      power -= 2;
    }
  }
  do_assign(var1, record);
}

/****************************************************************/

/* return the value of power in the form of a single integer
*/
int 
get_power(longint_t var2, int len) {
  int power = 0, i;
  for (i = len; i > 0; i--) {
    power = INT_TEN * power + var2[i];
  }
  return power;
}

/****************************************************************/

/* Update the indicated variable var1 by doing a division
   using var2 to compute var1 = var1 / var2 
*/
void 
do_divide(longint_t var1, longint_t var2) {
   /* denominator of 0 - undefined */
  exit_if_undefined(var2[0] == INT_ZERO); 

  longint_t one = LONG_ONE, divisor;
  char result[INTSIZE + 1];  /* record the digits in a string */
  int cmp_result, str_len = 0;

  /* detect and return results of 0 and 1*/
  if (var1[0] == INT_ZERO) {  /* numerator of 0 */
    return;
  }
  cmp_result = do_compare(var1, var2);
  if (cmp_result == SMALLER) {  /* bigger denominator */
    var1[0] = INT_ZERO;
    return;
  } else if (cmp_result == EQUAL) {
    do_assign(var1, one);
    return;
  }

  /* estimate an expected len of the result, which would be at most 
  (var1[0] - var2[0] + 1), at least (var1[0] - var2[0]) */
  int expected_len = var1[0] - var2[0] + 1, count, i;  
  /* keep adding digits to result until dividend is less than divisor*/
  while (do_compare(var1, var2) != SMALLER) {
    /* put zeros to fill the gaps (if there is any) */
    for (i = 0; i <= (divisor[0] - var1[0] - 1); i++) {
      result[str_len] = CH_ZERO;
      str_len++;
    }
    count = 0;  /* count # of subtractions to find the digit (1-9) to put */
    do_assign(divisor, var2); 

    /* get a suitable (MAX & VALID) divisor for doing subtraction */
    multiply_tens(divisor, var1[0] - divisor[0]);
    if (do_compare(divisor, var1) == LARGER) {
      divide_ten(divisor);
      if (str_len == 0) {
        expected_len--;
      }  
    }
    while (do_compare(var1, divisor) != SMALLER) {  /* do subtractions now */
      count++;
      do_subtract(var1, divisor);
    }
    /* place the digit contributed by count (ie: # of subtractions) */
    result[str_len++] = count + CH_ZERO;
  }

  /* add enidng zeros if there is any*/
  int num_zeros = expected_len - str_len;  /* number of 0s at the end */
  for (i = 0; i < num_zeros; i++) {
    printf("###\n");
    result[str_len++] = CH_ZERO;
  }
  result[str_len] = TERMINATION;  
  parse_num(result, var1);  /* pass the number info of 'result' to var1 */
}

/****************************************************************/

/* compare the numbers stored in var1 and var2, return 1 if var1 is bigger, 
return -1 if var1 is smaller, return 0 if var1 and var2 are equal 
*/
int 
do_compare(longint_t var1, longint_t var2) {
  int i;
  if (var1[0] < var2[0]) {
    return SMALLER;
  } else if (var1[0] > var2[0]) {
    return LARGER;
  }
  /* numbers with equal numbers of digits */
  for (i = var1[0]; i > 0; i--) {
    if (var1[i] > var2[i]) {
      return LARGER;
    } else if (var1[i] < var2[i]) {
      return SMALLER;
    }
  }
  return EQUAL;
}

/****************************************************************/

/* exit the program when the number is undefined in maths 
*/
void 
exit_if_undefined(int condition) {
  if (condition) {
    printf("The result is undefined!\n");
    exit(EXIT_FAILURE);
  }
}

/****************************************************************/

/* Update the indicated variable var1 by doing a subtraction
   using var2 to compute var1 = var1 - var2, given that var1 
  is no less than var2, and these two are non-zero 
*/
void 
do_subtract (longint_t var1, longint_t var2) {
  int borrow = 0, i, diff = 0;
  int len = var1[0], var2_len = var2[0];
  for (i=1; i <= len; i++) {
    var1[i] = var1[i] - borrow;
    borrow = 0;
    if (i <= var2_len && var1[i] < var2[i]) {
      /* borrow ten from the next digit */
      var1[i] = var1[i] + INT_TEN;
      borrow = 1;
    }
    if (i <= var2_len) {
      diff = var1[i] - var2[i];
    } else {
      diff = var1[i];  /* in case there is a long var1 */
    }
    var1[i] = diff;
  }
  get_valid_len(var1, var1[0]);  /* eliminate the useless 0s */
}

/****************************************************************/

/* modify var by multipling this longint number by 'num' number of 10s 
*/
void 
multiply_tens(longint_t var, int num) {
  if (num == 0) {
    return; 
  }
  int i;
  /* modify the len */
  var[0] = var[0] + num;
  for (i = var[0]; i > 0; i--) {
    if (i > num) {
      var[i] = var[i - num];
    } else {
      var[i] = INT_ZERO;
    }
  }
}

/****************************************************************/

/* modify var by dividing this longint number by 10 
*/
void 
divide_ten(longint_t var) {
  int len = --var[0], i;
  for (i = 1; i <= len; i++) {
    var[i] = var[i + 1];
  }
}

/* Algorithms are FUN ??? :D */