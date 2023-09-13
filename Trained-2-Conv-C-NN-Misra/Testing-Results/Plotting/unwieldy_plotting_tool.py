import numpy as np
import matplotlib.pyplot as plt
import csv

# Created:       21.08.2023
# Last modified: 22.08.2023
# @ Jukka J j

# Calculating avgs, from measured time of 10S, with Joulescope DC-analyzer
#=======================================================================================
data = """#time,current,voltage,power,charge,energy
0.0,0.399422,4.74572,1.89553,0,0
0.5,0.400691,4.74479,1.90114,0.200345,0.950568
1.0,0.399392,4.74555,1.89532,0.400042,1.89823
1.5,0.399461,4.74573,1.89572,0.599772,2.84609
2.0,0.399297,4.74579,1.89497,0.799421,3.79357
2.5,0.399307,4.74592,1.89506,0.999074,4.7411
3.0,0.399062,4.74588,1.89389,1.19861,5.68804
3.5,0.399372,4.74576,1.89531,1.39829,6.6357
4.0,0.399149,4.74593,1.89432,1.59787,7.58286
4.5,0.467436,4.69501,2.18967,1.83158,8.67769
5.0,0.460906,4.69969,2.16526,2.06204,9.76032
5.5,0.457727,4.70195,2.15196,2.2909,10.8363
6.0,0.456213,4.7031,2.14545,2.51901,11.909
6.5,0.456026,4.70327,2.14468,2.74702,12.9814
7.0,0.450968,4.70706,2.12233,2.9725,14.0425
7.5,0.448703,4.70874,2.11243,3.19685,15.0988
8.0,0.453785,4.70493,2.13463,3.42375,16.1661
8.5,0.461701,4.69915,2.16917,3.6546,17.2507
9.0,0.461177,4.69954,2.1669,3.88519,18.3341
9.5,0.461367,4.69928,2.16768,4.11587,19.4179
10.0,0.475491,4.6887,2.22835,4.35361,20.5321"""

# Split the data into lines and skip the header
lines = data.strip().split('\n')[1:]

# Initialize variables to calculate averages
total_current = 0
total_voltage = 0
total_power = 0
num_entries = len(lines)

# Process each line and calculate totals
for line in lines:
    _, current, voltage, power, _, _ = map(float, line.split(','))
    total_current += current
    total_voltage += voltage
    total_power += power

# Calculate averages
idle_avg_mA = total_current / num_entries * 1000
idle_avg_V = total_voltage / num_entries
idle_avg_W = total_power / num_entries

# Print the results
print("Average Current:", idle_avg_mA)
print("Average Voltage:", idle_avg_V)
print("Average Power:", idle_avg_W)
#=======================================================================================

#=======================================================================================
def add_value_pointers(ax, y, x, face_color1, face_color2, face_color3, face_color4, arrow_style, unit, calculated_avg, xyt_med, xyt_peak, override, which, subtr_text, xyt_avg):
    y_median = np.median(y)
    y_diff_from_avg = np.abs(y - calculated_avg)
    y_peak_index =  np.abs(np.argmax(y_diff_from_avg))
    xy_tmp = (x[y_peak_index], y[y_peak_index])
    
    if not override:
        if y[y_peak_index] < y_median:
            text = f'Trough: {y[y_peak_index]:.3f}'
        else:
            text = f'Crest: {y[y_peak_index]:.3f}'
    
    if override:
        if which == 1:
            text = f'Trough: {y[y_peak_index]:.3f}'
        else:
            text = f'Crest: {y[y_peak_index]:.3f}'

    ax.annotate(text, xy=(x[y_peak_index], y[y_peak_index]),
                 xytext=(xyt_peak[0], xyt_peak[1]),
                 arrowprops=dict(facecolor=face_color1, arrowstyle=arrow_style, alpha=0.7),
                 )

    ax.axhline(y=y_median, color='gray', linestyle='--')
    ax.annotate(f'Median: {y_median:.3f}', xy=(x[0], y_median),
                 xytext=(xyt_med[0], xyt_med[1]),
                 arrowprops=dict(facecolor=face_color2, arrowstyle=arrow_style, alpha=0.7),
                )

    ax.annotate(f'Avg: {calculated_avg:.3f}', xy=(3.0, calculated_avg),
                 xytext=(xyt_avg[0], xyt_avg[1]),
                 arrowprops=dict(facecolor=face_color1, arrowstyle=arrow_style, alpha=0.7),
                 )
    
    peak_value = y[y_peak_index]
    subtraction_result = peak_value - calculated_avg
    
    # Positioning the subtraction annotation in the upper left corner
    ax.annotate(f'{subtr_text}: {subtraction_result:.3f}: ({unit})', xy=(x[0], y[-1] - 0.005),  # Adjust the y-coordinate
                 xytext=(0.015, 0.85),  # Upper left corner position
                 xycoords='axes fraction', textcoords='axes fraction',
                 bbox=dict(boxstyle='round,pad=0.3', edgecolor="black", facecolor=face_color3),
                 )
    ax.annotate(text, xy=(x[y_peak_index], y[y_peak_index]),
                 xytext=(xyt_peak[0], xyt_peak[1]),
                 arrowprops=dict(facecolor=face_color1, arrowstyle=arrow_style, alpha=0.7),
                 )
     
    
#========================================================================================
x = np.arange(0.0, 4.5, 0.5)

y1_data = [0.446887, 0.44761, 0.448333, 0.450261, 0.490901, 0.452859, 0.447148, 0.447461, 0.447329, 0.447305]
# Unit conversion performed, from A to mA
for i in range(len(y1_data)):
    y1_data[i] = y1_data[i] * 1000

y1 = np.array(y1_data[:len(x)])  # Truncating y1 to match the length of x

y2_data = [4.71581, 4.71518, 4.71474, 4.71326, 4.68281, 4.71134, 4.71558, 4.71543, 4.71549, 4.71551]
y2 = np.array(y2_data[:len(x)])

y3_data = [2.10742, 2.11055, 2.11372, 2.12208, 2.29876, 2.13338, 2.10855, 2.10995, 2.10936, 2.10926]
y3 = np.array(y3_data[:len(x)])

y4_pre = [0, 1.05527, 2.11213, 3.17317, 4.32255, 5.38924, 6.44352, 7.4985, 8.55318, 9.6078]
y4_data = [y4_pre[0], y4_pre[1], (y4_pre[2] - y4_pre[1]), (y4_pre[3]-y4_pre[2]), (y4_pre[4] - y4_pre[3]), (y4_pre[5] - y4_pre[4]), (y4_pre[6] - y4_pre[5]), (y4_pre[7] - y4_pre[6]), (y4_pre[8] - y4_pre[7]), (y4_pre[9] - y4_pre[8])]

y4 = np.array(y4_data[:len(x)])

fig = plt.figure()
# Title above all subplots
fig.suptitle('Scen1', fontsize=16)

#========================================================================================
# Create a 2x2 grid of subplots
ax1 = fig.add_subplot(3, 1, 1)
ax1.plot(x, y1, color='#1864ab')
ax1.set_xlabel('Time (s)')
ax1.set_ylabel('Current (mA)')
ax1.set_xticks(x)  # Set custom tick positions
ax1.set_xticklabels([str(val) for val in x])  # Set custom tick labels
ax1.set_ylim(350, 980)  # Set custom y-axis range
xyt_med = [0.0, 625]
xyt_peak = [1.5, 625]
xyt_avg = [3.0, 625]
add_value_pointers(ax1, y1, x, 'black', 'black', 'lightblue', '#1864ab', '-', 'mA', idle_avg_mA, xyt_med, xyt_peak, True, 0, "Rise in current draw", xyt_avg)

ax2 = fig.add_subplot(3, 1, 2)
ax2.plot(x, y2, color='#f08c00')
ax2.set_xlabel('Time (s)')
ax2.set_ylabel('Voltage (V)')
ax2.set_xticks(x)  # Set custom tick positions
ax2.set_xticklabels([str(val) for val in x])  # Set custom tick labels
ax2.set_ylim(4.6, 5.25)  # Set custom y-axis range
xyt_med = [0.0, 4.85]
xyt_peak = [1.5, 4.85]
xyt_avg = [3.0, 4.85]
add_value_pointers(ax2, y2, x, 'black', 'black', 'lightblue', '#f08c00', '-', 'V', idle_avg_V, xyt_med, xyt_peak, True, 1, "Drop in voltage", xyt_avg)

ax3 = fig.add_subplot(3, 1, 3)
ax3.plot(x, y3, color='#f5c211')
ax3.set_xlabel('Time (s)')
ax3.set_ylabel('Power (W)')
ax3.set_xticks(x)  # Set custom tick positions
ax3.set_xticklabels([str(val) for val in x])  # Set custom tick labels
ax3.set_ylim(1.7, 4.78)  # Set custom y-axis range
xyt_med = [0.0, 3.0]
xyt_peak = [1.5, 3.0]
xyt_avg = [3.0, 3.0]
add_value_pointers(ax3, y3, x, 'black', 'black', 'lightblue', '#f5c211', '-', 'W', idle_avg_W, xyt_med, xyt_peak, True, 0, "Rise in power-consumption", xyt_avg)

# Adding idle avgs

# Add average dotted lines to subplots
ax1.axhline(y=idle_avg_mA, color='#ed333b', linestyle='dotted', label='Avg Current')
ax2.axhline(y=idle_avg_V, color='#ed333b', linestyle='dotted', label='Avg Voltage')
ax3.axhline(y=idle_avg_W, color='#ed333b', linestyle='dotted', label='Avg Power')

plt.tight_layout()  # Adjust subplot layout for better spacing
plt.savefig('scen1.png')
plt.show()
