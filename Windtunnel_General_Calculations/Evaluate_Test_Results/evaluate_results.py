import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
# from scipy.stats import entropy
from sklearn.metrics import mutual_info_score
import os

# pip install pandas odfpy matplotlib scipy


# Function to read either .csv or .ods file
def load_data(file_path):
    if file_path.endswith('.csv'):
        data = pd.read_csv(file_path,
                           sep="\t", 
                            skiprows=3,          # Skip the first 3 rows to start at the data
                 decimal=',',         # Specify comma as the decimal separator
                 header=None          # Since column headers are complex, load without headers and add them later
                 )
    elif file_path.endswith('.ods'):
        data = pd.read_excel(file_path, engine='odf')
    else:
        raise ValueError("Unsupported file format. Please provide a .csv or .ods file.")
    return data

# Load the data
cwd = os.getcwd()
print(cwd)
file_path = cwd + '/Docs/two_axis_test_sheet.csv'  # path to the .csv or .ods file
print(file_path)
data = load_data(file_path)

# Search for the known mass row
# known_mass = None
# for index, row in data.iterrows():
#     if "Known mass" in row.to_string():
#         known_mass = float(row[row.notna()].values[-1])
#         print(f"Known mass found: {known_mass}")
#         break

# if known_mass is None:
#     raise ValueError("Known mass not found in the dataset.")

# Select the data between "Loading from bottom" and "Loading from top" for processing
# This script assumes the table format and that the columns are labeled consistently
bottom_columns = ['Load cell 7 (bottom)', 'Load cell 3 (bottom)']
top_columns = ['Load cell 7 (top)', 'Load cell 3 (top)']

# Rename columns for easier access
data.columns = ['Index', 'Load cell 7 (bottom)', 'Load cell 3 (bottom)', 'Load cell 7 (top)', 'Load cell 3 (top)']

# Separate bottom and top loading data
bottom_data = data[bottom_columns].dropna().to_numpy()
top_data = data[top_columns].dropna().to_numpy()

# Calculate mean and standard deviation for each column
bottom_means = np.mean(bottom_data, axis=0)
top_means = np.mean(top_data, axis=0)
bottom_stds = np.std(bottom_data, axis=0)
top_stds = np.std(top_data, axis=0)

print("Bottom loading mean values:", bottom_means)
print("Bottom loading standard deviations:", bottom_stds)
print("Top loading mean values:", top_means)
print("Top loading standard deviations:", top_stds)

# Calculate mutual information as a measure of coupling between bottom and top load cells
# Using mutual information for continuous variables - this is a simplified approach
def mutual_information(x, y, bins=10):
    # Discretize data
    c_xy = np.histogram2d(x, y, bins)[0]
    mi = mutual_info_score(None, None, contingency=c_xy)
    return mi

mi_load_cell_7 = mutual_information(bottom_data[:, 0], top_data[:, 0])
mi_load_cell_3 = mutual_information(bottom_data[:, 1], top_data[:, 1])

print("Mutual Information between Load cell 7 (bottom) and Load cell 7 (top):", mi_load_cell_7)
print("Mutual Information between Load cell 3 (bottom) and Load cell 3 (top):", mi_load_cell_3)

# Plotting
fig, axs = plt.subplots(2, 2, figsize=(14, 10))
fig.suptitle("Coupling between Load Cells (Bottom vs. Top)")

# Plot Load cell 7 values from bottom and top
axs[0, 0].plot(bottom_data[:, 0], label="Bottom - Load cell 7", marker='o')
axs[0, 0].plot(top_data[:, 0], label="Top - Load cell 7", marker='x')
axs[0, 0].set_title("Load cell 7: Bottom vs. Top")
axs[0, 0].set_xlabel("Measurement Index")
axs[0, 0].set_ylabel("Load Cell Value")
axs[0, 0].legend()

# Plot Load cell 3 values from bottom and top
axs[0, 1].plot(bottom_data[:, 1], label="Bottom - Load cell 3", marker='o')
axs[0, 1].plot(top_data[:, 1], label="Top - Load cell 3", marker='x')
axs[0, 1].set_title("Load cell 3: Bottom vs. Top")
axs[0, 1].set_xlabel("Measurement Index")
axs[0, 1].set_ylabel("Load Cell Value")
axs[0, 1].legend()

# Plot histogram for Load cell 7 (bottom vs. top) to visualize distribution
axs[1, 0].hist(bottom_data[:, 0], bins=10, alpha=0.5, label="Bottom - Load cell 7")
axs[1, 0].hist(top_data[:, 0], bins=10, alpha=0.5, label="Top - Load cell 7")
axs[1, 0].set_title("Histogram: Load cell 7 (Bottom vs. Top)")
axs[1, 0].set_xlabel("Load Cell Value")
axs[1, 0].set_ylabel("Frequency")
axs[1, 0].legend()

# Plot histogram for Load cell 3 (bottom vs. top) to visualize distribution
axs[1, 1].hist(bottom_data[:, 1], bins=10, alpha=0.5, label="Bottom - Load cell 3")
axs[1, 1].hist(top_data[:, 1], bins=10, alpha=0.5, label="Top - Load cell 3")
axs[1, 1].set_title("Histogram: Load cell 3 (Bottom vs. Top)")
axs[1, 1].set_xlabel("Load Cell Value")
axs[1, 1].set_ylabel("Frequency")
axs[1, 1].legend()

plt.tight_layout(rect=[0, 0.03, 1, 0.95])
plt.show()
