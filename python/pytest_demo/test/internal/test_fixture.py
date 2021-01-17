import logging
import pytest


@pytest.fixture
def fixture_func():
    logging.info("call fixture_func")
    return "fixture test"

def test_fixture(fixture_func):
    assert fixture_func == "fixture test"
    logging.info("call test fixture")

@pytest.mark.parametrize("test_input,expected", [("3+5", 8), ("2+4", 6), ("6*9", 54)])
def test_eval(test_input, expected):
    assert eval(test_input) == expected

