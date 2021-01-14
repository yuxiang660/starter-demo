from tester.setup import Setup

import unittest

class SetupTestSuite(unittest.TestCase):

    def test_setup(self):
        mock_config = "mock_config"
        Setup(mock_config).gen_setup_file()
        Setup(mock_config).clean()


def main():
    unittest.main()


if __name__ == '__main__':
    main()
