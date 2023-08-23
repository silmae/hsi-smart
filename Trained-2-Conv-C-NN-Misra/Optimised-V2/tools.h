#ifndef TOOLS_H
#define TOOLS_H
#include <stdio.h>

#define C1IC 1 /* Input channels of the first convolution */
#define KS 5 /* Filter size */
#define C1B_AND_C2KC 8 /* Bias and kernel count of the second convolution */
#define C1KC_AND_C2IC 24 /* Bias and kernel count of the first convolution and the number of the input channels, of second convolution */ 
#define DLBC_AND_DLWD2 10 /* Dense layer's bias size and weight's second dimension */
#define DLWD1_AND_DLIC 3208 /* Dense layer's weight's first dimension and count of input channels */
#define PARAMS 4 /* Number of Batch-normalization parameters */

/* Created:       02.06.2023
   Last modified: 13.08.2023
   @ Jukka J jajoutzs@jyu.fi
*/

double relu(double value);
double sigmoid(double value);
void normalized_sigmoid(double** input, int size);
double single_value_batch_normalization(double value, double gamma, double beta, double mean, double variance);
void batch_normalization_1(double * input, int i_s);
void batch_normalization_2_and_3(double ** input, int i_s, int which_bn);
double ** convolution_layer_1(double * input, int i_s);
double ** convolution_layer_2(double ** input, int i_s);
double* flatten_layer(double** input, int nn_input_size);
double* dense_layer(double* input, int input_size);

void free_2D_array(double **array, int rows);
#endif
