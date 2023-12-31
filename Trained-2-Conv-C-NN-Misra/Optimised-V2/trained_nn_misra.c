#include "tools.h"
#include <stdlib.h>

/* Created:       25.05.2023
   Last modified: 15.08.2023
   @ Jukka J jajoutzs@jyu.fi
*/

/* Example of usage, test data is read, and passed through network: */

int main() {
    
    /* Looping through (1st image only in this case) test data, and passing data through NN */

    for (int i = 0; i < 1; i++) {

        /* Use single image as input, array can implicitly converted to a pointer
           => no need for an explicit cast
        */
        /* E.g: */
        double * input =  <your array of doubles here>;

        /* Determine size of the input dynamically like follows, or determine it in tools.h
           as IMG_VAL_COUNT and use it:
           e.g:
           int i_s = IMG_VAL_COUNT;
        */
        
        /* TODO: adjust to match actual input data structure
           Here, array of double arrays is used
        */
        /* Calculating dynamically size of the input */
        int i_s = sizeof(<your array here>[0]) / sizeof(<your array here>[0][0]);

        /*========== Function calling sequence: ==========================*/
        
        /* --------- First normalization layer --------- */
        batch_normalization_1(input, i_s);

        /* --------- First convolution layer ----------- */
        double ** arr1 = convolution_layer_1(input, i_s);
        
        /* --------- Second normalization layer -------- */
        batch_normalization_2_and_3(arr1, i_s, 2);

        /* --------- Second convolution layer ---------- */
        double ** arr2 = convolution_layer_2(arr1, i_s);
        /* Freeing allocated memory */
        int padding_start_and_end = (KS - 1) / 2;
        free_2D_array(arr1, (i_s + padding_start_and_end * 2));
        
        /* --------- Third normalization layer --------- */
        batch_normalization_2_and_3(arr2, i_s, 3);
        
        /* --------- Flatten layer --------------------- */
        double* arr3 = flatten_layer(arr2, i_s);
        /* Freeing allocated memory */
        free_2D_array(arr2, i_s);
        
        /* --------- Dense layer ----------------------- */
        double* arr4 = dense_layer(arr3, DLWD1_AND_DLIC);
        /* Freeing allocated memory */
        free(arr3);
        
        /* TODO: do something with produced predictions (for single image) */
        
        /* Freeing allocated memory */
        free(arr4);
        
        /* ================================================================ */
    }
    
    return 0;
    
}
