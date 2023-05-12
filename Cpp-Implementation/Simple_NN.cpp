#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "Tools.h"
#include "C.h"
#include <iomanip>

// This file contains NN implementation for trained NN, i.e program assumes calculated weights 
// Created:       08.05.2023
// Last modified: 12.05.2023
// @ Jukka J

int main() {

    // ======================= Loading weights and bises and storing them in vectors ======================================================================
    std::vector<std::string> weight_files = {"1st_hidden_weights.csv", "2nd_hidden_weights.csv", "3rd_hidden_weights.csv", "output_layer_weights.csv"};
    std::vector<std::string> bias_files = {"1st_hidden_bias.csv", "2nd_hidden_bias.csv", "3rd_hidden_weights.csv", "output_layer_bias.csv"};
    std::string relative_path_root = "./";
    std::vector<std::string> folder_of_layer = {"1st-Hidden/", "2nd-Hidden/", "3rd-Hidden/", "Output-Layer/"};
    
    // Structure of the weights: Number of neurons in current layer * Number of neurons in previous layer
    
    // 1st hidden layer
    std::cout << "\n" << std::endl;
    std::vector<std::vector<float>> first_hidden_weights = iterateWeightStructure((relative_path_root + folder_of_layer[0] + weight_files[0]));
    std::cout << "Weight matrix size (1st): " << first_hidden_weights.size() << "x" << first_hidden_weights[0].size() << std::endl;
    std::vector<float> first_hidden_biases = iterateBiasStructure((relative_path_root + folder_of_layer[0] + bias_files[0]), 128);
    std::cout << "Bias vector size (1st): " << first_hidden_biases.size() << std::endl;
    std::cout << "\n" << std::endl;
    
    // 2nd hidden layer
    std::vector<std::vector<float>> second_hidden_weights = iterateWeightStructure((relative_path_root + folder_of_layer[1] + weight_files[1]));
    std::cout << "Weight matrix size (2nd): " << second_hidden_weights.size() << "x" << second_hidden_weights[0].size() << std::endl;
    std::vector<float> second_hidden_biases = iterateBiasStructure((relative_path_root + folder_of_layer[1] + bias_files[1]), 64);
    std::cout << "Bias vector size (2nd): " << second_hidden_biases.size() << std::endl;
    std::cout << "\n" << std::endl;
    
    // 3rd hidden layer
    std::vector<std::vector<float>> third_hidden_weights = iterateWeightStructure((relative_path_root + folder_of_layer[2] + weight_files[2]));
    std::cout << "Weight matrix size (3rd): " << third_hidden_weights.size() << "x" << third_hidden_weights[0].size() << std::endl;
    std::vector<float> third_hidden_biases = iterateBiasStructure((relative_path_root + folder_of_layer[2] + bias_files[2]), 32);
    std::cout << "Bias vector size (3rd): " << third_hidden_biases.size() << std::endl;
    std::cout << "\n" << std::endl;    
    
    // Output layer
    std::vector<std::vector<float>> output_weights = iterateWeightStructure((relative_path_root + folder_of_layer[3] + weight_files[3]));
    std::cout << "Weight matrix size (output): " << output_weights.size() << "x" << output_weights[0].size() << std::endl;
    std::vector<float> output_biases = iterateBiasStructure((relative_path_root + folder_of_layer[3] + bias_files[3]), 10);
    std::cout << "Bias vector size (output): " << output_biases.size() << std::endl;
    std::cout << "\n" << std::endl;
    std::cout << "Weights and biases of layers loaded!" << std::endl;
    std::cout << "\n" << std::endl;
    
    // ======================== Actual implementation  ==========================================================================================================
    
    // Input data is ten first images from MNIST dataset, 28x28 image that is flattened into a 1D array with 128 elements.
    // Open folder of test data, and read filenames to array, sort
    std::string input_data_filepath = relative_path_root + "Test-Data";
    std::vector<std::string> test_filenames = readFileNames(input_data_filepath);
    std::sort(test_filenames.begin(), test_filenames.end());

    // For each image, load data into vector, pass to all layers
    // and save values of output layer neurons, per image
    std::vector<float> vectors_of_input_data;
    
    for (const auto& file_name : test_filenames) {

        std::string input_filepath = relative_path_root + "Test-Data/" + file_name;
        std::vector<float> input_data = iterateBiasStructure(input_filepath, 128);
        // Decide if values should be printed to console
        bool print_output_values = true;

        // Open a file for writing
        std::string output_filename = file_name.substr(0,(file_name.length() - 4));

        std::string info = "Passing " + output_filename + " for layers\n";
        C::print(C::R, info);
        std::cout << "Size of input data vector: " << input_data.size() << "\n" << std::endl;

        // Forward pass: calculating outputs of one layer, and passing outputs to next layer    

        std::vector<float> output_first_hidden = calculate_layer_output(input_data, first_hidden_weights, first_hidden_biases, "", print_output_values, "1st hidden layer", false);
    
        std::vector<float> output_second_hidden = calculate_layer_output(output_first_hidden, second_hidden_weights, second_hidden_biases, "", print_output_values, "2nd hidden layer", false);
    
        std::vector<float> output_third_hidden = calculate_layer_output(output_second_hidden, third_hidden_weights, third_hidden_biases, "", print_output_values, "3rd hidden layer", false);
    
        std::vector<float> output = calculate_layer_output(output_third_hidden, output_weights, output_biases, "", print_output_values, "Output layer", true);
        
        // Apply softmax
        output = softmax(output);
        
        std::string output_filepath = relative_path_root + "Output-Data/" + file_name;
        std::ofstream file(output_filepath);
        
        // Precision set to 10 number after decimal, this is to avoid saving values like 2.30609e+11 to .csv
        file << std::fixed << std::setprecision(10);
        
        // Write the vector to the file as comma-separated values
        for (int i = 0; i < output.size(); i++) {
            if (i > 0) {
                file << ",";
            }
            file << output[i];
        }

        // Close the file
        file.close();
    }

}
