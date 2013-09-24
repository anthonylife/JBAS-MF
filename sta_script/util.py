import numpy as np
import matplotlib.pyplot as plt

def my_histogram_plot(X, seg_num=5):
    histogram, bins = np.histogram(X, bins=seg_num, normed=False)
    bins = 0.5*(bins[1:] + bins[:-1])
    plt.plot(bins, histogram)
    plt.show()
