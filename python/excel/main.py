import pandas
import json
import re

JSON_KEY_TITLE = "title"
JSON_KEY_PASS = "pass"
JSON_KEY_RUN_TIME = "run_time"
JSON_KEY_BUILD = "build"
JSON_KEY_LOG_FILE = "log_file"

CSV_KEY_COMPILE_DOMAIN = "Compile Domain"
CSV_KEY_RELOC_DOMAIN = "Reloc Domain"
CSV_KEY_TEST_RESULT = "Test Result"

def get_compile_domain(title):
    matches = re.findall(r"\d\S*", title)
    assert len(matches) == 2
    return matches[1]

def get_reloc_domain(title):
    matches = re.findall(r"\d\S*", title)
    assert len(matches) == 2
    return matches[0]

def convert_to_csv_format(raw_data):
    compile_domain = get_compile_domain(raw_data[JSON_KEY_TITLE])
    reloc_domain = get_reloc_domain(raw_data[JSON_KEY_TITLE])
    csv_data = {
        CSV_KEY_COMPILE_DOMAIN : compile_domain,
        CSV_KEY_RELOC_DOMAIN : reloc_domain,
        CSV_KEY_TEST_RESULT : "Pass" if raw_data[JSON_KEY_PASS] else "Fail"
    }
    return csv_data

def to_comb_csv():
    with open('input_comb.json', 'r') as f:
        summary = json.load(f)
    csv_list = []
    for _, test_results in summary.items():
        for raw_data in test_results:
            csv_list.append(convert_to_csv_format(raw_data))
    pandas.DataFrame(csv_list).to_csv('output_comb.csv')

def to_csv():
    with open('input.json', 'r') as f:
        test_results = json.load(f)
    csv_list = []
    for raw_data in test_results:
        csv_list.append(convert_to_csv_format(raw_data))
    pandas.DataFrame(csv_list).to_csv('output.csv')

if __name__ == "__main__":
    to_comb_csv()
    to_csv()

