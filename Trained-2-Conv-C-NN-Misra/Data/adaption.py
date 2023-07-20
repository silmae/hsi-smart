import os
import numpy as np
import tensorflow as tf
from keras.models import Model, load_model
import tensorflow.keras.backend as K

# Created:       02.06.2023
# Last modified: 17.07.2023
# Pre-tained-model: @ David Korda
# Adaption: @ Jukka J jajoutzs@jyu.fi


# Allows printing all the data
np.set_printoptions(suppress = True)

# Loading the labels
labels = np.load("../Data/Mineral-Spectra-Denoised-Norm/labels.npy")

endmembers_counts = [2, 2, 3, 0]
num_minerals = 3

def gimme_indices(num_minerals: int, endmembers_counts: np.ndarray, reduced: bool = True, return_mineral_indices: bool = False) -> np.ndarray:
    # This function returns first and last indices of modal/mineral groups
    indices = np.zeros((len(endmembers_counts) + 1, 3), dtype=int)
    indices[:, 0] = np.arange(-1, len(endmembers_counts))
    indices[0, 1:] = 0, num_minerals
    for k, counts in enumerate(endmembers_counts):
        indices[k + 1, 1:] = indices[k, 2], indices[k, 2] + counts
    if reduced:
        indices = np.array([[ind_of_mineral, start, stop] for ind_of_mineral, start, stop in indices if start != stop])
    if return_mineral_indices:
        return indices
    return indices[:, 1:]

# Custom sigmoid
@tf.function
def my_sigmoid(x):
    x_new = tf.zeros_like(x[:, 0:0])
    for start, stop in gimme_indices(num_minerals, endmembers_counts):
        tmp = tf.sigmoid(x[..., start:stop])
        tmp /= (tf.reduce_sum(tmp, axis=1, keepdims=True) + tf.keras.backend.epsilon())  # normalization to unit sum
        x_new = tf.concat([x_new, tmp], axis=1)
    return x_new

dummy = lambda x: x

custom_objects = {"loss": dummy, "my_sigmoid": my_sigmoid, "my_softmax": dummy, "mse": dummy,
                  "rmse": dummy, "quantile": dummy, "mae": dummy, "Lp_norm": dummy,
                  "r2": dummy, "sam": dummy, "sse": dummy}

model = load_model("../Data/CNN_450-2450-5-550_1110-11-110-111-000_20230605090251_new.h5", custom_objects=custom_objects)

print(model.layers)

# Getting Layer Outputs for the First Line of Test Data (first image of dataset)
data = np.loadtxt("../Data/Mineral-Spectra-Denoised-Norm/mineral-spectra_denoised_norm_spectra.dat")
first_line_data = data[0:1, :]  # Selec# Allows printing all the data
np.set_printoptions(suppress = True)
print(first_line_data)

# Intermediate outputs of NN
outputs = []
layer_names = []
input_data = first_line_data  # For transparency
for layer in model.layers:
    layer_names.append(layer.name)
    intermediate_layer_model = Model(inputs=model.input, outputs=layer.output)
    intermediate_output = intermediate_layer_model.predict(input_data)
    outputs.append(intermediate_output)

# Display Outputs of chosen layers
for name, output in zip(layer_names, outputs):
    if name.startswith("conv") or name.startswith("act"):
        print(f"Output of layer '{name}':")
        with np.printoptions(threshold=np.inf):  # Print the entire array
            print(output)
        print()


