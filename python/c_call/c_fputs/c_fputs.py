import fputs
import logging

def c_fputs():
    logging.debug('--- call c_fputs ---')
    val = fputs.fputs("Real Pyhton", "write.log")
    logging.debug(f'--- return {val} ---')


