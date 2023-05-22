import tensorflow as tf
import numpy as np
import os
import csv
from tensorflow.keras.callbacks import TensorBoard, Callback
from PIL import Image

# This file contains NN implementation for trained NN, i.e program assumes calculated weights 
# Created:       05.05.2023
# Last modified: 22.05.2023
# @ Jukka J

# Defining callback function for TensorBoard (to save logs)
# log_dir = "./logs/"
# tensorboard_callback = TensorBoard(log_dir=log_dir, histogram_freq=1)

output_dir_test_data = "./Test-Data"

#  Class to store the layer outputs during training in the layer_outputs attribute.
class SaveLayerOutputsCallback(Callback):
    def __init__(self, model, num_images, validation_data):
        super(SaveLayerOutputsCallback, self).__init__()
        self.model = model
        self.num_images = num_images
        self.validation_data = validation_data
        self.layer_outputs = []

    def on_epoch_end(self, epoch, logs=None):
        output_func = tf.keras.backend.function([self.model.input], [layer.output for layer in self.model.layers])
        outputs = output_func(self.validation_data[0][:self.num_images])
        outputs_np = [np.array(output) for output in outputs]  # Convert TensorFlow tensors to NumPy arrays
        self.layer_outputs.append(outputs_np)

        # Save output values to CSV files:
  
        output_dir = "./Tensorflow-Layer-Outputs"
        if not os.path.exists(output_dir):
            os.makedirs(output_dir)

        for layer_index, layer_output in enumerate(outputs_np):
            layer_name = f"layer_{layer_index + 1}"
            for image_index, image_output in enumerate(layer_output):
                image_name = f"image_{image_index + 1}"
                output_filename = f"{layer_name}_{image_name}_output.csv"
                output_filepath = os.path.join(output_dir, output_filename)
                np.savetxt(output_filepath, image_output, delimiter=",")
                print(f"Output values for {layer_name}, {image_name} saved to {output_filepath} in CSV format.")
                
                if layer_index == 0:
                    output_filename = f"mnist_img_{image_index}.csv"
                    output_filepath_test_data = os.path.join(output_dir_test_data, output_filename)
                    np.savetxt(output_filepath_test_data, image_output, delimiter=",")

# Loading of MNIST dataset
# 60k images in the training set and 10k images in the test set.

# Change directory to "archive"
os.chdir(os.path.join(os.getcwd(), '..', 'archive'))

# np.genfromtxt() reads in the header line of the CSV file as well, which adds an extra row to the resulting arrays.
# Therefore header should be skipped
mnist_train =  np.genfromtxt('mnist_train.csv', delimiter=',', skip_header=1)
mnist_test =  np.genfromtxt('mnist_test.csv', delimiter=',', skip_header=1)


# Change directory back to "Trained-NN-Python"
os.chdir(os.path.join(os.getcwd(), '..'))

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

# Configuring and compiling the model:
# Class for optimization: adam
# Loss function to be used: own
# Metrics to be evaluated, accuracy
 
ml_model.compile(optimizer='adam',
              loss='sparse_categorical_crossentropy',
              metrics=['accuracy'])

ml_model.summary()


# Change directory to "Cpp-Implementation"
os.chdir(os.path.join(os.getcwd(), 'Cpp-Implementation'))
current_folder = os.getcwd()
print("Current folder:", current_folder)

layer_output_callback = SaveLayerOutputsCallback(ml_model, num_images=10, validation_data=(x_test, y_test))

# Iterations of entire dataset set to 15
ml_model.fit(x_train, y_train, epochs=15, validation_data=(x_test, y_test), callbacks=[layer_output_callback])


# Get the output of each layer for the first 10 images in the test set
#output_values = [layer_output_callback.layer_outputs[layer_index][0][:10] for layer_index in range(len(layer_output_callback.layer_outputs))]
# Save output values to CSV files

output_dir = "./Tensorflow-Output-Values"

# Create the output directory if it does not exist
if not os.path.exists(output_dir):
    os.makedirs(output_dir)

#======================Saving weights and biases of each layer in .csv format ===============================
print(ml_model.layers)

# Get the weights of the model
weights = ml_model.get_weights()
# Loop through the layers and print the weights and biases
for i, (w, b) in enumerate(zip(weights[::2], weights[1::2])):
    print(f"Layer {i + 1}:")
    print("Weights:")
    print(w)
    print("Biases:")
    print(b)
    print()


name_array_weights = ["1st_hidden_weights.csv", "2nd_hidden_weights.csv", "3rd_hidden_weights.csv", "output_layer_weights.csv"]
name_array_biases = ["1st_hidden_bias.csv", "2nd_hidden_bias.csv", "3rd_hidden_bias.csv", "output_layer_bias.csv"]
name_array_folders = ["1st-Hidden", "2nd-Hidden", "3rd-Hidden", "Output-Layer"]
output_dir = os.getcwd()

# Loop through the layers and save the weights and biases in separate csv files
for i, (w, b) in enumerate(zip(weights[::2], weights[1::2])):
    folder_path = os.path.join(output_dir, name_array_folders[i])
    folder_path
    if not os.path.exists(folder_path):
        os.makedirs(folder_path)
    weights_file_path = os.path.join(folder_path, name_array_weights[i])
    biases_file_path = os.path.join(folder_path, name_array_biases[i])
    np.savetxt(weights_file_path, w, delimiter=",")
    print(f"Weights shape: {w.shape}")
    np.savetxt(biases_file_path, b, delimiter=",")
    print(f"Biases shape: {b.shape}")
print("Weights and biases saved successfully!")
