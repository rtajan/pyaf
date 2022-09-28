import numpy as np
import matplotlib.pyplot as plt

class sck_plot:
    def __init__(self, x, y, *args, **kwargs):
        self.x = np.squeeze(x[:])
        self.y = np.squeeze(y[:])
        self.line, = plt.plot(self.x, self.y, *args, **kwargs)

    def refresh(self):
        self.line.set_xdata(self.x)
        self.line.set_ydata(self.y)
