#ifndef TRAINED_MODEL_H
#define TRAINED_MODEL_H

/* Created:       12.09.2023
   Last modified: 12.09.2023
   @ Jukka J jajoutzs@jyu.fi
*/

/* Gamma, beta, mean and variance, for BN1 */
extern double batch_normalization_1_p[PARAMS];

/* One array for gammas, betas, means and variances of BN2 */
extern double batch_normalization_2[PARAMS][C1KC_AND_C2IC];

/* One array for gammas, betas, means and variances of BN3 */
extern double batch_normalization_3[PARAMS][C1B_AND_C2KC];

/* Conv1d: */
extern double conv1d_bias[C1KC_AND_C2IC];
extern double conv1d_kernel[KS][C1IC][C1KC_AND_C2IC];

/* Conv1d_1: */
extern double conv1d_1_bias[C1B_AND_C2KC];
extern double conv1d_1_kernel[KS][C1KC_AND_C2IC][C1B_AND_C2KC];

/* Dense: */
extern double dense_bias[DLBC_AND_DLWD2];
extern double dense_weights[DLWD1_AND_DLIC][DLBC_AND_DLWD2];

#endif /* TRAINED_MODEL_H */
