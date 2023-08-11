import os
import numpy as np
import tensorflow as tf
from keras.models import Model, load_model
import tensorflow.keras.backend as K

# Created:       01.08.2023
#   Last modified: 01.08.2023
#   @ Jukka J

# Loading the labels
labels = np.load("../../archive/mineral-spectra_denoised_norm/labels.npy")

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

model = load_model("CNN_450-2450-5-550_1110-11-110-111-000_20230605090251_new.h5", custom_objects=custom_objects)

# Getting model's weights
model_weights = model.get_weights()
# Saving the model's weights to a .txt file in C arrays format
with open("model_weights.txt", "w") as f:
    for i, layer_weights in enumerate(model_weights):
        # For 1D weights (e.g., biases)
        if len(layer_weights.shape) == 1:
            f.write("const float layer{}_weights[{}] = {{".format(i, layer_weights.shape[0]))
            f.write(", ".join(map(str, layer_weights)))
            f.write("};\n")
        # For 2D weights (e.g., dense layer weights)
        elif len(layer_weights.shape) == 2:
            f.write("const float layer{}_weights[{}][{}] = {{\n".format(i, layer_weights.shape[0], layer_weights.shape[1]))
            for w in layer_weights:
                f.write("{")
                f.write(", ".join(map(str, w)))
                f.write("},\n")
            f.write("};\n")
        # For 3D weights (e.g., kernel/convolutional weights)
        elif len(layer_weights.shape) == 3:
            f.write("const float layer{}_weights[{}][{}][{}] = {{\n".format(i, layer_weights.shape[0], layer_weights.shape[1], layer_weights.shape[2]))
            for w in layer_weights:
                f.write("{")
                for row in w:
                    f.write("{")
                    f.write(", ".join(map(str, row)))
                    f.write("},\n")
                f.write("},\n")
            f.write("};\n")
        else:
            pass

print("Model weights saved to model_weights.txt")

