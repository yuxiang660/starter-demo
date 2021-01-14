from internal import CShell

import unittest

class CShellTestSuite(unittest.TestCase):

    def test_cshell(self):
        CShell("abc").communicate()


def main():
    unittest.main()


if __name__ == '__main__':
    main()
