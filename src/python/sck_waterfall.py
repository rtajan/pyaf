import numpy as np
import matplotlib.pyplot as plt

class sck_waterfall:
    def __init__(self, x, y, *args, **kwargs):
        plt.ion
        self.x = x[:]
        self.y = y[:]
        self.n_frames = self.y.shape[0]
        self.image = np.zeros((512,self.x.shape[1]))
        self.fig = plt.gcf()
        self.ax  = self.fig.gca()
        self.dspl = self.ax.imshow(self.image, interpolation='nearest', *args, **kwargs)

    def refresh(self):
        self.image = np.roll(self.image,self.n_frames,0)
        self.image[0:self.n_frames,:] = self.y[:]
        self.dspl.set_data(self.image)
        plt.draw()

if __name__ == "__main__":
    mtx = np.array([[1,2,3,4], [5,6,7,8], [9,10,11,12]])
    mtx2 = np.roll(mtx,1,axis=0)
    print(mtx)
    print(mtx2[0,:])


