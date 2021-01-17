import logging

class TestABC:
    @classmethod
    def setup_class(cls):
        logging.info("starting class: {} execution".format(cls.__name__))

    @classmethod
    def teardown_class(cls):
        logging.info("teardown class: {} execution".format(cls.__name__))

    def setup_method(self, method):
        logging.info("starting execution of tc: {}".format(method.__name__))

    def teardown_method(self, method):
        logging.info("teardown execution of tc: {}".format(method.__name__))

    def test_foo(self):
        logging.info("testting foo")
        assert True

    def test_bar(self):
        logging.info("testting bar")
        assert True
