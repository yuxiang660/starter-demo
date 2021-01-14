from tester_reloc.main import RelocTester

import unittest

class SetupTestSuite(unittest.TestCase):

    def test_setup(self):
        mock_config = "mock_config"
        RelocTester.compile(mock_config)


def main():
    unittest.main()


if __name__ == '__main__':
    main()
