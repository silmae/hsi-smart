#include "dat_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tools.h"

/* Created:       06.06.2023
   Last modified: 13.06.2023
   @ Jukka J jajoutzs@jyu.fi
*/

/* Number with 401 inputs around 3207. This kind of number is likely
   the upper-limit for single lines char count.
*/

void parse_file(const char* filename, LineData* data, int* num_lines) {
    FILE* file;

    /* Open the file for reading */
    file = fopen(filename, "r");
    if (file == NULL) {
        return;
    }

    /* Reading the first line to determine the count of the values */
    char firstLine[MAX_CHARS_PER_LINE];
    if (fgets(firstLine, sizeof(firstLine), file)) {
        char* token;
        int count = 0;
        double value;
        /* Per line */
        int actual_char_count = 0;
        token = strtok(firstLine, "\t");
        while (token != NULL) {
            count++;
            actual_char_count += strlen(token);
            actual_char_count += strlen("\t");
            token = strtok(NULL, "\t");
        }

        /* Allocating memory for the maximum number of values per line */
        double* values = malloc(count * sizeof(double));

        /* Resetting the file pointer to the beginning of the file */
        rewind(file);
        
        /* Reading the file line by line */
        char line[actual_char_count];
        *num_lines = 0;
        while (fgets(line, sizeof(line), file)) {
            int numValues = 0;

            /* Splitting the line with tab as the delimiter */
            token = strtok(line, "\t");
            while (token != NULL) {
                value = atof(token);
                values[numValues] = value;
                numValues++;

                token = strtok(NULL, "\t");
            }

            /* Allocating memory for the actual number of values per line */
            data[*num_lines].values = malloc(numValues * sizeof(double));
            memcpy(data[*num_lines].values, values, numValues * sizeof(double));
            data[*num_lines].numValues = numValues;
            (*num_lines)++;

            /* Checking if we have exceeded the maximum number of lines */
            if (*num_lines >= MAX_LINES) {
                break;
            }
        }

        /* Freeing the memory for the maximum number of values per line */
        free(values);
    }

    /* Closing the file */
    fclose(file);
}
