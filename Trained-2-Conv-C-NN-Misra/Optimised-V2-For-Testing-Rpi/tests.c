#include <stdlib.h>
#include <unistd.h>
#include "tools.h"

/* Created:       17.08.2023
   Last modified: 17.08.2023
   @ Jukka J jajoutzs@jyu.fi
*/

/* Contains sequence of NN-function calls in sizeable loop */
void sequence(double * input, int count) {
    for (int i = 0; i < count; i++) {

        /*========== Function calling sequence: ==========================*/

        batch_normalization_1(input, i_s);
        double ** arr1 = convolution_layer_1(input, i_s);
        
        batch_normalization_2_and_3(arr1, i_s, 2);
        double ** arr2 = convolution_layer_2(arr1, i_s);

        int padding_start_and_end = (KS - 1) / 2;
        free_2D_array(arr1, (i_s + padding_start_and_end * 2));
        batch_normalization_2_and_3(arr2, i_s, 3);
        
        double* arr3 = flatten_layer(arr2, i_s);
        free_2D_array(arr2, i_s);
        
        double* arr4 = dense_layer(arr3, DLWD1_AND_DLIC);
        free(arr3);
        free(arr4);

    }
}

/* Single small set of images passed once */
void scen1(double * input, int i_s) {
    sleep(2.5);
    sequence(input, 10);
}

/* Single large set of images passed once, without sleep function */
void scen2(double * input, int i_s) {
    sequence(input, 50);
}

/* Single large set of images passed once */
void scen3(double * input, int i_s) {
    sleep(2.5);
    sequence(input, 100);
}

/* Single small set of images passed in intervals. In between passes,
   program is in "idle"-state
*/
void scen4(double * input, int i_s) {
    sleep(2);
    sequence(input, 10);
    sleep(2);
    sequence(input, 10);
    sleep(2);
    sequence(input, 10);
    sleep(2);
    sequence(input, 10);
}

/* Single set of images, with growing image-counts, passed in intervals.
   In between passes, program is in "idle"-state
*/
void scen5(double * input, int i_s) {
    sleep(2);
    sequence(input, 10);
    sleep(2);
    sequence(input, 50);
    sleep(2);
    sequence(input, 100);
    sleep(2);
    sequence(input, 200);
}