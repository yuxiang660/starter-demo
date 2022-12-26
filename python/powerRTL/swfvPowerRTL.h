#pragma once

#include "swfvPowerDB.h"
#include "swfvCellDB.h"

namespace NPE
{

class PowerParserRTL;
class PowerEngineRTL
{
public:
   explicit PowerEngineRTL(const char *dbFolder);
   ~PowerEngineRTL() = default;
   PowerEngineRTL(const PowerEngineRTL&) = delete;
   PowerEngineRTL& operator=(const PowerEngineRTL&) = delete;

private:
   void importTCL();

private:
   const LibertyInf &m_libertyInf;
   PowerTCL m_powerTcl;
   std::unique_ptr<LibertyMgr> m_libMgr;
   std::unique_ptr<swfvCellDB> m_cellDB;
   std::unique_ptr<swfvCellDBInst> m_cellDBInst;
   std::unique_ptr<PowerParserRTL> m_powerParser;
};

class PowerParserRTL
{
public:
   explicit PowerParserRTL(const LibertyMgr &libMgr, const swfvDBReader &swfvDB,
      swfvCellDB &cellDB, swfvCellDBInst &cellDBInst);
   ~PowerParserRTL() = default;
   PowerParserRTL(const PowerParserRTL&) = delete;
   PowerParserRTL& operator=(const PowerParserRTL&) = delete;

public:
   void traversePass1();
   void traversePass2();
   void dump(const char *folder) const;

private:
   void traversePass1Helper(smdb::iHierPath &path);
   void analyzeAfterPass1();

   void traversePass2Helper(smdb::iHierPath &path);
   void addStdInstDB(const smdb::iHierPath &path, const smdb::iCell *cell);
   void addQtInstDB(const smdb::iHierPath &path, const smdb::iCell *cell);
   double lookupPinInternalPower(const smdb::iCell *cell, const smdb::iTerm *term);
   double lookupPinCap(const smdb::iCell *cell, const smdb::iTerm *term);

private:
   bool isInternalCell(const smdb::iCell *cell) const;
   bool isLibertyGateCell(const smdb::iCell *cell) const;
   void dumpLibCache(const char *folder) const;
   void dumpInstId(const char *folder) const;

private:
   const LibertyMgr &m_libMgr;
   const swfvDBReader &m_swfvDB;
   swfvCellDB &m_cellDB;
   swfvCellDBInst &m_cellDBInst;
   typedef std::unordered_map<const smdb::iCell *, std::unordered_map<const smdb::iTerm *, double>> LibCache;
   LibCache m_powerCache;
   LibCache m_capCache;

private:
   const smdb::iCell *m_activeStdCell = nullptr;
   const uint64_t INVALID_INST_ID = UINT64_MAX;
   uint64_t m_activeStdInstId = INVALID_INST_ID;

   std::unordered_map<const smdb::iCell *, std::unordered_map<const smdb::iCell *, uint32_t>> m_stdToQTCellMap;
   uint32_t m_maxQtCellPinNum = 0;

   /**
    * cell name to cell id members
   */
   std::unordered_map<std::string, uint32_t> m_stdCellNameToId;
   std::unordered_map<std::string, uint32_t> m_qtCellNameToId;
   // key1: qtCellName, ke2: qtPinName, value: qtPinId
   std::unordered_map<std::string, std::unordered_map<std::string, uint32_t>> m_qtCellPinNameToId;
};

}
