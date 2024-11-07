import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import entropy
from scipy.stats import norm
import os

def load_data(file_path):
    if file_path.endswith('.csv'):
        data = pd.read_csv(file_path,
                           sep="\t", 
                            skiprows=3,          # Skip the first 3 rows to start at the data
                 decimal=',',         # Specify comma as the decimal separator
                 header=None          # Since column headers are complex, load without headers and add them later
                 )
    return data

# Load the data
cwd = os.getcwd()
print(cwd)
file_path = cwd + '/Docs/two_axis_test_sheet.csv'  # path to the .csv or .ods file
print(file_path)
# df = load_data(file_path)

# Load data, skipping the first 2 rows with headers, handling commas as decimals, and specifying tab as the separator
df = pd.read_csv(file_path, sep="\t", skiprows=2, decimal=",", header=None, skipfooter=4, engine="python")

# Define column names
df.columns = [
    "Load cell 7 - Bottom", "Load cell 3 - Bottom",
    "Load cell 7 - Top", "Load cell 3 - Top",
    "Load cell 7 - 90° Drag", "Load cell 3 - 90° Drag"
]

# Drop any rows with non-numeric data if present (e.g., 'mean drag', 'mean lift' rows at the end)
df = df.apply(pd.to_numeric, errors='coerce').dropna()

# Find the "Known mass" value by reading the first row
known_mass = 462.6  # You mentioned the known mass directly, we could also extract it if it’s in the file

# Compute the mean, std, and information content for each column
stats = {}
for col in df.columns:
    data = df[col].dropna()
    mean = data.mean()
    std_dev = data.std()
    # For information content, using entropy as a measure (can be thought of as spread)
    data_histogram = np.histogram(data, bins=10, density=True)[0]
    info_content = entropy(data_histogram)
    
    stats[col] = {"mean": mean, "std": std_dev, "information_content": info_content}

# Print computed statistics
for col, stat in stats.items():
    print(f"{col}: Mean = {stat['mean']}, Std Dev = {stat['std']}, Information Content = {stat['information_content']}")

# Plot "Loading from bottom" vs "Loading from top" to show coupling
plt.figure(figsize=(12, 6))

# Load cell 7 - Bottom vs Top
plt.subplot(1, 2, 1)
plt.scatter(df["Load cell 7 - Bottom"], df["Load cell 7 - Top"], c="blue", label="Load Cell 7")
plt.xlabel("Load Cell 7 - Bottom")
plt.ylabel("Load Cell 7 - Top")
plt.title("Coupling: Load Cell 7 (Bottom vs Top)")
plt.legend()
plt.grid(True)

# Load cell 3 - Bottom vs Top
plt.subplot(1, 2, 2)
plt.scatter(df["Load cell 3 - Bottom"], df["Load cell 3 - Top"], c="green", label="Load Cell 3")
plt.xlabel("Load Cell 3 - Bottom")
plt.ylabel("Load Cell 3 - Top")
plt.title("Coupling: Load Cell 3 (Bottom vs Top)")
plt.legend()
plt.grid(True)

plt.tight_layout()
plt.show()

# Plotting
fig, axs = plt.subplots(2, 2, figsize=(14, 10))
fig.suptitle("Coupling between Load Cells (Bottom vs. Top)")

# Plot Load cell 7 values from bottom and top
axs[0, 0].plot(df["Load cell 7 - Bottom"], label="Bottom - Load cell 7", marker='o')
axs[0, 0].plot(df["Load cell 7 - Top"], label="Top - Load cell 7", marker='x')
axs[0, 0].set_title("Load cell 7: Bottom vs. Top")
axs[0, 0].set_xlabel("Measurement Index")
axs[0, 0].set_ylabel("Load Cell Value")
axs[0, 0].legend()

# Plot Load cell 3 values from bottom and top
axs[0, 1].plot(df["Load cell 3 - Bottom"], label="Bottom - Load cell 3", marker='o')
axs[0, 1].plot(df["Load cell 3 - Top"], label="Top - Load cell 3", marker='x')
axs[0, 1].set_title("Load cell 3: Bottom vs. Top")
axs[0, 1].set_xlabel("Measurement Index")
axs[0, 1].set_ylabel("Load Cell Value")
axs[0, 1].legend()

# Plot histogram for Load cell 7 (bottom vs. top) to visualize distribution
axs[1, 0].hist(df["Load cell 7 - Bottom"], bins=10, alpha=0.5, label="Bottom - Load cell 7")
axs[1, 0].hist(df["Load cell 7 - Top"], bins=10, alpha=0.5, label="Top - Load cell 7")
axs[1, 0].set_title("Histogram: Load cell 7 (Bottom vs. Top)")
axs[1, 0].set_xlabel("Load Cell Value")
axs[1, 0].set_ylabel("Frequency")
axs[1, 0].legend()

# Plot histogram for Load cell 3 (bottom vs. top) to visualize distribution
axs[1, 1].hist(df["Load cell 3 - Bottom"], bins=10, alpha=0.5, label="Bottom - Load cell 3")
axs[1, 1].hist(df["Load cell 3 - Top"], bins=10, alpha=0.5, label="Top - Load cell 3")
axs[1, 1].set_title("Histogram: Load cell 3 (Bottom vs. Top)")
axs[1, 1].set_xlabel("Load Cell Value")
axs[1, 1].set_ylabel("Frequency")
axs[1, 1].legend()

plt.tight_layout(rect=[0, 0.03, 1, 0.95])
plt.show()


# Select a column for which to plot the distribution, e.g., "Load cell 7 - Bottom"
column = "Load cell 7 - Bottom"
data = df[column]

# Calculate mean and standard deviation
mean = data.mean()
std_dev = data.std()

# Create values for the normal distribution curve
x_values = np.linspace(mean - 4*std_dev, mean + 4*std_dev, 100)
normal_dist_curve = norm.pdf(x_values, mean, std_dev)

# Plot the histogram of the data
plt.figure(figsize=(10, 6))
plt.hist(data, bins=10, density=True, alpha=0.6, color="skyblue", edgecolor="black", label="Data Histogram")

# Plot the normal distribution curve
plt.plot(x_values, normal_dist_curve, color="red", label=f"Normal Dist. (mean={mean:.2f}, std={std_dev:.2f})")

# Add titles and labels
plt.title(f"Normal Distribution Fit for {column}")
plt.xlabel(column)
plt.ylabel("Density")
plt.legend()
plt.grid(True)
plt.show()
