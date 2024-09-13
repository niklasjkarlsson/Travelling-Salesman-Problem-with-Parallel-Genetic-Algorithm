import numpy as np
rng = np.random.default_rng(seed=10)

city_nums = [10,50,100,200]
max_coord = 100.0


def generate(num_cities):
    coords = []
    for i in range(1, num_cities+1):
        x = rng.random()*max_coord
        y = rng.random()*max_coord
        coords.append([x,y])
    return coords

def main():
    for i in city_nums:
        coords = generate(i)
        f = open(f"../run/TSP_data{i}.dat", "w")
        f.write(f"DIMENSION : {i}\n")
        for j in coords:
            f.write(f"{j[0]} {j[1]}\n")
        f.write("EOF\n")
        f.close()
        
if __name__=="__main__":
    main()
