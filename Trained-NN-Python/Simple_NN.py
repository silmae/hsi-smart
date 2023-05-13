import tensorflow as tf
import numpy as np
import os
import csv
import tensorflow.keras.utils
from tensorflow.keras.callbacks import TensorBoard
# This file contains NN implementation for trained NN, i.e program assumes calculated weights 
# Created:       05.05.2023
# Last modified: 12.05.2023
# @ Jukka J

# Defining callback function for TensorBoard (to save logs)
log_dir = "./logs/"
tensorboard_callback = TensorBoard(log_dir=log_dir, histogram_freq=1)


# Loading of MNIST dataset
# 60k images in the training set and 10k images in the test set. 
# mnist = tf.keras.datasets.mnist
os.chdir('/home/jukka/HSI-Smart/archive')

# np.genfromtxt() reads in the header line of the CSV file as well, which adds an extra row to the resulting arrays.
# Therefore header should be skipped
mnist_train =  np.genfromtxt('mnist_train.csv', delimiter=',', skip_header=1)
mnist_test =  np.genfromtxt('mnist_test.csv', delimiter=',', skip_header=1)

os.chdir('/home/jukka/HSI-Smart/Trained-NN-Python')
# Data splitting
# Split the datasets into input features and labels:

# 1 Select all rows in the array and all columns except the first column. 
# 2 select all rows in the array and only the first 
x_train, y_train = mnist_train[:, 1:], mnist_train[:, 0]
x_test, y_test = mnist_test[:, 1:], mnist_test[:, 0]

# The Sequential model expects input with shape (None, 28, 28)
# Shaped to 3D tensor where the first dimension is the varying batch size and then the height and width of the input image
x_train = x_train.reshape((-1, 28, 28))
x_test = x_test.reshape((-1, 28, 28))

print("Shapes:")
print(x_train.shape)
print(x_test.shape)
print(y_train.shape)
print(y_train.shape)

"""
Normalize the pixel values of the image data:

Integer values ranging from 0 to 255. Dividing
each pixel value by 255.0 scales the pixel values
to be between 0 and 1.0.
"""
x_train, x_test = x_train / 255.0, x_test / 255.0

print("\nDataset train:\n")
print(mnist_train)

print("Dataset test:\n")
print(mnist_test)

print("Dataset loaded")

# MNIST dataset as a tuple of NumPy arrays
# (x_train, y_train), (x_test, y_test) = mnist.load_data()

#=====Layers=====:

# Flattens the input shape of (28, 28) to a one-dimensional array of 784 (28x28) pixels.
# Fully connected layer with 128 units that uses the ReLU activation function.
# Randomly drop out 20% of the input units during training to prevent overfitting.
# Output layer with 10 units, representing the 10 possible digit classes (0 to 9).

ml_model = tf.keras.models.Sequential([
  tf.keras.layers.Flatten(input_shape=(28,28)),  # Layer to convert 2D data to 1D. The layer has 128*784+128 = 100,480 parameters to learn.
  tf.keras.layers.Dense(128, activation='relu'), # 1st hiddenlayer, relu as activation func, 128 neurons. The layer has 64*128+64 = 8,256 parameters to learn.
  tf.keras.layers.Dense(64, activation='relu'),  # 2nd hiddenlayer, relu as activation func, 64 neurons. The layer has 32*64+32 = 2,080 parameters to learn.
  tf.keras.layers.Dense(32, activation='relu'),  # 3rd layer, relu as activation func, 32 neurons. 
  tf.keras.layers.Dropout(0.2),                  # Sets 20% of the input units to 0 at each update during training, which helps prevent overfitting. This layer does not have any learnable parameters
  tf.keras.layers.Dense(10, activation='softmax')   # Output layer with 10 neurons, one for each class in the MNIST dataset. This layer has 32*10+10 = 330 parameters to learn.
])


# Vector of logits 
predictions = ml_model(x_train[:1]).numpy()

# Logits to probabilities
#tf.nn.softmax(predictions).numpy()

# Loss function for training
loss_fn = tf.keras.losses.SparseCategoricalCrossentropy(from_logits=True)

# The loss function takes a vector of ground truth values and a vector of logits and returns a scalar loss for each example
# (and output to numpy array)
loss_fn(y_train[:1], predictions).numpy()

# Configuring and compiling the model:
# Class for optimization: adam
# Loss function to be used: own
# Metrics to be evaluated, accuracy
 
ml_model.compile(optimizer='adam',
              loss=loss_fn,
              metrics=['accuracy'])
              
ml_model.summary()

# Iterations of entire dataset set to 5
ml_model.fit(x_train, y_train, epochs=5, callbacks=[tensorboard_callback])

#======================Saving weights and biases of each layer in .csv format ===============================
print(ml_model.layers)

# Get the weights of the model
weights = ml_model.get_weights()
print(weights)

name_array_weights = ["1st_hidden_weights.csv", "2nd_hidden_weights.csv", "3rd_hidden_weights.csv", "output_layer_weights.csv"]
name_array_biases = ["1st_hidden_bias.csv", "2nd_hidden_bias.csv", "3rd_hidden_bias.csv", "output_layer_bias.csv"]
name_array_folders = ["1st-Hidden", "2nd-Hidden", "3rd-Hidden", "Output-Layer"]
output_dir = "../Cpp-Implementation"

print()
# Loop through the layers and save the weights and biases in separate csv files
for i, (w, b) in enumerate(zip(weights[::2], weights[1::2])):
    folder_path = os.path.join(output_dir, name_array_folders[i])
    folder_path
    if not os.path.exists(folder_path):
        os.makedirs(folder_path)
    weights_file_path = os.path.join(folder_path, name_array_weights[i])
    biases_file_path = os.path.join(folder_path, name_array_biases[i])
    np.savetxt(weights_file_path, w, delimiter=",")
    print(f"  Weights shape: {w.shape}")
    np.savetxt(biases_file_path, b, delimiter=",")
    print(f"  Biases shape: {b.shape}")


print("Weights and biases saved successfully!")

# Get the output of the last layer for the first 10 images in the test set
output = ml_model.layers[-1].output
output_func = tf.keras.backend.function([ml_model.input], [output])
output_values = output_func(x_test[:10])[0]

# Save output values to csv files
# Set precision of 10 numbers after decimal
# Define the output directory
output_dir = "../Cpp-Implementation/Tensorflow-Output-Values"

# Create the output directory if it does not exist
if not os.path.exists(output_dir):
    os.makedirs(output_dir)

# Loop through the output data and save it to separate csv files
for i, output_data in enumerate(output_values):
    output_filepath = os.path.join(output_dir, f"output_{i}.csv")
    if output_data.ndim == 1:
        np.savetxt(output_filepath, output_data.reshape(1, -1), delimiter=",", fmt="%.15f")
    else:
        np.savetxt(output_filepath, output_data, delimiter=",", fmt="%.15f,"*(output_data.shape[1]-1) + "%.15f")
    print(f"Output values for image {i} saved to {output_filepath} in csv format")

