#include "crushit.h"
#include "mydefs.h"

/*
EXPLAIN :
The code failed to run all of the tests successfully.
Everything up to line 89 runs smoothly,
but the results showed an error running around line 89.
Line 89 : assert(strcmp(str, "ABCDE.BCDE.CDEF.DEFABCDEFCDEFADEFAB") == 0);
The tests in this file (void test(void)) all run smoothly
*/

bool initialise(state* s, const char* str)
{
//Step 1: determine whether it's a NULL pointer
if (s == NULL || str == NULL) {
    return false;
}

//Step 2: determine whether it's a file or str
FILE* fp = fopen(str, "r");
if (fp != NULL) {
    //try to read the file
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            //read the letter and store in variable c
            char c = fgetc(fp);
            //File ending or non-letter encountered
            if ((c == EOF) || (isalpha(c) == 0)) {
                fclose(fp);
                return false;
            }
            s->board[i][j] = c;
        }
        fgetc(fp);
    }
    fclose(fp);
    return true;
} 
//another situation: it's a str
else {
    //check whether the length of the str meets requirements
    int length = strlen(str);
    if (length != (HEIGHT * WIDTH)) {
        return false;
    }
    
    //check whether there are non-alphabetic symbols in the str
    for (int i = 0; i < length; i++) {
        if (isalpha(str[i]) == 0) {
            return false;
        }
    }

    //put the letters in str into board
    for (int i = 0; i < MAXROWS; i++) {
        for (int j = 0; j < WIDTH; j++) {
            s->board[i][j] = str[i * WIDTH + j];
        }
    }
    return true;
}
}


bool tostring(const state* s, char* str)
{
//Step 1: determine whether it's a NULL pointer
if (s == NULL || str == NULL){
    return false;
}

int highestRow = -1;
//determine if this row is full of dots
for (int i = 0; i < HEIGHT; i++) {
    bool FullOfDots = true;
    for (int j = 0; j < WIDTH; j++) {
        if (s->board[i][j] != '.') {
            FullOfDots = false;
        }
    }
    //determine the highest valid row
    if (FullOfDots == false) {
        highestRow = i;
    }
}

//there is no valid letter
if (highestRow == -1) {
    str[0] = '\0';
    return true;
}

//Step 2: builds a string, 
//starting with the most significant line and skipping lines that are all '.
int k = 0;
for (int i = 0; i < HEIGHT; i++) {
    bool FullOfDots = true;
    for (int j = 0; j < WIDTH; j++) {
        if (s->board[i][j] != '.') {
            FullOfDots = false;
        }
    }

    if (FullOfDots == false) {
        for (int j = 0; j < WIDTH; j++) {
            //add the line character to the string
            str[k] = s->board[i][j];
            k++;
        }
    }
}
str[k] = '\0';
return true;
}


bool matches(state* s)
{
//Step 1: determine whether it's a NULL pointer
if (s == NULL){
    return false;
}

bool matchFlag = false;
bool matched[MAXROWS][WIDTH] = {false};

//check the vertical matches
for (int j = 0; j < WIDTH; j++) {
    //start from the bottom row
    for (int i = MAXROWS - 1; i >= 2; i--) {
        if (s->board[i][j] == s->board[i-1][j] &&
            s->board[i][j] == s->board[i-2][j] &&
            s->board[i][j] != '.' &&
            //avoid matching four values by matching only three of the same values
            (i == 2 || s->board[i][j] != s->board[i-3][j])) {
            matchFlag = true;
            matched[i][j] = matched[i-1][j] = matched[i-2][j] = true;
        }
    }
}

//check the horizontal matches
for (int i = MAXROWS - 1; i >= 0; i--) { 
    for (int j = WIDTH - 1; j >= 2; j--) {
        if (s->board[i][j] == s->board[i][j-1] &&
            s->board[i][j] == s->board[i][j-2] &&
            s->board[i][j] != '.' &&
            //avoid matching four values by matching only three of the same values
            (j == 2 || s->board[i][j] != s->board[i][j-3])) {
            matchFlag = true;
            matched[i][j] = matched[i][j-1] = matched[i][j-2] = true;
        }
    }
}

//Step 2: remove the matched letters by replacing them with '.'
for (int i = 0; i < MAXROWS; i++) {
    for (int j = 0; j < WIDTH; j++) {
        if (matched[i][j] == true) {
            s->board[i][j] = '.';
        }
    }
}
return matchFlag; 
}



bool dropblocks(state* s)
{
//Step 1: determine whether it's a NULL pointer
if (s == NULL){
    return false;
}

//Step 2: mark the drop position
bool removed = false;
for (int j = 0; j < WIDTH; j++) {
    int removedRow = HEIGHT - 1;
    //go through each column of squares from the bottom up
    for (int i = HEIGHT - 1; i >= 0; i--) {
        if (s->board[i][j] != '.') {
            if (i != removedRow) {
                //block needs to be moved to the lowest available position
                s->board[removedRow][j] = s->board[i][j];
                s->board[i][j] = '.';
                removed = true;
            }
            //update drop position, move up one space
            removedRow--;
        }
    }
}
return removed;
}



void test(void)
{
assert(initialise(NULL, "") == false);
state s;
assert(initialise(&s, NULL) == false);
//enter a string of multiples of 4
assert(initialise(&s, "ABCDABCDABCDABCDABCDABCD") == false);
//enter a string of multiples of 3
assert(initialise(&s, "ABCABCABCABCABCABCABC") == false);
//incorrect character '-'
assert(initialise(&s, "ABCDB-DAACDAABDAABCAABCDABCDBA") == false);
//incorrect character '!'
assert(initialise(&s, "ABCDB!DAACDAABDAAB!!ABCDABCDBA") == false);

/*
AAACB
DCBBD
CBBAC
BBADB
BADCB
ADBBB
*/
   // Well-formed string
   assert(initialise(&s, "AAACBDCBBDCBBACBBADBBADCBADBBB") == true);
   char str[WIDTH*MAXROWS+1];
   assert(tostring(&s, str) == true);
   assert(strcmp(str, "AAACBDCBBDCBBACBBADBBADCBADBBB") == 0);

   assert(matches(&s) == true);
/*
...CB
DCBBD
CBBAC
BBAD.
BADC.
AD...

*/
   assert(tostring(&s, str) == true);
   assert(strcmp(str, "...CBDCBBDCBBACBBAD.BADC.AD...") == 0);
   assert(dropblocks(&s) == true);
/*
.....
DC.C.
CBBB.
BBBAB
BAADD
ADDCC
*/
   assert(tostring(&s, str) == true);
   assert(strcmp(str, "DC.C.CBBB.BBBABBAADDADDCC") == 0 );

/*
.....
DC.C.
C....
...AB
BAADD
ADDCC
*/
   assert(matches(&s) == true);
   assert(tostring(&s, str) == true);
   assert(strcmp(str, "DC.C.C.......ABBAADDADDCC") == 0);
   assert(dropblocks(&s) == true);
/*
D..C.
CC.AB
BAADD
ADDCC

*/
   assert(tostring(&s, str) == true);
   assert(strcmp(str, "D..C.CC.ABBAADDADDCC") == 0);

}

