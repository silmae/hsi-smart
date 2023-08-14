import csv

# Created:       20.07.2023
# Last modified: 20.07.2023
# @ Jukka J

csv_file = 'dense_weight_1.csv'
txt_file = 'weights.txt'

# Reading the CSV file and convert to C format
c_code = ''
with open(csv_file, 'r') as file:
    reader = csv.reader(file)
    for row in reader:
        c_row = '{'
        c_row += ','.join(row)
        c_row += '},\n'
        c_code += c_row

# Removing the trailing comma and newline from the last row
c_code = c_code[:-2]

# Write the C code to a text file
with open(txt_file, 'w') as file:
    file.write(c_code)

