import sys
import numpy as np
import matplotlib.pyplot as plt
import subprocess
import time

def compare_values(vector1, vector2, output_file):
    # Filter out non-valid value pairs
    filtered_pairs = [(x, y) for x, y in zip(vector1, vector2) if x != '0' and y != '0']

    if len(filtered_pairs) == 0:
        print("No valid value pairs found.")
        return

    # Extract values from filtered pairs and convert to floats
    values1 = [float(val) for x, y in filtered_pairs for val in x.split(',')]
    values2 = [float(val) for x, y in filtered_pairs for val in y.split(',')]

    # Calculate midpoints for each pair
    midpoints = [(float(x) + float(y)) / 2 for x, y in filtered_pairs]

    # Calculate the mean value
    mean_value = np.mean(midpoints)

    # Create plot with two curves, midpoint line, and mean line
    plt.plot(values1, label='Tensorflow', color='blue')
    plt.plot(values2, label='Cpp-implementation', color='green')
    plt.plot(midpoints, label='Midpoint', color='red', linestyle='--')
    plt.axhline(mean_value, color='orange', linestyle='--', label='Mean')
    plt.xlabel("Index")
    plt.ylabel("Value")
    plt.title("Comparison of Value-pairs")
    plt.legend()

    # Save the plot as an image
    plt.savefig(output_file)
    plt.close()  # Close the plot to release resources

# Get command-line arguments
vector1 = sys.argv[1].split(',')
vector2 = sys.argv[2].split(',')
output_file = sys.argv[3]

# Call the compare_values function
compare_values(vector1, vector2, output_file)

# Open the saved image using the default image viewer
subprocess.run(['xdg-open', output_file])
