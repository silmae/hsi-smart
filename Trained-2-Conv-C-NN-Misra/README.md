## What is this?:
This is C-implementation for trained neural network (Structure down below).
Program designed to be MISRA C:2012 compliance and dynamic in contrast to size
of input and hidden layer -> input sizes are computed from the shape of input data.

Input data type of weights, biases and test data -> .h5

## Structure of NN:

         +------------------------+
         |      Input (401)       |
         |      Relu              |
         +------------------------+
                  |
         +------------------------+
         | Conv1 (24 filters, 5x5)|
         |      Relu              |
         +------------------------+
                  |
         +------------------------+
         | Conv2 (8 kernels, 5x5) |
         |      Sigmoid (10)      |
         +------------------------+
                  |
         +------------------------+
         |      Output (10)       |
         +------------------------+
         
## Task distribution of:

**High level logic and operations**:

- Source file: **trained_nn_misra.c**

**Activation functions, loading of the necessary data, unit conversions,
re-structuring, saving etc. :**

- Function prototype: **calc_tools.h**
- Source file: **calc_tools.c**

**Possible visualisations via separate python program:**
- **visualiser.py**

## Process flow:
**Todo**
