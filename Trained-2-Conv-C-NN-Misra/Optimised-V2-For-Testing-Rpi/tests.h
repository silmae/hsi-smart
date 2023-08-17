#ifndef TESTS_H
#define TESTS_H

/* Created:       17.08.2023
   Last modified: 17.08.2023
   @ Jukka J jajoutzs@jyu.fi
*/

/* Contains sequence of NN-function calls in sizeable loop */
void sequence(double * input, int i_s, int count);

/* Single small set of images passed once */
void scen1(double * input, int i_s);

/* Single large set of images passed once, without sleep function */
void scen2(double * input, int i_s);

/* Single large set of images passed once */
void scen3(double * input, int i_s);

/* Single small set of images passed in intervals. In between passes,
   program is in "idle"-state
*/
void scen4(double * input, int i_s);

/* Single set of images, with growing image-counts, passed in intervals.
   In between passes, program is in "idle"-state
*/
void scen5(double * input, int i_s);

#endif