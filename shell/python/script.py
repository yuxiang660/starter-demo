#!/grid/common/pkgs/python/v3.6.0/bin/python3.6

import argparse
import os
import re

SUCCESS = 0
ERROR = 1

def check_pin_data(pin, log):
    values = [val for val in log if pin in val]
    if not values or len(values) != 2:
        print(f'!!! Log file does not have input/output data of "{pin}" !!!')
        return False
    data = []
    for val in values:
        matches = re.findall("h[0-9a-fA-F]+", val)
        data.append(matches[0])
    if data[0] != data[1]:
        print(f'!!! "{pin}" input/output data do not match. !!!')
        return False
    print(f'"{pin}" input/output data do match.')
    return True

def parse_log(pins, log_file):
    print(pins)
    print(log_file)
    if not os.path.exists(log_file):
        print(f'!!! Log file {log_file} does not exist !!!')
        return ERROR

    with open(log_file, 'r') as f:
        log = f.readlines()

    for pin in pins:
        print(f'check for {pin}')
        if not check_pin_data(pin, log): return ERROR

    return SUCCESS


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Checker')
    parser.add_argument('pins', nargs='+', help='Pin Names')
    parser.add_argument('-l', '--log', dest='log_file', help='log file', default='output.log')
    args = parser.parse_args()

    ret = parse_log(args.pins, args.log_file)

    exit(ret)
