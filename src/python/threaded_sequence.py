from ast import Not
import sys
from typing import List
sys.path.insert(0, '../py_aff3ct/build/lib') # py_aff3ct location
import py_aff3ct
import threading
from signal_catcher import signal_catcher

class threaded_sequence(py_aff3ct.tools.sequence.Sequence,threading.Thread):
    def __init__(self, *args, **kwargs):
        if len(args) > 0:
            firsts = args[0]
        elif "firsts" in kwargs:
            firsts = kwargs["firsts"]
        if firsts is not List:
                firsts = [firsts]
        self.sgc = signal_catcher()
        self.sgc["do_nothing"] = firsts[0]["status"]
        super(threaded_sequence,self).__init__(*args, **kwargs)
        threading.Thread.__init__(self)

    def run(self):
        py_aff3ct.tools.sequence.Sequence.exec(self)


