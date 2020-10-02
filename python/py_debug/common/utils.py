import os

def printPythonPath():
  try:
    print(os.environ['PYTHONPATH'])
  except BaseException as e:
    print(f'An exception occurred: {e}')

