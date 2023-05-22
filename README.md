# hsi-smart

Purpose of this entity is to demonstrate, how trained Tensorflow-NN can be implented in C++

## High-level operation principles:

**1)** Python program (/Trained-NN-Python/**Simple_NN.py**) trains Tensorflow-NN with mnist-dataset, and saves weight and biases accordingly for each layer, to:
- "/Cpp-Implementation/1st_Hidden"
- "/Cpp-Implementation/2nd_Hidden"
- "/Cpp-Implementation/3rd_Hidden"
- "/Cpp-Implementation/Output-Layer"

Program also saves:

output-values/predictions for each image (in this case 10 first) to:
- "/Cpp-Implementation/Tensorflow-Output-Values".

10 First images in Flatten state, to:
- "/Cpp-Implementation/Test-Data"

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


**3)** C++ program (/Cpp-Implementation/**Simple_NN.cpp**) reads contents (weights, biases and test data) from previously mentioned locations.

**4)** C++ program applies corresponding weights, biases and activation-function for input data on current layer and passes output of
that layer to next. Following layer performs similar operations.

For each neuron, is applied principle of: y = activation(w * x + b), where:

* y stands for the output of a single neuron
* x represents the input data
* b means bias term
* w represents the weights
* activation function applies a non-linear transformation to the output of the linear operation (w * x + b)

For output-layer instead, is applied: y = Softmax(x)

* Where x is given an input vector (x = [x_1, x_2, ..., x_n])
* Where y is output vector

**5)* Output values from tensorflow and Cpp-implementation (of same NN-structure) are compared via correlation. This done as follows:
* After calculating values for current layer, Cpp-program calls python script ("/Correlation-Plotter-Python/**plotter.py**"), and passes those values and Tensorflow's corresponding layer's output to that script.
* Script creates comparing figure and saves that as image in same folder as script is located.
* Lastly, script opens figure. Cpp-program continues to next layer 


**6)** Program saves output-values (predictions for each mnist image, passed to NN as .csv) to "/Cpp-Implementation/Output-Data".
