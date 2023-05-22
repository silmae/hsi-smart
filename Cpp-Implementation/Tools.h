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
#include <random>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Text_Display.H>
#include <cstdlib>
#include <unistd.h>
#include <limits.h>

// This file contains tools for NN implementation (And tools used for debugging and visulising purposes)
// Created:       08.05.2023
// Last modified: 22.05.2023
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

// Softmax activation function:  convert a vector of real numbers into a probability distribution. 
std::vector<float> softmax(const std::vector<float>& input) {
    std::vector<float> output;
    float sum = 0.0;

    // Compute the normalization term (sum of exponentials)
    for (float value : input) {
        sum += std::exp(value);
    }

    // Compute the softmax values
    for (float value : input) {
        output.push_back(std::exp(value) / sum);
    }

    return output;
} 

// More extensive softmax activation function:  convert a vector of real numbers into a probability distribution. 
std::vector<float> normalizing_softmax(const std::vector<float>& input) {
  std::vector<float> output;
  float max_value = input[0];
  
  // Find the maximum value in the input vector
  for (float value : input) {
    if (value > max_value)
      max_value = value;
  }
  
  // Compute the softmax values
  float sum = 0.0f;
  for (float value : input) {
    float exp_value = std::exp(value - max_value);
    output.push_back(exp_value);
    sum += exp_value;
  }
  
  // Normalize the softmax values
  for (float& value : output) {
    value /= sum;
  }
  
  return output;
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
    std::cout << weights[0].size() << std::endl;
    std::cout << weights.size() << std::endl;

    // For each column (input neurons) of weight matrix
    for (int i = 0; i < output.size(); i++) { //128
        float single_neuron_output = 0;
        for (int j = 0; j < input.size(); j++) { // 784
            single_neuron_output += input[j] * weights[j][i];
        }
        // This if prevents activation function to be applied to output layer
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
        } else {
              output[i] = single_neuron_output;
          }
    }
    
    
    if (print_output_values) {
        std::string title = "Neurons and values of " + layer + ", w/o softmax:\n";
        C::print(C::G, title);    
        print_output(output);
        std::cout << "\n";
    }
    if (if_output_layer) {
        // Apply softmax
        output = softmax(output);
    }

    return output;
}

// Function to perform dropout-layer functionality
// rng-parameter is C++11 random number generator
// TODO: remove if not used
std::vector<float> dropout(const std::vector<float> input, float rate, std::mt19937& rng) {
    std::vector<float> output(input.size());
    
    // Creates dynamic array of floats, with uniform distribution over the interval (0.0, 1.0)
    std::uniform_real_distribution<float> uniform_dist(0.0f, 1.0f);
    for (int i = 0; i < input.size(); i++) {
        
        // If our 
        if (uniform_dist(rng) < rate) {
            output[i] = 0.0f;
        } else {
            output[i] = input[i] / (1.0f - rate);
        }
    }
    return output;
}

// Function to save 1D-vector to given relative path, as .csv
void vector_saver_1d(const std::vector<float> input, const std::string relative_path) {
    
    // Open the output file
    std::ofstream outfile(relative_path);
    if (!outfile.is_open()) {
        std::cerr << "Failed to open file: " << relative_path << std::endl;
        return;
    }
    
    // Write the vector to the .CSV file
    for (int i = 0; i < input.size(); i++) {
        outfile << input[i];
        if (i != input.size() - 1) {
            outfile << ",";
        }
    }
    // Close the output file
    outfile.close();
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

std::string vectorToString(const std::vector<float>& vector, const char delimiter=',') {
    std::stringstream ss;
    for (const auto& value : vector)
        ss << value << delimiter;
    std::string vectorString = ss.str();
    // Remove the last delimiter
    vectorString.pop_back();
    return vectorString;
}

// Calls python program, that creates plot and saves it as png, also opens it after saving
void correlationCaller(const std::vector<float>& vector1, const std::vector<float>& vector2, std::string filename) {
    // Convert vectors to strings
    std::string strVector1 = vectorToString(vector1);
    std::string strVector2 = vectorToString(vector2);

    // Get the current working directory
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == nullptr) {
        // Failed to get the current working directory
        return;
    }

    // Construct the path to the Python script
    std::string scriptPath = std::string(cwd) + "/../Correlation-Plotter-Python/plotter.py";

    // Construct the command to call the Python script and save the plot as an image
    std::string outputFilePath = std::string(cwd) + "/../Correlation-Plotter-Python/" + filename;
    std::string command = "python3 " + scriptPath + " " + strVector1 + " " + strVector2 + " " + outputFilePath;

    // Call the Python script
    int result = system(command.c_str());
    if (result != 0) {
        // Failed to execute the Python script
        std::cerr << "Failed to execute the Python script." << std::endl;
    }
}


void compareVectors(const std::vector<float>& tensorflow_vector, const std::vector<float>& cpp_vector, std::string layer_name, std::string plot_compare, std::string filename) {

    // Call for displaying plots
    if (plot_compare == "y" || plot_compare == "Y") {
        correlationCaller(tensorflow_vector, cpp_vector, filename);
    }

    // Creating message to be displayed in the pop-up window
    std::string message = layer_name + "\n\nValue pairs of output neurons (Tensorflow : Cpp-implementation) :\n\n";

    // Iterating through the vectors and compare their values
    for (int i = 0; i < tensorflow_vector.size(); ++i) {
        float value_tensorflow = tensorflow_vector[i];
        float value_cpp = cpp_vector[i];

        // Adding the value pair to the message string
        message += "Value pair " + std::to_string(i+1) + ": " + std::to_string(value_tensorflow) + " : " + std::to_string(value_cpp) + "\n";
    }
    const char* message_cstr = message.c_str(); // Converting message to const char*
    
    // Create a pop-up window
    Fl_Window *window = new Fl_Window(550, 600, "Value comparisons");

    // Create a text widget and set its size and position
    Fl_Text_Display *text = new Fl_Text_Display(0, 0, 550, 600);
    text->buffer(new Fl_Text_Buffer());
    text->buffer()->text(message_cstr);
    text->align(FL_ALIGN_CENTER);

    window->add(text);
    // End the window group and show the window
    window->end();
    window->show();
    Fl::run();

}
#endif // TOOLS_H
