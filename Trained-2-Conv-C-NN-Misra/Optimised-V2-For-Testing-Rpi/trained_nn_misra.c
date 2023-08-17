#include "tools.h"
#include "tests.h"
#include "inputs_for_testing.h" /* TODO: remove import and header, if implented with hardware */
#include <stdlib.h>

/* Created:       25.05.2023
   Last modified: 17.08.2023
   @ Jukka J jajoutzs@jyu.fi
*/

/* Example of usage, test data is read, and passed through network: */

int main() {

     /* Scen1 */
     Scen1(four_test_images);

     /* Scen2 */
     /* Scen2(four_test_images); */

     /* Scen3 */
     /* Scen3(four_test_images); */

     /* Scen4 */
     /* Scen4(four_test_images); */

     /* Scen5 */
     /* Scen5(four_test_images); */
        
    /* Looping through (1st image only in this case) test data, and passing data through NN */

    for (int i = 0; i < 1; i++) {

        /* Single image as input, array is implicitly converted to a pointer
           => no need for an explicit cast
        */
        double * input = four_test_images[0];

        /* Determine size of the input dynamically like follows, or determine it in tools.h
           as IMG_VAL_COUNT and use it:
           e.g:
           int i_s = IMG_VAL_COUNT;
        */
        
        /* TODO: adjust to match actual input */
        /* Calculating dynamically size of the input */
        int i_s = sizeof(four_test_images[0]) / sizeof(four_test_images[0][0]);

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
        
        /*
        for (int i = 0; i < DLBC_AND_DLWD2; i++) {
            printf("%.8lf\n", arr4[i]);
        }
        */
        
        /* Freeing allocated memory */
        free(arr4);
        
        /* ================================================================ */
    }
    
    return 0;
    
}
