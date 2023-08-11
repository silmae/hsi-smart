#include <hdf5.h>
#include "datparser.h"
#include "tools.h"

/* Created:       25.05.2023
   Last modified: 11.08.2023
   @ Jukka J jajoutzs@jyu.fi
*/

/* Example of usage, test data is read, and passed through network: */

int main() {
    
    /* ---------- Loading data for testing start------------------------------------ */

    LineData data[1];
    int numLines = 0;
    const char* filename = "../archive/mineral-spectra_denoised_norm_spectra.dat";

    parseFile(filename, data, &numLines);
    
    /* ---------- Loading data for testing end -------------------------------------- */

    /* Looping through test data, and passing data through NN */

    for (int i = 0; i < 1; i++) {

        /* Single image as input, and corresponding size */
        double * input = data[i].values;
        int i_s = data[i].numValues;
        
        /*========== Function calling sequence: ==========================*/
        
        /* --------- First normalization layer --------- */
        Batch_normalization_1(input, i_s);

        /* --------- First convolution layer ----------- */
        double ** arr1 = convolution_layer_1(input, i_s);
            
        /* --------- Second normalization layer -------- */
        Batch_normalization_2_and_3(arr1, i_s, 2);

        /* --------- Second convolution layer ---------- */
        double ** arr2 = convolution_layer_2(arr1, i_s);
        
        /* --------- Third normalization layer --------- */
        Batch_normalization_2_and_3(arr2, i_s, 3);
        
        /* --------- Flatten layer --------------------- */
        double* arr3 = flatten_layer(arr2, i_s);
        
        /* --------- Dense layer ----------------------- */
        double* arr4 = dense_layer(arr3, DLWD1_AND_DLIC);
        

        /* ================================================================ */
    }
    
    return 0;
    
}
