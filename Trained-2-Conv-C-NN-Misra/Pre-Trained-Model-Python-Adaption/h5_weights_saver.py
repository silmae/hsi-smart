import os
import numpy as np
import tensorflow as tf
import csv
from keras.models import Model, load_model

# Created:       25.05.2023
# Last modified: 29.06.2023
# Pre-trained model by: @ David Korda D.Korda@seznam.cz
# Adaption: @ Jukka J jajoutzs@jyu.fi

# Loading the labels
labels = np.load("../archive/mineral-spectra_denoised_norm/labels.npy")

#=============== Model loading start =============================================================================================================
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

model = load_model("../archive/CNN_450-2450-5-550_1110-11-110-111-000_20230605090251_new.h5", custom_objects=custom_objects)
#=============== Model loading end =============================================================================================================

# Create a directory to store the weights files if it doesn't exist
weights_dir = "weights"
os.makedirs(weights_dir, exist_ok=True)

# Save the weights of each layer to separate CSV files
for layer in model.layers:
    if layer.name.startswith("conv") or layer.name.startswith("dense"):
        
        print(layer.name)

        #========= Looping over bias =======================================
        bias_dims = tuple(int(dim) for dim in layer.bias.shape)        
        print(bias_dims)
       
        layer_dir = os.path.join(weights_dir, layer.name)
        os.makedirs(layer_dir, exist_ok=True)
        
        dataset_name_bias = layer.name + "_bias"       
        filename = os.path.join(layer_dir, f"{dataset_name_bias}.csv")
        bias_values = layer.bias.numpy()  # Convert to numpy array
        
        with open(filename, 'w', newline='') as csvfile:
            writer = csv.writer(csvfile)
            writer.writerow(bias_values)
        
        #========= Looping over kernels =====================================
        kernel_dims = tuple(int(dim) for dim in layer.kernel.shape)  
        print(kernel_dims)
        dataset_name_kernel = layer.name + "_kernels"
        kernel_values = layer.kernel.numpy()
        
        if len(kernel_dims) == 3:
            for c in range(kernel_dims[0]):
                filename = os.path.join(layer_dir, f"{dataset_name_kernel}_index_{c}.csv")
                with open(filename, 'a', newline='') as csvfile:
                    writer = csv.writer(csvfile)
                    for d in range(kernel_dims[1]):
                        single_line_values = []
                        for e in range(kernel_dims[2]):
                            single_line_values.append(kernel_values[c][d][e])
                        writer.writerow(single_line_values)
                    
        if len(kernel_dims) == 2:
            filename = os.path.join(layer_dir, f"{layer.name}_weights.csv")
            # Using append mode to avoid replacing previous populations
            with open(filename, 'a', newline='') as csvfile:
                writer = csv.writer(csvfile)
                for c in range(kernel_dims[0]):
                    single_line_values_2D = []
                    for d in range(kernel_dims[1]):
                        single_line_values_2D.append(kernel_values[c][d])
               
                writer.writerow(single_line_values_2D)
