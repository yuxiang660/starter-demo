import argparse
import logging
import json
import os
import plotly.express as px
import pandas as pd

logging.basicConfig(level=logging.INFO,
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
    TOP_DATA = {"maxQtCellPinNum": "value0",
                "stdInstPowerW": "value1",
                "stdInstPinCapW": "value2",
                "qtInstPinIds": "value3"}

    def __init__(self, file) -> None:
        with open(file) as f:
            self.data = json.load(f)
        for _, value in self.data.items():
            self._initMaxQtCellPinNum(value[self.TOP_DATA["maxQtCellPinNum"]])
            self._initStdInstPowerW(value[self.TOP_DATA["stdInstPowerW"]])
            self._initStdInstPinCapW(value[self.TOP_DATA["stdInstPinCapW"]])
            self._initQtInstPinIds(value[self.TOP_DATA["qtInstPinIds"]])

    def _initMaxQtCellPinNum(self, data):
        self.maxQtCellPinNum = data
        logging.debug(f"maxQtCellPinNum: {self.maxQtCellPinNum}")

    def _initStdInstPowerW(self, data):
        self.stdInstPowerWDict = {}
        for item in data:
            gateId = item["key"]
            self.stdInstPowerWDict.setdefault(gateId, {})
            for value in item["value"]:
                stdInstId = value["key"]
                stdInstW = value["value"]
                self.stdInstPowerWDict[gateId].update({stdInstId: stdInstW})
        logging.debug(f"stdInstPowerWDict: {self.stdInstPowerWDict}")

    def _initStdInstPinCapW(self, data):
        self.stdInstPinCapWDict = {}
        for item in data:
            gateId = item["key"]
            self.stdInstPinCapWDict.setdefault(gateId, {})
            for value in item["value"]:
                stdInstId = value["key"]
                stdInstPinCap = value["value"]
                self.stdInstPinCapWDict[gateId].update(
                    {stdInstId: stdInstPinCap})
        logging.debug(f"stdInstPinCapWDict: {self.stdInstPinCapWDict}")

    def _initQtInstPinIds(self, data):
        self.qtInstPinIdsDict = {}
        for item in data:
            gateId = item["key"]
            self.qtInstPinIdsDict.setdefault(gateId, {})
            for value in item["value"]:
                stdInstId = value["key"]
                self.qtInstPinIdsDict[gateId].setdefault(stdInstId, {})
                qtInstPinIds = value["value"]
                for qtInstPinId in qtInstPinIds:
                    qtInstId = qtInstPinId["key"]
                    pinIdList = qtInstPinId["value"]
                    self.qtInstPinIdsDict[gateId][stdInstId].update(
                        {qtInstId: pinIdList})
        logging.debug(f"qtInstPinIdsDict: {self.qtInstPinIdsDict}")


class CellDBInst:
    TOP_DATA = {"stdCellIdToName": "value0",
                "qtCellIdToName": "value1",
                "qtCellPinIdToName": "value2",
                "stdInstIdToStdCellId": "value3",
                "qtInstIdToQtCellId": "value4"}

    def __init__(self, file) -> None:
        with open(file) as f:
            self.data = json.load(f)
        for _, value in self.data.items():
            self._initStdCellIdToName(value[self.TOP_DATA["stdCellIdToName"]])
            self._initQtCellIdToName(value[self.TOP_DATA["qtCellIdToName"]])
            self._initQtCellPinIdToName(
                value[self.TOP_DATA["qtCellPinIdToName"]])
            self._initStdInstIdToStdCellId(
                value[self.TOP_DATA["stdInstIdToStdCellId"]])
            self._initQtInstIdToQtCellId(
                value[self.TOP_DATA["qtInstIdToQtCellId"]])

    def getNameByStdInstId(self, instId):
        return self.stdCellIdToNameList[self.stdInstIdToStdCellIdList[instId]]

    def getNameByQtInstId(self, instId):
        return self.qtCellIdToNameList[self.qtInstIdToQtCellIdList[instId]]

    def getNameByQtPinId(self, qtInstId, pinId):
        return self.qtCellPinIdToNameDict[self.qtInstIdToQtCellIdList[qtInstId]][pinId]

    def _initStdCellIdToName(self, data):
        self.stdCellIdToNameList = data
        logging.debug(f"stdCellIdToNameList: {self.stdCellIdToNameList}")

    def _initQtCellIdToName(self, data):
        self.qtCellIdToNameList = data
        logging.debug(f"qtCellIdToNameList: {self.qtCellIdToNameList}")

    def _initQtCellPinIdToName(self, data):
        self.qtCellPinIdToNameDict = {}
        for item in data:
            self.qtCellPinIdToNameDict[item["key"]] = item["value"]
        logging.debug(f"qtCellPinIdToNameDict: {self.qtCellPinIdToNameDict}")

    def _initStdInstIdToStdCellId(self, data):
        self.stdInstIdToStdCellIdList = data
        logging.debug(f"stdInstIdToStdCellIdList: {self.stdInstIdToStdCellIdList}")

    def _initQtInstIdToQtCellId(self, data):
        self.qtInstIdToQtCellIdList = data
        logging.debug(f"qtInstIdToQtCellIdList: {self.qtInstIdToQtCellIdList}")


class Viewer:
    CELLDB_LOG_FOLDER = "swfvCellDB.log"

    def __init__(self, logDir) -> None:
        self.logFolder = os.path.join(logDir, self.CELLDB_LOG_FOLDER)
        os.makedirs(self.logFolder, exist_ok=True)

    def _mergeByGateId(self, cellDBInst, cellDB):
        stdInstInfo = {}  # key: stdInstId - {qtInstId: qtPinNameSet}, {'stdPowerW': num}, {'stdPinCapW': num}
        for gateId, stdInsts in cellDB.qtInstPinIdsDict.items():
            for stdInstId, qtInsts in stdInsts.items():
                stdInstInfo.setdefault(stdInstId, {})
                stdInstInfo[stdInstId].setdefault('stdPowerW', 0)
                stdInstInfo[stdInstId].setdefault('stdPinCapW', 0)
                stdInstInfo[stdInstId]['stdPowerW'] += cellDB.stdInstPowerWDict[gateId][stdInstId]
                stdInstInfo[stdInstId]['stdPinCapW'] += cellDB.stdInstPinCapWDict[gateId][stdInstId]
                for qtInstId, pinIds in qtInsts.items():
                    stdInstInfo[stdInstId].setdefault(qtInstId, set())
                    qtPinNames = [cellDBInst.getNameByQtPinId(qtInstId, pinId) for pinId in pinIds]
                    qtPinNames = stdInstInfo[stdInstId][qtInstId] | set(qtPinNames)
                    stdInstInfo[stdInstId].update({qtInstId: qtPinNames})
        return stdInstInfo

    def viewInst(self, cellDBInst, cellDB):
        dfData = []
        for stdInstId, stdInstInfo in self._mergeByGateId(cellDBInst, cellDB).items():
            dfItem = {}
            stdInst = cellDBInst.getNameByStdInstId(stdInstId) + "(" + str(stdInstId) + ")"
            dfItem.update({'stdInstName': stdInst})
            stdInstPowerW = stdInstInfo['stdPowerW']
            stdInstPinCapW = stdInstInfo['stdPinCapW']
            numQtInsts = len(stdInstInfo) - 2
            dfItem.update({'qtInfo': []})
            for qtInstId, qtPins in stdInstInfo.items():
                dfQtItem = {}
                if not isinstance(qtInstId, int):
                    continue
                qtInst = cellDBInst.getNameByQtInstId(qtInstId) + "(" + str(qtInstId) + ") "
                dfQtItem.update({'qtInstName': qtInst})
                dfQtItem.update({'qtPinNames': ", ".join(qtPins)})
                dfQtItem.update({'qtPowerAvgW': stdInstPowerW / numQtInsts})
                dfQtItem.update({'qtPinCapAvgW': stdInstPinCapW / numQtInsts})
                dfItem['qtInfo'].append(dfQtItem)
            dfData.append(dfItem)
        data = pd.json_normalize(dfData, "qtInfo", ["stdInstName"])
        print(data)
        df = pd.DataFrame(data)
        fig = px.treemap(df, path=[px.Constant("top"), 'stdInstName', 'qtInstName'],
                 values='qtPowerAvgW', hover_data=['qtPinNames'])
        fig.update_traces(root_color="lightgrey")
        fig.update_layout(margin=dict(t=50, l=25, r=25, b=25))
        fig.write_image(os.path.join(self.logFolder, "instPowerW.svg"))
        fig.write_html(os.path.join(self.logFolder, "instPowerW.html"))

        fig2 = px.treemap(df, path=[px.Constant("top"), 'stdInstName', 'qtInstName'],
                 values='qtPinCapAvgW', hover_data=['qtPinNames'])
        fig2.update_traces(root_color="lightgrey")
        fig2.update_layout(margin=dict(t=50, l=25, r=25, b=25))
        fig2.write_image(os.path.join(self.logFolder, "instPinCapW.svg"))
        fig2.write_html(os.path.join(self.logFolder, "instPinCapW.html"))


def main(args):
    checkArgs(args)
    cellDB = CellDB(os.path.join(args.dir, CELLDB_FILENAME))
    cellDBInst = CellDBInst(os.path.join(args.dir, CELLDB_INST_FILENAME))
    viewer = Viewer(args.logDir)
    viewer.viewInst(cellDBInst, cellDB)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='View swfvCellDB')
    parser.add_argument('-d', '--directory', dest='dir',
                        default="./dbFiles", help='The dirctory of CellDB JSON files')
    parser.add_argument('-ld', '--log_directory', dest='logDir',
                        default="./tmp", help='The log directory of the utitlity')
    args = parser.parse_args()
    main(args)
