#ifndef DATPARSER_H
#define DATPARSER_H

#define MAX_LINES 1
#define MAX_CHARS_PER_LINE 3300 /* Adjust, if input size varies */

/* Created:       06.06.2023
   Last modified: 13.06.2023
   @ Jukka J jajoutzs@jyu.fi
*/

typedef struct {
    double* values;
    int numValues;
} LineData;

void parse_file(const char* filename, LineData* data, int* numLines);

#endif

