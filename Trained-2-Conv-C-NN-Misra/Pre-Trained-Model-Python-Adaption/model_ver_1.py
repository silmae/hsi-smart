import os
import numpy as np
import tensorflow as tf
from keras.models import Model, load_model
import tensorflow.keras.backend as K

# Created:       10.07.2023
# Last modified: 11.08.2023
# @ Jukka J

np.set_printoptions(suppress = True)

# Loading the labels
labels = np.load("../../archive/Mineral-Spectra-Denoised-Norm/labels.npy")

# For getting layer lutputs for the first line of test data
data = np.loadtxt("../../archive/mineral-spectra_denoised_norm_spectra.dat")
first_line_data = data[0:1, :]  # Select only the first line of the test data
print(first_line_data)

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

# Save weights to .txt file
weights_output_path = "dense_layer_weights.txt"

with open(weights_output_path, "w") as f:
    for layer in model.layers:
        if isinstance(layer, tf.keras.layers.Dense):
            weights = layer.get_weights()
            if len(weights) == 2:  # Check if weights exist for the layer
                weights_arr = weights[0]
                biases_arr = weights[1]

                # Save weights array
                f.write(f"// Weights for layer: {layer.name}\n")
                f.write(f"float {layer.name}_weights[] = {{\n")
                for row in weights_arr:
                    row_str = ", ".join([str(val) for val in row])
                    f.write(f"    {row_str},\n")
                f.write("};\n\n")

                # Save biases array
                f.write(f"// Biases for layer: {layer.name}\n")
                f.write(f"float {layer.name}_biases[] = {{\n")
                biases_str = ", ".join([str(val) for val in biases_arr])
                f.write(f"    {biases_str},\n")
                f.write("};\n\n")

print("Weights saved to", weights_output_path)

model.summary()
print(model.layers)

# Print Outputs of Each Layer
outputs = []
layer_names = []

input_data = first_line_data  # For transparency
for layer in model.layers:
    layer_names.append(layer.name)
    print("Output of layer: ", layer.name)
    print()

    intermediate_layer_model = Model(inputs=model.input, outputs=layer.output)
    intermediate_output = intermediate_layer_model.predict(input_data)
    outputs.append(intermediate_output)
    with np.printoptions(threshold=np.inf):
        print(intermediate_output)


with np.printoptions(threshold=np.inf):
    print("pnt")
    print(model.get_layer('conv1d_1').get_weights())

   
model.summary()
