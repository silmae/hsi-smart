import pandas as pd
import os
import matplotlib.pyplot as plt
import seaborn as sns

# Set the paths to the two folders containing the .csv files
folder1_path = "../Cpp-Implementation/Output-Data"
folder2_path = "../Cpp-Implementation/Tensorflow-Output-Values"

# Create two empty dataframes to hold the data from each folder
c_data = pd.DataFrame()
tf_data = pd.DataFrame()

# Loop through the .csv files in folder 1 and concatenate the dataframes into c_data
for filename in os.listdir(folder1_path):
    if filename.endswith(".csv"):
        file_path = os.path.join(folder1_path, filename)
        c_data = pd.concat([c_data, pd.read_csv(file_path)], axis=1)

# Loop through the .csv files in folder 2 and concatenate the dataframes into tf_data
for filename in os.listdir(folder2_path):
    if filename.endswith(".csv"):
        file_path = os.path.join(folder2_path, filename)
        tf_data = pd.concat([tf_data, pd.read_csv(file_path)], axis=1)

# Calculate the correlation matrix between the two dataframes
corr_matrix = c_data.corrwith(tf_data)

print(c_data.shape)
print(tf_data.shape)
# Plot the correlation matrix as a heatmap
sns.heatmap(corr_matrix, cmap="coolwarm", annot=True)
plt.show()

