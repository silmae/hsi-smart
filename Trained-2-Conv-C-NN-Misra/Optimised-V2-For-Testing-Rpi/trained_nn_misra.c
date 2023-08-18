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
     
     double * input = four_test_images[0];
     int i_s = sizeof(four_test_images[0]) / sizeof(four_test_images[0][0]);

     /* Scen1 */
     scen1(input, i_s);

     /* Scen2 */
     /* scen2(input, i_s); */

     /* Scen3 */
     /* scen3(input, i_s); */

     /* Scen4 */
     /* scen4(input, i_s); */

     /* Scen5 */
     /* scen5(input, i_s); */

     /* Scen6 */
     /* scen6(input, i_s); */
    
    return 0;
    
}
