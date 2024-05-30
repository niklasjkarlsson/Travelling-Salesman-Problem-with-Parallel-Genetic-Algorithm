import matplotlib.pyplot as plt
import numpy as np

# Read data from file
data = np.loadtxt('avgs.dat')
numbins = len(data)
binindices = np.arange(1,numbins+1)

# Create histogram
plt.bar(binindices, data, edgecolor='black', label='Wall clock times')

# Ideal curve
def ideal(x):
    return 2**(-x) * 2*data[0]

# Create ideal curve
x_vals = np.linspace(1,10,100)
y_vals = ideal(x_vals)
plt.plot(x_vals, y_vals, 'r--', label='Ideal behaviour')

# Add titles and labels
plt.legend()
plt.title('Wall clock average times')
plt.xlabel('Number of processes')
plt.ylabel('Time in seconds')

# Show plot
plt.savefig('walltimes.png')
