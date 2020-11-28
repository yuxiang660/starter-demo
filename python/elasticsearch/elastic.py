import json
import os
import re
from datetime import datetime
from elasticsearch import Elasticsearch, helpers

def datetime_parser(dct):
    datetime_format = "%Y-%m-%dT%H:%M:%S"
    datetime_format_regex = re.compile(r'^\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}$')
    for k, v in dct.items():
        if isinstance(v, str) and datetime_format_regex.match(v):
            dct[k] = datetime.strptime(v, datetime_format)
    return dct

class ElasticReader:
    def __init__(self, log_file):
        assert os.path.exists(log_file)
        with open(log_file, 'r') as f:
            self._raw_data = json.load(f, object_hook=datetime_parser)

    def get_index(self):
        return "ap2_bbb"

    def get_souce_list(self):
        source_list = []
        for keyword, log in self._raw_data.items():
            source = {}
            source.update({"test_name": keyword})
            source.update(log)
            source_list.append(source)
        return source_list

    def add(self):
        actions = []
        source_list = self.get_souce_list()
        for source in source_list: 
            actions.append({
                "_index": self.get_index(),
                "_source": source
            })
        es = Elasticsearch(hosts = [{'host': 'localhost', 'port': 9200}])
        print(helpers.bulk(es, actions))

if __name__ == "__main__":
    reader = ElasticReader("reg3.json")
    reader.get_souce_list()
    reader.add()

