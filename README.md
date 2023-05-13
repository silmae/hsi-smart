# hsi-smart



## High-level operation principles, demonstrating how trained Tensorflow-NN can be implented in C++:

**1)** Python program (/Trained-NN-Python/**Simple_NN.py**) trains Tensorflow-NN with mnist-dataset, and saves weight and biases accordingly for each layer, to:
- "/Cpp-Implementation/1st_Hidden"
- "/Cpp-Implementation/2nd_Hidden"
- "/Cpp-Implementation/3rd_Hidden"
- "/Cpp-Implementation/Output-Layer"

Program also saves output-values/predictions for each image (in this case 10 first) to:
- "/Cpp-Implementation/Tensorflow-Output-Values".

This is for comparisons in later phase.

- Saving format for all files: .csv (comma separated values)

## Structure of NN:

     --------------------------------------------
    |                Flatten Layer               |
    |                Input shape: (28, 28)       |
    |                Output shape: (784,)        |
     --------------------------------------------
                          |
                          |
     --------------------------------------------
    |                Dense Layer 1               |
    |              Number of neurons: 128        |
    |            Activation Function: ReLU       |
     --------------------------------------------
                          |
                          |
     --------------------------------------------
    |                Dense Layer 2               |
    |              Number of neurons: 64         |
    |            Activation Function: ReLU       |
     --------------------------------------------
                          |
                          |
     --------------------------------------------
    |                Dense Layer 3               |
    |              Number of neurons: 32         |
    |            Activation Function: ReLU       |
     --------------------------------------------
                          |
                          |
     --------------------------------------------
    |                Dropout Layer               |
    |                Rate: 0.2                   |
     --------------------------------------------
                          |
                          |
     --------------------------------------------
    |                Dense Layer 4               |
    |              Number of neurons: 10         |
    |           Activation Function: Softmax     |
     --------------------------------------------

**2)** Python program (/Trained-NN-Python/**MNIST_Image_Saver.py**) is used for saving given amount of images (10), from mnist-dataset to:

- "/Cpp-Implementation/Test-Data"

**3)** C++ program (/Cpp-Implementation/**Simple_NN.cpp**) reads contents (weights, biases and test data) from previously mentioned locations.

**4)** C++ program applies corresponding weights, biases and activation-function for input data on current layer and passes output of
that layer to next. Following layer performs similar operations.

**5)** Program saves output-values (predictions for each mnist image, passed to NN as .csv) to "/Cpp-Implementation/Output-Data".

**6)** Optional: output values from tensorflow and Cpp-implementation (of same NN-structure) can be compared via correlation, by using
"/Correlation-Plotter-Python/**plotter.py** 
