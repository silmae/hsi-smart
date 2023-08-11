## What is this?:
This is C-implementation for trained 2-convolution neural network (Structure down below), that
produces predictions of mineral type of the input image. Program is designed to be MISRA C:2012
compliance and dynamic in contrast to size of input and hidden layers -> input sizes are
computed from the shape of input data. Pre-trained values (weights, biases etc.) are located in
one header file, to avoid file reading operations and for simplification reasons. Program is designed
to be energy efficient.

High level operation principle of the program is very straightforward. Each (necessary) layer of trained
model is represented as function, and one such layer's return value is the input of the layer that follows.

Input data of the network (flattened image) is 1D array of floats.

Program contains only code necessary, and is stripped from use of development tools etc..


## Structure of NN (As trained and implemented):
```plaintext
+------------------------+  
| Input (401)            | (Not implementated in C-version as definion of layer)  
+------------------------+  
| Output: (None, 401, 1) |  
+------------------------+  
                  |  
+------------------------+  
| Batch_Normalization    |  
+------------------------+  
| Output: (None, 401, 1) |  
| Params: 4              |  
+------------------------+  
                  |  
+------------------------+  
| Dropout (Rate: 0.0)    | (Not implementated in C-version)  
+------------------------+  
| Output: (None, 401, 1) |  
| Params: 0              |  
+------------------------+  
                  |  
+------------------------+  
| Conv1D (24 kernel)     | => Filter size : 5  
+------------------------+  
| Output: (None, 401, 24)|  
| Params: 144            |  
+------------------------+  
                  |  
+------------------------+  
| Activation             | => Relu  
+------------------------+  
| Output: (None, 401, 24)|  
+------------------------+  
                  |  
+------------------------+  
| Batch_Normalization_1  |  
+------------------------+  
| Output: (None, 401, 24)|  
| Params: 96             |  
+------------------------+  
                  |  
+------------------------+
| Dropout_1 (Rate: 0.3)  | (Not implementated in C-version)  
+------------------------+  
| Output: (None, 401, 24)|  
| Params: 0              |  
+------------------------+  
                  |  
+------------------------+  
| Conv1D_1 (8 kernels)   | => Filter size : 5  
+------------------------+  
| Output: (None, 401, 8) |  
| Params: 968            |  
+------------------------+  
                  |  
+------------------------+  
| Activation_1           | => Relu  
+------------------------+  
| Output: (None, 401, 8) |  
+------------------------+  
                  |  
+------------------------+  
| Batch_Normalization_2  |  
+------------------------+  
| Output: (None, 401, 8) |  
| Params: 32             |  
+------------------------+  
                  |  
+------------------------+  
| Flatten                |  
+------------------------+  
| Output: (None, 3208)   |  
+------------------------+  
                  |  
+------------------------+
| Dropout_2 (Rate: 0.4)  | (Not implementated in C-version)  
+------------------------+  
| Output: (None, 3208)   |  
| Params: 0              |  
+------------------------+  
                  |  
+------------------------+  
| Dense (10)             |  
+------------------------+  
| Output: (None, 10)     |  
| Params: 32090          |  
+------------------------+  
                  |  
+------------------------+  
| Activation_2           | => Normalised sigmoid  
+------------------------+  
| Output: (None, 10)     |  
+------------------------+  
```
         
## Task distribution of the C-implementation:

**Necessary**:
tools.h            : Function prototype, also contains defined numbers, like kernel counts etc.  
tools.c            : Contains functionality and layers  
trained_model.h    : Contains pre trained data  
trained_nn_misra.c : Passes input to layers  

**For testing purposes**:  
dat_parser.h       : Function prototype, also contains defined numbers  
dat_parser.c       : Tool for reading and stroing test data to be passed to NN  
