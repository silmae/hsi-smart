NN implementation in C++, based on biases and weights produced by tensorflow NN, trained with MNIST-dataset.
Program loads weights, biases and input-data from separate .csv-files. Input data represents first image of
MNIST-dataset.

For each neuron, is applied principle of:

output = activation_function(w1x1 + w2x2 + ... + wn*xn + bias)

where:
    w1, w2, ..., wn are the weights on each input
    x1, x2, ..., xn are the values of each input
    activation_function, like sigmoid, ReLU, or tanh
