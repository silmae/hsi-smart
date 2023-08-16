#include "tools.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "trained_model.h"

/* Created:       02.06.2023
   Last modified: 14.08.2023
   @ Jukka J jajoutzs@jyu.fi
*/

/* Function to free dynamically allocated memory, for 2D structures */
void free_2D_array(double **array, int rows) {
    for (int i = 0; i < rows; i++) {
        free(array[i]);
    }
    free(array);
}

/* Function to perform batch normalizations on root-level */
double single_value_batch_normalization(double value, double gamma, double beta, double mean, double variance) {
    double epsilon = 1e-3;
    return (value - mean) / sqrt(variance + epsilon) * gamma + beta;
}

/* Batch-normalization-layer for input data */
void batch_normalization_1(double * input, int i_s) {
    for (int i = 0; i < i_s; i++) {
        input[i] = single_value_batch_normalization(input[i], batch_normalization_1_p[0], batch_normalization_1_p[1], batch_normalization_1_p[2], batch_normalization_1_p[3]);
    }
}

/* Two last batch normalizations, of NN (both have similar shape)
   Important note: input array (output of conv1d) contains zero padding, this function acknowledges this
*/
void batch_normalization_2_and_3(double ** input, int i_s, int which_bn) {
        
            /* BN2 */
            if (which_bn == 2) {
                /* Iterating over inputs, except zero-padding */
                for (int j = 2; j <= (i_s - 1 + KS / 2); j++) {
                    /* Over kernel count */
                    for (int i = 0; i < C1KC_AND_C2IC; i++) {

                        input[j][i] = single_value_batch_normalization(input[j][i], batch_normalization_2[0][i], batch_normalization_2[1][i], batch_normalization_2[2][i], batch_normalization_2[3][i]);
                    }
            
                }
            }
            /* BN3 */
            else {
                /* Over inputs */
                for (int j = 0; j < i_s; j++) {
                    /* Over kernel count */
                    for (int i = 0; i < C1B_AND_C2KC; i++) {
                        input[j][i] = single_value_batch_normalization(input[j][i], batch_normalization_3[0][i], batch_normalization_3[1][i], batch_normalization_3[2][i], batch_normalization_3[3][i]);
                    }
                }
            }
}

/* ReLu activation function */
double relu(double value) {
    return (value > 0) ? value : 0;
}

/* Regular sigmoid to be called by normalized sigmoid */
double sigmoid(double value) {
    return 1 / (1 + exp(-value));
}

/* Normalized sigmoid */
void normalized_sigmoid(double** input, int size) {

    double sum = 0.0;
    
    if (size >= 8 && size % 2 == 0) {
        /* 3 first values */
        for (int i = 0; i < 3; i++) {
            double sig = sigmoid((*input)[i]);
            (*input)[i] = sig;
            sum += sig;
        }
        (*input)[0] /= sum;
        (*input)[1] /= sum;
        (*input)[2] /= sum;

        /* Normalizing the values between, as pairs */
        for (int i = 3; i < size - 3; i += 2) {
            sum = 0.0;
            double sig1 = sigmoid((*input)[i]);
            double sig2 = sigmoid((*input)[i + 1]);
            sum += sig1 + sig2;
            (*input)[i] = sig1 / sum;
            (*input)[i + 1] = sig2 / sum;
        }

        /* Last 3 values */
        sum = 0.0;
        for (int i = size - 3; i < size; i++) {
            double sig = sigmoid((*input)[i]);
            (*input)[i] = sig;
            sum += sig;
        }
        (*input)[size-3] /= sum;
        (*input)[size-2] /= sum;
        (*input)[size-1] /= sum;

    }
}

/* First convolution-layer operations */
double ** convolution_layer_1(double * input, int i_s) {

    /* Padding of NN => same, as zeroes, therefore, expansion of input array is
       needed.
    */
    
    /* For transparency */
    int padding_start_and_end = (KS - 1) / 2;
    int padded_size = (i_s + padding_start_and_end * 2);
    int m_index = padding_start_and_end + 1;

    /* Creating dynamically 2D output array */
    /* 405 24 */
    double ** output = (double ** ) malloc(padded_size * sizeof(double * ));
    for (int i = 0; i < padded_size; i++) {
        output[i] = (double * ) malloc(C1KC_AND_C2IC * sizeof(double));
    }

    /* Temporary, padded input array */
    double * padded_input = (double * ) malloc(padded_size * sizeof(double));

    /* Copying elements with offset, into the padded input array */
    memcpy(padded_input + padding_start_and_end, input, i_s * sizeof(double));
    
    for (int i = 0; i < 2; i++) {

        /* Assigning zero padding */
        for (int j = 1; j < (KS + 1); j++) {
            if (i == 0) {
                if (0 < j && j < m_index) {
                    for (int k = 0; k < C1KC_AND_C2IC; k++) {
                        output[j - 1][k] = 0.0;
                    }
                }
            } else {
                if (m_index < j && j < (KS + 1)) {
                    for (int k = 0; k < C1KC_AND_C2IC; k++) {
                        output[padded_size - j - m_index][k] = 0.0;
                    }
                }
            }
        }
    }

    /* Iterating over kernel count */
    for (int h = 0; h < C1KC_AND_C2IC; h++) {
        /* Iterating over input channels, is one with the first convolution of this model,
           so this is mainly for transparency
        */
        for (int c = 0; c < C1IC; c++) {

            /* Iterating over inputs */
            for (int i = 0; i < i_s; i++) {
                double sum = 0.0;
                /* Iterating through the kernel */
                for (int j = 0; j < KS; j++) {
                    sum += padded_input[i + j] * conv1d_kernel[j][c][h];
                }
                /* Adding bias */
                sum += conv1d_bias[h];
                /* Performing activation an populating the corresponding index of
                   the output array with the result of calculations.
                */
                /* Performing activation and populating output array */
                output[i + padding_start_and_end][h] = relu(sum);
            }
        }
    }
    
    /* Freeing temporary array */
    free(padded_input);
    return output;
}

/* Second convolution-layer operations */
double ** convolution_layer_2(double ** input, int i_s) {

    int padding_start_and_end = (KS - 1) / 2;
    int padded_size = (i_s + padding_start_and_end * 2);
    int m_index = padding_start_and_end + 1;
    
    /* Creating dynamically 2D output array */
    /* 401 8 */
    double ** output = (double **)malloc(i_s * sizeof(double *));
    for (int i = 0; i < i_s; i++) {
        output[i] = (double *)malloc(C1B_AND_C2KC * sizeof(double));
    }
    
    /* Padding of NN => same, input array contains padding.
       Spatial dimension are same, but all input kernels are 
       summed up to achieve channel reduction (24 -> 8).
    */
           
    /* Iterating over kernel count */
    for (int h = 0; h < C1B_AND_C2KC; h++) {
    
        /* Creating temporary array for calculations */
        /* 401 24 */
        double ** tmp = (double ** ) malloc(i_s * sizeof(double * ));
        for (int i = 0; i < i_s; i++) {
            tmp[i] = (double * ) malloc(1 * sizeof(double));
        }
        /* Iterating over input channels */
        for (int c = 0; c < C1KC_AND_C2IC; c++) {
            for (int i = 0; i < (padded_size - (padding_start_and_end * 2)); i++) {
                double sum = 0.0;
                for (int j = 0; j < KS; j++) {
                    sum += input[i + j][c] * conv1d_1_kernel[j][c][h];
                }
                /* Channel reduction: */
                tmp[i][0] += sum;

            }
        }

        /* Tmp contains now kernels sum -> populating output and applying activation */
        for (int x = 0; x < i_s; x++) {
            output[x][h] = relu(tmp[x][0] + conv1d_1_bias[h]);
        }
        
        /* Freeing memory, allocated for the tmp */
        free_2D_array(tmp, i_s);
    }
    
    return output;
}

/* Function to convert kernel outputs to single 1D structure
   In, shape:   [401][8],  dimension may very
   Out, shape:  [3208], size may vary
*/
double * flatten_layer(double** input, int input_size) {

    double* output = (double*)malloc(DLWD1_AND_DLIC * sizeof(double));
    int ind = 0;

    for (int i = 0; i < input_size; i++) {
        for (int j = 0; j < C1B_AND_C2KC; j++) {
            output[ind] = input[i][j];
            ind++;
        }
    }
    return output;
}

/* Dense layer operations */
/* In, shape:   [3208], size may very
   Out, shape:  [10], fixed size
*/
double * dense_layer(double* input, int input_size) {

    double* output = (double*)malloc((DLBC_AND_DLWD2) * sizeof(double));

        for (int i = 0; i < DLBC_AND_DLWD2; i++) {
            double sum = 0.0;
            for (int j = 0; j < DLWD1_AND_DLIC; j++) { 
                sum += input[j] * dense_weights[j][i];
            }
            output[i] = sum + dense_bias[i];
        }

    /* Activation: */
    normalized_sigmoid(&output, DLBC_AND_DLWD2);

    return output;
}
