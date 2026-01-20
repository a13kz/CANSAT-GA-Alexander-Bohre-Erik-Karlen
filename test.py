import matplotlib.pyplot as plt
import numpy as np

plt.style.use('_mpl-gallery')

# make data
x = np.linspace(0, 10, 100)
a = np.array(['g', 'f'])
b = np.array([1, 2, 3, 4, 5, 6])

print(a)
# plot
fig, ax = plt.subplots()
ax.plot(b, a, linewidth=2.0)


ax.set(xlim=(0, 8), xticks=np.arange(1, 8),
       ylim=(0, 8), yticks=np.arange(1, 8))

plt.show()