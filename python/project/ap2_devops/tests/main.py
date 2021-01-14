import unittest

loader = unittest.TestLoader()
suite = loader.discover('.', '*_tests.py')
unittest.TextTestRunner().run(suite)
