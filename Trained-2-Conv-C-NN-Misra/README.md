## What is this?:
This is C-implementation for trained neural network (Structure down below).
Program (Version in Optimised directory) is designed to be MISRA C:2012 compliance
and dynamic in contrast to size of input and hidden layers -> input sizes are
computed from the shape of input data. Optimised version has hardcoded weights, biases etc.
to minimize resources needed. Also, that directory contains only code necessary. 

Version in base implementation, is mainly for development purposes, and to allow
changes in .h5 files (from which weights etc. are loaded) to be more easily implemented
in code. Program assumes similar hierarch and datasets from .h5, but is not restricted by
variation in size of those datasets.

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
         
## Task distribution in both versions:

**High level logic and operations**:

- Main-program: **trained_nn_misra.c**

**Activation functions, loading of the necessary data, unit conversions,
re-structuring, saving etc. :**

- Function prototype: **calc_tools.h**
- Source file: **calc_tools.c**

**Possible visualisations via separate python program:**
- **visualiser.py**

## Process flow:
**Todo**
