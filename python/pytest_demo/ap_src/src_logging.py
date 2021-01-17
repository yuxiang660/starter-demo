import logging

def log_src():
    logging.debug('source debug')
    logging.info('source info')
    logging.warning('source warning')
    logging.error('source error')
    logging.critical('source critical')
    assert True
