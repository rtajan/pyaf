#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt
from py_aff3ct.module.py_module import Py_Module

class Display(Py_Module):
	def plot(self, x):
		if  self.i_plt % 50 == 0:
			self.line.set_data(x[0,::2], x[0,1::2])
			self.fig.canvas.draw()
			self.fig.canvas.flush_events()
			plt.pause(0.00000001)
		self.i_plt += 1
		return 0

	def __init__(self, N,window):
		Py_Module.__init__(self) # Call the aff3ct Py_Module __init__
		self.name = "py_plot"   # Set your module's name

		t_plot = self.create_task("plot") # create a task for your module
		self.create_socket_in (t_plot, "x", N, np.float32)

		self.create_codelet(t_plot, lambda slf, lsk, fid: slf.plot(lsk[0])) # create codelet

		self.fig   = plt.figure()
		self.ax    = self.fig.add_subplot(1, 1, 1)
		self.line, = self.ax.plot([], '*b')
		self.i_plt = 0
		plt.xlabel("Real part")
		plt.ylabel("Imaginary part")
		plt.ylim(-window,window)
		plt.xlim(-window,window)