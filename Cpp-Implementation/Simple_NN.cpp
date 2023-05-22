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
// Last modified: 22.05.2023
// @ Jukka J

// Storage to hold outputs for all test images passed, for each layer
std::vector<std::vector<float>> outputs_for_all_images_first_hidden;
std::vector<std::vector<float>> outputs_for_all_images_second_hidden;
std::vector<std::vector<float>> outputs_for_all_images_third_hidden;
std::vector<std::vector<float>> outputs_for_all_images_dropout;
std::vector<std::vector<float>> input_data_all_images;
std::vector<std::vector<float>> outputs_for_all_images_final;

int main() {

    // ======================= Loading weights and bises and storing them in vectors ======================================================================
    std::vector<std::string> weight_files = {"1st_hidden_weights.csv", "2nd_hidden_weights.csv", "3rd_hidden_weights.csv", "output_layer_weights.csv"};
    std::vector<std::string> bias_files = {"1st_hidden_bias.csv", "2nd_hidden_bias.csv", "3rd_hidden_weights.csv", "output_layer_bias.csv"};
    
    // ======================= Filenames for output values for each of first 10 images from mnist dataset, for corresponding layers  ======================
    std::vector<std::string> tensorflow_layer_outputs_layer_input = {"layer_1_image_1_output.csv", "layer_1_image_2_output.csv", "layer_1_image_3_output.csv", "layer_1_image_4_output.csv", "layer_1_image_5_output.csv", "layer_1_image_6_output.csv", "layer_1_image_7_output.csv", "layer_1_image_8_output.csv", "layer_1_image_9_output.csv", "layer_1_image_10_output.csv"};
    
    std::vector<std::string> tensorflow_layer_outputs_layer_1h = {"layer_2_image_1_output.csv", "layer_2_image_2_output.csv", "layer_2_image_3_output.csv", "layer_2_image_4_output.csv", "layer_2_image_5_output.csv", "layer_2_image_6_output.csv", "layer_2_image_7_output.csv", "layer_2_image_8_output.csv", "layer_2_image_9_output.csv", "layer_2_image_10_output.csv"};
    
    std::vector<std::string> tensorflow_layer_outputs_layer_2h = {"layer_3_image_1_output.csv", "layer_3_image_2_output.csv", "layer_3_image_3_output.csv", "layer_3_image_4_output.csv", "layer_3_image_5_output.csv", "layer_3_image_6_output.csv", "layer_3_image_7_output.csv", "layer_3_image_8_output.csv", "layer_3_image_9_output.csv", "layer_3_image_10_output.csv"};
    
    std::vector<std::string> tensorflow_layer_outputs_layer_3h = {"layer_4_image_1_output.csv", "layer_4_image_2_output.csv", "layer_4_image_3_output.csv", "layer_4_image_4_output.csv", "layer_4_image_5_output.csv", "layer_4_image_6_output.csv", "layer_4_image_7_output.csv", "layer_4_image_8_output.csv", "layer_4_image_9_output.csv", "layer_4_image_10_output.csv"};
    
    std::vector<std::string> tensorflow_layer_outputs_layer_dropout = {"layer_5_image_1_output.csv", "layer_5_image_2_output.csv", "layer_5_image_3_output.csv", "layer_5_image_4_output.csv", "layer_5_image_5_output.csv", "layer_5_image_6_output.csv", "layer_5_image_7_output.csv", "layer_5_image_8_output.csv", "layer_5_image_9_output.csv", "layer_5_image_10_output.csv"};
    
    std::vector<std::string> tensorflow_layer_outputs_layer_final = {"layer_6_image_1_output.csv", "layer_6_image_2_output.csv", "layer_6_image_3_output.csv", "layer_6_image_4_output.csv", "layer_6_image_5_output.csv", "layer_6_image_6_output.csv", "layer_6_image_7_output.csv", "layer_6_image_8_output.csv", "layer_6_image_9_output.csv", "layer_6_image_10_output.csv"};
    
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
        std::cout << "Test data filenames:\n" << std::endl;  
        std::cout << file_name << std::endl;
        std::vector<float> input_data = iterateBiasStructure(input_filepath, 784);
        
        // Store in dynamic vector for later use
        input_data_all_images.push_back(input_data);
        
        // Decide if values should be printed to console
        bool print_output_values = true;

        // Open a file for writing
        std::string output_filename = file_name.substr(0,(file_name.length() - 4));
        std::string info = "Passing " + output_filename + " for layers\n";
        C::print(C::R, info);
        std::cout << "Size of input data vector: " << input_data.size() << "\n" << std::endl;

        // Forward pass: calculating outputs of one layer, and passing outputs to next layer    
        std::vector<float> output_first_hidden = calculate_layer_output(input_data, first_hidden_weights, first_hidden_biases, "", print_output_values, "1st hidden layer", false);
        
        // Store in dynamic vector for later use
        outputs_for_all_images_first_hidden.push_back(output_first_hidden);
        std::vector<float> output_second_hidden = calculate_layer_output(output_first_hidden, second_hidden_weights, second_hidden_biases, "", print_output_values, "2nd hidden layer", false);
        
        // Store in dynamic vector for later use
        outputs_for_all_images_second_hidden.push_back(output_second_hidden);
        std::vector<float> output_third_hidden = calculate_layer_output(output_second_hidden, third_hidden_weights, third_hidden_biases, "", print_output_values, "3rd hidden layer", false);
        
        // Store in dynamic vector for later use
        outputs_for_all_images_third_hidden.push_back(output_third_hidden);
        
        // Pass data to "dropout-layer"
        float rate = 0.2f; // Dropout rate
        std::mt19937 rng(0); // Random number generator
        // Not needed TODO: Possibly remove, if no other use
        //std::vector<float> output_dropout = dropout(output_third_hidden, rate, rng);
        
        std::vector<float> output_dropout = output_third_hidden;
        
        // Store in dynamic vector for later use
        outputs_for_all_images_dropout.push_back(output_dropout);
        
        std::vector<float> output = calculate_layer_output(output_dropout, output_weights, output_biases, "", print_output_values, "Output layer", true);

        // Store in dynamic vector for later use
        outputs_for_all_images_final.push_back(output);
        
        std::string output_filepath = relative_path_root + "Output-Data/" + file_name;
        std::ofstream file(output_filepath);
        
        // Precision set to 15 number after decimal, this is accuracy of tensorflow tf.float64
        file << std::fixed << std::setprecision(15);
        
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
    
    
    // Asking for visualizing comparisons of layers output values of Tensorflow and this C++ implementation. Mostly for debugging and transparency purposes
    std::string show_plot;
    std::string answer;
    std::cout << "Compare output values against Tensorflow-model's? (y/n): ";
    std::cin >> answer;
    if (answer == "y" || answer == "Y") {
        std::string answer_2nd;
        std::string plot_compare;
        
        std::cout << "Give number of test image (0-9): ";
        std::cin >> answer_2nd;
        int index = std::stoi(answer_2nd);
        std::cout << "\nShow correlation plots? (y/n): ";
        std::cin >> plot_compare;
        
        // Iterate over those Tensorflow-layers final output values, saved in .CSVs
        // 5 layers
        // Filenames for plots to be saved
        std::vector<std::string> names_for_plot_images = {"comparison_plot_input.png", "comparison_plot_1st_hidden.png", "comparison_plot_2nd_hidden.png", "comparison_plot_3rd.png", "comparison_plot_dropout.png", "comparison_plot_output.png"}; 

        for (int i = 0; i < 6; i++) {
          
            switch (i) {
                case 0: {
                // Lets again utilize this function, as structure of data is similar
                std::string file_relative_path = relative_path_root + "Tensorflow-Layer-Outputs/" + tensorflow_layer_outputs_layer_input[index];                
                std::vector<float> input_tensor = iterateBiasStructure(file_relative_path, 784);
                compareVectors(input_tensor, input_data_all_images[index], "Input-layer (flatten)", plot_compare, names_for_plot_images[i]);
                    break;
                }
                case 1: {
                std::string file_relative_path = relative_path_root + "Tensorflow-Layer-Outputs/" + tensorflow_layer_outputs_layer_1h[index];
                std::vector<float> first_hidden_tensor = iterateBiasStructure(file_relative_path, 128);
                compareVectors(first_hidden_tensor, outputs_for_all_images_first_hidden[index], "1st-hidden", plot_compare, names_for_plot_images[i]);
                    break;
                }
                case 2: {
                std::string file_relative_path = relative_path_root + "Tensorflow-Layer-Outputs/" + tensorflow_layer_outputs_layer_2h[index];
                std::vector<float> second_hidden_tensor = iterateBiasStructure(file_relative_path, 64);
                compareVectors(second_hidden_tensor, outputs_for_all_images_second_hidden[index], "2nd-hidden", plot_compare, names_for_plot_images[i]);
                    break;
                }
                case 3: {
                std::string file_relative_path = relative_path_root + "Tensorflow-Layer-Outputs/" + tensorflow_layer_outputs_layer_3h[index];
                std::vector<float> third_hidden_tensor = iterateBiasStructure(file_relative_path, 32);
                compareVectors(third_hidden_tensor,  outputs_for_all_images_third_hidden[index], "3rd-hidden", plot_compare, names_for_plot_images[i]);
                    break;
                }
                case 4: {
                std::string file_relative_path = relative_path_root + "Tensorflow-Layer-Outputs/" + tensorflow_layer_outputs_layer_dropout[index];
                std::vector<float> dropout_tensor = iterateBiasStructure(file_relative_path, 32);
                compareVectors(dropout_tensor, outputs_for_all_images_dropout[index], "Dropout-layer", plot_compare, names_for_plot_images[i]);
                    break;
                }
                default: {
                std::string file_relative_path = relative_path_root + "Tensorflow-Layer-Outputs/" + tensorflow_layer_outputs_layer_final[index];
                
                std::vector<float> output_tensor = iterateBiasStructure(file_relative_path, 10);
                compareVectors(output_tensor, outputs_for_all_images_final[index], "output-layer", plot_compare, names_for_plot_images[i]);
                    break;
                }
           }

        }
        
    }
}
