import numpy as np
from tensorflow.keras.datasets import mnist
from PIL import Image

# This file contains NN implementation for trained NN, i.e program assumes calculated weights 
# Created:       07.05.2023
# Last modified: 12.05.2023
# @ Jukka J

# Load the MNIST dataset
(x_train, y_train), (x_test, y_test) = mnist.load_data()

print(y_test[0])

# Select an image from the dataset
for i in range(0,10):
    img = x_test[i]
    # Resize the image to a 128x1 shape
    img_resized = np.array(Image.fromarray(img).resize((128, 1)))

    # Flatten the resized image into a 1D array
    img_flat = img_resized.flatten()
    f_name = "../Cpp-Implementation/Test-Data/mnist_img_" + str(i) + ".csv";
    print_name = "mnist_img_" + str(i) + ".csv"
    print(print_name)
    # Save the flattened image as a CSV file
    np.savetxt(f_name, img_flat, delimiter=",")

