#ifndef TOOLS_H
#define TOOLS_H

#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <iostream>
#include "C.h"
#include <dirent.h>

// This file contains tools for NN implementation (And tools used for debugging and visulising purposes)
// Created:       08.05.2023
// Last modified: 12.05.2023
// @ Jukka J

// Checks if value is positive, if not returns 0
float relu(float value) {
    return value >= 0 ? value : 0;
}

// Leaky Relu activation function: maps any input value to a value between -1 and 1. 
float leaky_relu(float value) {
    return value >= 0 ? value : 0.1 * value;
}

// Sigmoid activation function: maps any input value to a value between 0 and 1.
float sigmoid(float value) {
    return 1 / (1 + exp(-value));
}

// Tanh activation function: maps any input value to a value between -1 and 1
float tanh(float x) {
    return tanh(x);
}

// Softmax activation function: Takes a vector of floats as input and outputs another vector where each element is a probability between 0 and 1.
std::vector<float> softmax(std::vector<float> values) {
    std::vector<float> result(values.size());
    float sum = 0.0;
    for (int i = 0; i < values.size(); i++) {
        result[i] = exp(values[i]);
        sum += result[i];
    }
    for (int i = 0; i < values.size(); i++) {
        result[i] /= sum;
    }
    return result;
}


// Function to print neuron outputs (1D-array) of NN-layer
void print_output(std::vector<float> output) {

    for (int i = 0; i < output.size(); i++) {
        std::cout << "Output of (" << std::flush;
        C::print(C::CY, std::to_string(i + 1));
        std::cout << ") neuron: " << std::flush;
        C::print(C::Y, std::to_string(output[i]));
        std::cout << "" << std::endl;
    }
}

// Function to calculate and return output of each layer (1D-vector)
std::vector<float> calculate_layer_output(std::vector<float> input, std::vector<std::vector<float>> weights, std::vector<float> biases, std::string activation_function, bool print_output_values, std::string layer, bool if_output_layer) {
    // Output vector of the layer, size is neuron count
    std::vector<float> output(weights[0].size());

    // For each column (input neurons) of weight matrix
    for (int i = 0; i < weights[0].size(); i++) {
        float single_neuron_output = 0;
        for (int j = 0; j < weights.size(); j++) { // Items in row
            single_neuron_output += input[j] * weights[j][i];
        }
        // This if prevents activation function to be apllied to output layer
        if (!if_output_layer) {
        
            // Adding biases for each neuron output
            single_neuron_output += biases[i];
        
            if (activation_function == "sigmoid") {
                output[i] = sigmoid(single_neuron_output);
            }

            else if (activation_function == "tanh") {
                output[i] = tanh(single_neuron_output);
            }
        
            else if (activation_function == "leaky_relu") {
                output[i] = leaky_relu(single_neuron_output);
            }
            // If none of above, we use basic relu
            else {
                output[i] = relu(single_neuron_output);
            }
        }
    }
    
    if (print_output_values) {
        std::string title = "Neurons and values of " + layer + ":\n";
        C::print(C::G, title);    
        print_output(output);
        std::cout << "\n";
    }
    
    return output;
}

// Function for converting row of floats to string
std::string rowToString(const std::vector<float>& row, const char delimiter=',') {
    std::stringstream ss;
    std::copy(row.begin(), row.end(), std::ostream_iterator<float>(ss, &delimiter));
    std::string rowString = ss.str();
    // Remove the last delimiter
    rowString.pop_back();
    return rowString;
}

// Function for converting vector to printable form
std::string vectorToString(const std::vector<std::vector<float>>& vec, const std::string& delimiter) {
    std::stringstream ss;
    for (const auto& row : vec) {
        for (const auto& val : row) {
            ss << val << delimiter;
        }
        ss << "\n";
    }
    return ss.str();
}

// Function for loading bias values of NN (.csv-files) from relative paths, to 1D-vectors
std::vector<float> iterateBiasStructure(std::string filename, int neuron_count) {
    std::vector<float> biases(neuron_count);
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cout << "Failed to open file: " << filename << std::endl;
    }
    
    if (file) {
        std::string line;
        int ind = 0;
        // Iterate, as long as lines in file
        while (getline(file, line) && ind < neuron_count) {
            // Input stream
            std::istringstream iss(line);
          
            std::string value;
            if (getline(iss, value, ',')) {
                // Convert string to float and store in vector
                biases[ind] = std::stof(value);
                ind++;
            }
        }
        file.close();
    }
    
    return biases;
}
// int columns, int rows, 
// Function for loading csv files (weights of NN), from relative paths, to 2D-vectors
std::vector<std::vector<float>> iterateWeightStructure(std::string filename) {
    
    // Opening of filestream
    
    std::ifstream infile(filename);
    
    // Checking if file was found
    if (!infile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
    }
    std::string line;
    // Initiliazing 2D vector of given size
    std::vector<std::vector<float>> matrix;
    // std::vector<std::vector<float>> matrix(rows, std::vector<float>(columns));

    // Equivalent to columns count
    while (getline(infile, line)) {
        std::vector<float> row;
        std::stringstream ss(line);
        std::string value;
        while (getline(ss, value, ','))
        {
            float f = stof(value);
            row.push_back(f);
        }
        matrix.push_back(row);
    }
    return matrix;
}

std::vector<std::string> readFileNames(std::string folder_path) {
    
    DIR* dir = opendir(folder_path.c_str());
    if (!dir) {
        std::cout << "Can not open folder :(" << std::endl;
    }
    std::vector<std::string> file_names;
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        std::string file_name = entry->d_name;
        if (file_name != "." && file_name != "..") {
            file_names.push_back(file_name);
        }
    }
    closedir(dir); 
    return file_names;
}

#endif // TOOLS_H
