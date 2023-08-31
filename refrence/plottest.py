import matplotlib.pyplot as plt
import cmath as c
import numpy as np

data = (0, 0.5, 0.707, 0.866, 1, 0.866, 0.707, 0.5, 0, -0.5, -0.707, -0.866, -1, -0.866, -0.707, -0.5, 0)
unpacked_data = zip(data)
size = 17
time = (0, c.pi/6, c.pi/4, c.pi/3, c.pi/2, 2 * c.pi/3, 3 * c.pi/4, 5 * c.pi/6, c.pi, 7 * c.pi/6, 5 * c.pi/4, 4 * c.pi/3, 3 * c.pi/2, 10 * c.pi/6, 7 * c.pi/4, 11 * c.pi/6, 2 * c.pi)

print(type(unpacked_data))
plt.scatter(time, data)
plt.show()
