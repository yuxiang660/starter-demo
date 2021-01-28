import fputs
import logging
import sys
import os
from .output import OutputGrabber, HideOutput, Capturing

def c_fputs():
    logging.debug('--- call c_fputs ---')
    with OutputGrabber(sys.stderr):
        print("aaaaa")
        val = fputs.fputs("Real Pyhton", "write.log")
    with OutputGrabber(sys.stderr):
        os.system('ls -l')
    logging.debug(f'--- return {val} ---')


