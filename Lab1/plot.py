import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D


fig = plt.figure(figsize = (64,40))
ax = plt.axes(projection='3d')

x = np.arange(0, 60.01, 0.01)
t = np.arange(0, 50.01, 0.01)

T, X = np.meshgrid(t, x)

Z = np.genfromtxt ("build/output.csv", delimiter=",", dtype=np.double)

#surf = ax.plot_surface(T, X, Z, cmap = plt.cm.cividis)
surf = ax.plot_surface(T, X, Z, cmap=plt.cm.coolwarm)

ax.set_xlabel('x', labelpad = 20)
ax.set_ylabel('t', labelpad = 20)
ax.set_zlabel('z', labelpad = 20)

fig.colorbar(surf, shrink = 0.8, aspect = 20)

plt.show()