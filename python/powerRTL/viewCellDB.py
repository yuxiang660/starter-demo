import argparse
import logging
import json
import os


logging.basicConfig(level=logging.DEBUG,
                    format='[%(levelname)7s] | %(message)s')

CELLDB_FILENAME = "swfvCellDB.json"
CELLDB_INST_FILENAME = "swfvCellDB.inst.json"


def checkArgs(args):
    if (not os.path.exists(args.dir)):
        logging.error(f"{args.dir} does not exit")
        exit()
    if (not os.path.exists(os.path.join(args.dir, CELLDB_FILENAME))):
        logging.error(f"{CELLDB_FILENAME} does not exit")
        exit()
    if (not os.path.exists(os.path.join(args.dir, CELLDB_INST_FILENAME))):
        logging.error(f"{CELLDB_INST_FILENAME} does not exit")
        exit()


class CellDB:
    def __init__(self, file) -> None:
        with open(file) as f:
            self.data = json.load(f)


class CellDBInst:
    TOP_DATA = {"stdCellIdToName": "value0",
                "qtCellIdToName": "value1",
                "qtCellPinIdToName": "value2",
                "stdInstIdToStdCellId": "value3",
                "qtInstIdToQtCellId": "value4"}

    def __init__(self, file) -> None:
        with open(file) as f:
            self.data = json.load(f)
        for key, value in self.data.items():
            self._initStdCellIdToName(value[self.TOP_DATA["stdCellIdToName"]])
            self._initQtCellIdToName(value[self.TOP_DATA["qtCellIdToName"]])
            self._initQtCellPinIdToName(value[self.TOP_DATA["qtCellPinIdToName"]])
            self._initStdInstIdToStdCellId(value[self.TOP_DATA["stdInstIdToStdCellId"]])
            self._initQtInstIdToQtCellId(value[self.TOP_DATA["qtInstIdToQtCellId"]])

    def _initStdCellIdToName(self, data):
        self.stdCellIdToNameList = data
        logging.debug(self.stdCellIdToNameList)

    def _initQtCellIdToName(self, data):
        self.qtCellIdToNameList = data
        logging.debug(self.qtCellIdToNameList)

    def _initQtCellPinIdToName(self, data):
        self.qtCellIdToNameDict = {}
        for item in data:
            self.qtCellIdToNameDict[item["key"]] = item["value"]
        logging.debug(self.qtCellIdToNameDict)

    def _initStdInstIdToStdCellId(self, data):
        self.stdInstIdToStdCellIdList = data
        logging.debug(self.stdInstIdToStdCellIdList)

    def _initQtInstIdToQtCellId(self, data):
        self.qtInstIdToQtCellIdList = data
        logging.debug(self.qtInstIdToQtCellIdList)


def main(args):
    checkArgs(args)
    CellDB(os.path.join(args.dir, CELLDB_FILENAME))
    CellDBInst(os.path.join(args.dir, CELLDB_INST_FILENAME))
    pass


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='View swfvCellDB')
    parser.add_argument('-d', '--directory', dest='dir',
                        default="./dbFiles", help='The dirctory of swfvCellDB JSON files')
    args = parser.parse_args()
    main(args)
