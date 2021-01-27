from c_fputs import c_fputs
import logging

logging.basicConfig(level=logging.DEBUG, format='c_call | %(message)s')



if __name__ == "__main__":
    c_fputs()
