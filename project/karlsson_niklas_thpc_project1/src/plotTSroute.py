import sys
import numpy as np
import matplotlib.pyplot as plt

if len(sys.argv) != 2:
    raise ValueError("Provide a coordinate file.")

fname = sys.argv[1]

cities = []
x_coords = []
y_coords = []

with open(fname, "r") as f:
    f.readline()
    fitness = float(f.readline().split()[2])
    for line in f:
        line = line.strip()
        if line == "EOF":
            break
        line = line.split()
        cities.append(int(line[0]))
        x_coords.append(float(line[1]))
        y_coords.append(float(line[2]))
f.close()

plt.figure(figsize=(8,8))

for i in range(len(x_coords)):
    plt.plot([x_coords[i], x_coords[(i+1) % len(x_coords)]], 
             [y_coords[i], y_coords[(i+1) % len(y_coords)]], 'b-')

for i in range(len(x_coords)):
    plt.scatter(x_coords[i], y_coords[i], s=300, marker='o', c='w', edgecolors='r', zorder=2)  
    plt.text(x_coords[i], y_coords[i], str(cities[i]), horizontalalignment='center', verticalalignment='center', fontsize=10)

plt.title(f'Best route with {len(cities)} cities, fitness {fitness}')
plt.xlim(0,100)
plt.ylim(0,100)

figname = fname[:-4] + "plot.png"
plt.savefig(figname)

