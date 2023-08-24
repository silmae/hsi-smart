import pandas as pd

# List of .CSV files
csv_files = ["../Raw-Data/scen1/scen1_1.csv", "../Raw-Data/scen1/scen1_2.csv", "../Raw-Data/scen1/scen1_3.csv", "../Raw-Data/scen1/scen1_4.csv", "../Raw-Data/scen1/scen1_5.csv", "../Raw-Data/scen1/scen1_6.csv", "../Raw-Data/scen1/scen1_7.csv", "../Raw-Data/scen1/scen1_8.csv", "../Raw-Data/scen1/scen1_9.csv", "../Raw-Data/scen1/scen1_10.csv"]
# Initializing a dictionary to store time-step averages for each column
time_step_averages = {}

# Process each .CSV file
for csv_file in csv_files:
    df = pd.read_csv(csv_file)
    
    # Iterate through each row
    for index, row in df.iterrows():
        time = row['#time']
        
        # Calculate averages for columns, ignore col-titles
        for column in df.columns:
            if column != '#time' and df[column].dtype in [int, float]:
                if time not in time_step_averages:
                    time_step_averages[time] = {}
                time_step_averages[time].setdefault(column, []).append(row[column])

# Calculate and print the average for each time step and column
for time, column_data in time_step_averages.items():
    print(f"Time: {time}")
    for column, values in column_data.items():
        # Only calculate average if there are values for all CSV files at this time step
        if len(values) == len(csv_files):
            avg = sum(values) / len(values)
            print(f"  Average of {column}: {avg}")
        else:
            print(f"  Average of {column}: N/A (incomplete data)")

