from ap2_devops.internal import CShell

import logging
import os
import time
import warnings
import unittest

logging.basicConfig(level=logging.INFO, format='internal.command | %(message)s')

log_file = 'output/tests/internal/command_tests.log'
os.makedirs(os.path.dirname(log_file), exist_ok=True)

class CShellTestSuite(unittest.TestCase):
    def setUp(self):
        print()

    @classmethod
    def setUpClass(cls):
        warnings.simplefilter("ignore", ResourceWarning)

    def test_simple_cmd(self):
        runtime = CShell("ls -l", log_file).communicate()
        self.assertIsNotNone(runtime)
        runtime = CShell("ls -l").communicate()
        self.assertIsNotNone(runtime)

    def test_timeout(self):
        runtime = CShell("sleep 2", log_file).communicate(1)
        self.assertIsNone(runtime)

    def test_kill(self):
        CShell("sleep 100", log_file).kill()


if __name__ == '__main__':
    unittest.main()
