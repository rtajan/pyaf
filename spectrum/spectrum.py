# %%
import sys
sys.path.insert(0, '../build/lib') # pyaf location
sys.path.insert(0, '../py_aff3ct/build/lib') # py_aff3ct location
sys.path.insert(0, '../py_aff3ct/examples/full_python') # py_aff3ct location
import py_aff3ct
from py_aff3ct import module as mdl
import pyaf
from py_signal_catcher import signal_catcher

import matplotlib.pyplot as plt
import numpy as np


Fc = 815e6
# Fc = 868e6
Fs = 25e6

# %%
rad_params = pyaf.radio.USRP_params()
rad_params.fifo_size  = 1
rad_params.N          = 1024
rad_params.threaded   = True
rad_params.rx_enabled = True
rad_params.usrp_addr  = "addr = 192.168.10.2"
rad_params.rx_rate    = Fs
rad_params.rx_antenna = "TX/RX"
rad_params.rx_freq    = Fc
rad_params.rx_gain    = 10

rad      = pyaf.radio.Radio_USRP(rad_params)
spectrum = pyaf.spectrum.Spectrum(2048, 0.7, 1, 1024)
sgc      = signal_catcher()

# %%
f   = (spectrum.get_freq()-0.5)*Fs + Fc
X_f = np.fft.fftshift(10*np.log10(spectrum.get_spectrum()+1e-15))

fig = plt.figure()
ax = fig.add_subplot(111)
line1, = ax.plot(f, X_f) # Returns a tuple of line objects, thus the comma
plt.ylim((-100, 20))
plt.xlabel("f in Hz")
plt.ylabel("Spectrum")
plt.grid()

# %%
rad["receive::Y_N1"] = spectrum["analyze::X"     ]
sgc["do_nothing"   ] = spectrum["analyze::status"]

sequence = py_aff3ct.tools.pipeline.Pipeline(
    [rad["receive"]],
    [sgc["do_nothing"]],
    [[[rad["receive"     ]], [rad["receive"   ]], []],
     [[spectrum["analyze"]], [sgc["do_nothing"]], []]],
    [1,1],
    [10000],
    [False]
)

#sequence = py_aff3ct.tools.sequence.Sequence(rad["receive"])

l_tasks = sequence.get_tasks_per_types()
for lt in l_tasks:
    for t in lt:
        t.stats = True
        #t.debug = True
        #t.set_debug_limit(10)
#sequence.export_dot("chaine.dot")
#sequence.exec()

# %%
import time
import threading
class seq_execution(threading.Thread):
    def __init__(self, seq):
        threading.Thread.__init__(self)
        self.seq = seq
        self.starting_time = time.time()

    def run(self):
        self.starting_time = time.time()
        self.seq.exec()

se = seq_execution(sequence)
se.start()
while se.is_alive():
    starting_time = time.time()
    X_f = np.fft.fftshift(10*np.log10(spectrum.get_spectrum()+1e-15))
    line1.set_ydata(X_f)
    delta = starting_time - time.time()
    plt.pause(0.2-delta)

sequence.show_stats()

del sequence
plt.show()
