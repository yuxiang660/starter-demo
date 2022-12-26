#include "swfvPowerRTL.h"

namespace
{

int ignoreTclCmd(ClientData cldata, Tcl_Interp *interp, int ac, const char **av)
{
   fvDEBUGx(PWRDB, 0, "-ignore TCL command %s\n", (const char *)cldata);
   return TCL_OK;
}

std::string toPathStr(const smdb::iHierPath &path)
{
   std::string fullname;
   if (!path.empty())
   {
      for(size_t i = 0; i < path.size(); ++i)
      {
         fullname.append((path[i])->GetName());
         if (i + 1 < path.size())
            fullname.append(".");
      }
   }
   return fullname;
}

}

namespace NPE
{

PowerEngineRTL::PowerEngineRTL(const char *dbFolder) :
   m_libertyInf(LibertyInf::getSingleton(dbFolder)),
   m_powerTcl(dbFolder),
   m_cellDB(std::make_unique<swfvCellDB>()),
   m_cellDBInst(std::make_unique<swfvCellDBInst>())
{
   importTCL();
   m_libMgr = std::make_unique<LibertyMgr>(m_powerTcl.getLibertyItems());
   const swfvDBReader &swfvDB = swfvDBReader::getSwfvDB(); // TODO: support partial swfvDB
   m_powerParser = std::make_unique<PowerParserRTL>(*m_libMgr, swfvDB, *m_cellDB, *m_cellDBInst);

   m_powerParser->traversePass1(); //TODO
   m_powerParser->traversePass2(); //TODO
   m_powerParser->dump(dbFolder); // TODO
   m_cellDB->save(dbFolder); // TODO
   if (dumpJsonCellDB())
   {
      m_cellDB->saveToJSON(dbFolder);
      m_cellDBInst->saveToJSON(dbFolder);
   }
}

void PowerEngineRTL::importTCL()
{
   fvDebugAutoLevel level(fvDebug::PWRDB, "PowerEngineRTL::importTCL()");

   Tcl_CreateCommand(m_powerTcl.getInterp(), "import_power_liberty", PowerTCL::cmdImportLiberty, (void *)(&m_powerTcl), nullptr);
   Tcl_CreateCommand(m_powerTcl.getInterp(), "import_spef_file", ignoreTclCmd, (void *)"import_spef_file", nullptr);
   Tcl_CreateCommand(m_powerTcl.getInterp(), "import_spef_voltage", ignoreTclCmd, (void *)"import_spef_voltage", nullptr);
   Tcl_CreateCommand(m_powerTcl.getInterp(), "import_power_table", ignoreTclCmd, (void *)"import_power_table", nullptr);

   m_powerTcl.eval();
}

PowerParserRTL::PowerParserRTL(const LibertyMgr &libMgr, const swfvDBReader &swfvDB,
   swfvCellDB &cellDB, swfvCellDBInst &cellDBInst) :
   m_libMgr(libMgr),
   m_swfvDB(swfvDB),
   m_cellDB(cellDB),
   m_cellDBInst(cellDBInst)
{
}

bool PowerParserRTL::isInternalCell(const smdb::iCell *cell) const
{
   if (cell->iGetLib()->iGetHierNum() <= DBASICLIB)
      return true;

   const char *cellName = cell->GetName();
   if (!strncmp(cellName, "MPR", 3) || !strncmp(cellName, "MPW", 3))
      return true;

   return false;
}

bool PowerParserRTL::isLibertyGateCell(const smdb::iCell *cell) const
{
   if (m_libMgr.getCellLib(cell->GetName()).empty())
      return false;

   auto cellType = m_libMgr.getCellType(cell->GetName());
   if (cellType == LibertyFile::BlackBox)
   {
      fvDEBUGx(PWRDB, 0, "-bypass black box cell %s\n", cell->GetName());
      return false;
   }

   return true;
}

void PowerParserRTL::traversePass1()
{
   fvDebugAutoLevel level(fvDebug::PWRDB, "PowerParserRTL::traversePass1()");
   smdb::iHierPath path;
   traversePass1Helper(path);
   analyzeAfterPass1();
}

void PowerParserRTL::analyzeAfterPass1()
{
   fvDebugAutoLevel level(fvDebug::PWRDB, "PowerParserRTL::analyzeAfterPass1()");

   std::vector<std::pair<const smdb::iCell *, const smdb::iCell *>> simpleStdCells;
   std::unordered_map<const smdb::iCell *, std::unordered_map<const smdb::iCell *, uint32_t>> complexStdCells;
   for (const auto &[stdCell, qtCells] : m_stdToQTCellMap)
   {
      if (qtCells.size() == 1 && qtCells.begin()->second == 1)
         simpleStdCells.emplace_back(stdCell, qtCells.begin()->first);
      else
         complexStdCells[stdCell] = qtCells;
   }
   fvDEBUGx(PWRDB, 0, "-total %zu simple STD cells, %zu complex STD cells in all %zu STD cells\n",
      simpleStdCells.size(), complexStdCells.size(), m_stdToQTCellMap.size());

   for (const auto &[stdCell, qtCells] : complexStdCells)
   {
      const char *stdCellName = stdCell->GetName();
      uint32_t stdCellId = m_cellDBInst.m_stdCellIdToName.size();
      if (m_stdCellNameToId.find(stdCellName) == m_stdCellNameToId.end())
      {
         fvDEBUGl(10, PWRDB, 0, "-add STD cell %s with std cell index %u\n", stdCellName, stdCellId);
         m_stdCellNameToId[stdCellName] = stdCellId;
         m_cellDBInst.m_stdCellIdToName.push_back(stdCellName);
         assert(qtCells.size() > 1); // complex STD cell
         for (const auto &iter : qtCells)
         {
            const smdb::iCell *qtCell = iter.first;
            const char *qtCellName = qtCell->GetName();
            uint32_t qtCellId = m_cellDBInst.m_qtCellIdToName.size();
            if (m_qtCellNameToId.find(qtCellName) == m_qtCellNameToId.end())
            {
               fvDEBUGl(10, PWRDB, 0, "-add QTRef cell %s with qtref cell index %u\n", qtCellName, qtCellId);
               m_qtCellNameToId[qtCellName] = qtCellId;
               m_cellDBInst.m_qtCellIdToName.push_back(qtCellName);
               uint32_t qtCellPinId = 0;
               for (const auto &term : qtCell->iGetTerms())
               {
                  const char *qtCellPinName = term.GetName();
                  fvDEBUGl(10, PWRDB, 0, "-add QTRef cell %s(%u)'s pin: %s(%u)\n", qtCellName, qtCellId, qtCellPinName, qtCellPinId);
                  m_qtCellPinNameToId[qtCellName][qtCellPinName] = qtCellPinId;
                  m_cellDBInst.m_qtCellPinIdToName[qtCellId].push_back(term.GetName());
                  qtCellPinId++;
               }
               if (m_maxQtCellPinNum < qtCellPinId)
                  m_maxQtCellPinNum = qtCellPinId;
            }
         }
      }
   }
   m_cellDB.setMaxQtCellPinNum(m_maxQtCellPinNum);
   fvDEBUGx(PWRDB, 0, "-total %zu complex std cells, that consists of %zu qtref cells, max qtref cell pin number is %u\n",
      m_stdCellNameToId.size(), m_qtCellPinNameToId.size(), m_maxQtCellPinNum);
}

void PowerParserRTL::traversePass1Helper(smdb::iHierPath &path)
{
   const smdb::iCell *cell = path.empty() ? smdb::smGetAppl().iGetTopCell() : path.back()->iGetTarget();
   if (isInternalCell(cell))
   {
      if (cell->iGetLib()->iGetHierNum() == iLibHierNums::REF && m_activeStdCell != nullptr)
         (m_stdToQTCellMap[m_activeStdCell][cell])++;
      return;
   }

   if (isLibertyGateCell(cell))
   {
      if (m_stdToQTCellMap.find(cell) == m_stdToQTCellMap.end())
         m_activeStdCell = cell;
   }

   path.push_back(0);
   for (const auto &inst : cell->iGetInstances())
   {
      path.back() = &inst;
      traversePass1Helper(path);
   }
   path.pop_back();

   // recover to default value when leaving liberty cell tracing
   if (isLibertyGateCell(cell))
      m_activeStdCell = nullptr;
}

void PowerParserRTL::traversePass2()
{
   fvDebugAutoLevel level(fvDebug::PWRDB, "PowerParserRTL::traversePass2()");
   smdb::iHierPath path;
   traversePass2Helper(path);
}

void PowerParserRTL::traversePass2Helper(smdb::iHierPath &path)
{
   const smdb::iCell *cell = path.empty() ? smdb::smGetAppl().iGetTopCell() : path.back()->iGetTarget();
   if (isInternalCell(cell))
   {
      if (m_activeStdInstId != INVALID_INST_ID)
      {
         addQtInstDB(path, cell);
         return;
      }
   }

   if (isLibertyGateCell(cell))
   {
      if (m_stdCellNameToId.find(cell->GetName()) != m_stdCellNameToId.end())
      {
         m_activeStdInstId = m_cellDBInst.m_stdInstIdToStdCellId.size();
         addStdInstDB(path, cell);
      }
   }

   path.push_back(0);
   for (const auto &inst : cell->iGetInstances())
   {
      path.back() = &inst;
      traversePass2Helper(path);
   }
   path.pop_back();

   // recover to default value when leaving liberty cell tracing
   if (isLibertyGateCell(cell))
      m_activeStdInstId = INVALID_INST_ID;
}

void PowerParserRTL::addStdInstDB(const smdb::iHierPath &path, const smdb::iCell *cell)
{
   fvDebugAutoLevel level(10, fvDebug::PWRDB, "PowerParserRTL::addStdInstDB(%s, %s)",
      toPathStr(path).c_str(), cell->GetName());

   uint32_t stdCellId = m_stdCellNameToId.at(cell->GetName());
   uint64_t stdInstId = m_cellDBInst.m_stdInstIdToStdCellId.size();
   m_cellDBInst.m_stdInstIdToStdCellId.push_back(stdCellId);
   fvDEBUGl(10, PWRDB, 0, "-add STD cell(%u) instance(%s) with index %zu\n",
      stdCellId, toPathStr(path).c_str(), stdInstId);

   for (const auto &term : cell->iGetTerms())
   {
      const smdb::iTermInst *termInst = term.iGetTarget(path.back());
      const smdb::iNet *netH = termInst->iGetNet();
      fvDEBUGl(10, PWRDB, 0, "-instance(%lu) terminal %s is connected with net %s\n",
         stdInstId, term.GetName(), netH ? netH->GetName() : "none");
      if (netH != nullptr)
      {
         crt::smSynID synId = smdb::smGetAppl().GetRtSynonymID(path, termInst->iGetTarget()->iGetNet());
         if (synId == crt::smSynID_invalid)
            continue;
         crt::smNetID netId = crt::GetNetBySynonym(synId);
         netId = crt::IsIdValid(netId);
         if (crt::IsIdConstant(netId) || netId == crt::smNetID_invalid)
            continue;
         netId = rel_smNetID(netId);
         uint64_t gateId = m_swfvDB.lookupNet(netId);
         double power = lookupPinInternalPower(cell, &term);
         double cap = lookupPinCap(cell, &term);
         m_cellDB.addInternalPowerWeight(gateId, m_activeStdInstId, power);
         m_cellDB.addPinCapWeight(gateId, m_activeStdInstId, cap);
         fvDEBUGl(10, PWRDB, 0, "-add STD database: gateId=%lu, stdInstId=%lu, power(fJ)=%f, cap(ff)=%f\n",
            gateId, m_activeStdInstId, power, cap);
      }
   }
}

double PowerParserRTL::lookupPinInternalPower(const smdb::iCell *cell, const smdb::iTerm *term)
{
   if (m_powerCache.find(cell) != m_powerCache.end())
      if (m_powerCache.at(cell).find(term) != m_powerCache.at(cell).end())
         return m_powerCache.at(cell).at(term);

   double avgPower = m_libMgr.lookupAveragePinInternalPower(cell->GetName(), term->GetName(),
      (m_libMgr.getLibertyPara(cell->GetName())).getSlewIndex(),
      (m_libMgr.getLibertyPara(cell->GetName())).getCapIndex());
   double energyfJ = avgPower * (m_libMgr.getEnergyUnitfJ(cell->GetName()));
   m_powerCache[cell][term] = energyfJ;

   return m_powerCache.at(cell).at(term);
}

double PowerParserRTL::lookupPinCap(const smdb::iCell *cell, const smdb::iTerm *term)
{
   if (m_capCache.find(cell) != m_capCache.end())
      if (m_capCache.at(cell).find(term) != m_capCache.at(cell).end())
         return m_capCache.at(cell).at(term);

   const auto &inputPinCaps = m_libMgr.getCellInPinAgvCaps(cell->GetName());
   if (inputPinCaps.find(term->GetName()) != inputPinCaps.end())
   {
      double cap = inputPinCaps.at(term->GetName());
      double capff = cap * m_libMgr.getCellInPinAgvCapUnitff(cell->GetName());
      m_capCache[cell][term] = capff;
   }
   else
   {
      m_capCache[cell][term] = 0.0;
   }

   return m_capCache.at(cell).at(term);
}

void PowerParserRTL::addQtInstDB(const smdb::iHierPath &path, const smdb::iCell *cell)
{
   fvDebugAutoLevel level(10, fvDebug::PWRDB, "PowerParserRTL::addQtInstDB(%s, %s)",
      toPathStr(path).c_str(), cell->GetName());

   const char *qtCellName = cell->GetName();
   assert(m_qtCellNameToId.find(qtCellName) != m_qtCellNameToId.end());
   uint32_t qtCellId = m_qtCellNameToId.at(qtCellName);
   uint64_t qtInstId = m_cellDBInst.m_qtInstIdToQtCellId.size();
   m_cellDBInst.m_qtInstIdToQtCellId.push_back(qtCellId);
   fvDEBUGl(10, PWRDB, 0, "-add QTRef cell(%u) instance(%s) with index %zu\n",
      qtCellId, toPathStr(path).c_str(), qtInstId);

   for (const auto &term : cell->iGetTerms())
   {
      const smdb::iTermInst *termInst = term.iGetTarget(path.back());
      const smdb::iNet *netH = termInst->iGetNet();
      fvDEBUGl(10, PWRDB, 0, "-instance(%lu) terminal %s is connected with net %s\n",
         qtInstId, term.GetName(), netH ? netH->GetName() : "none");
      if (netH != nullptr)
      {
         crt::smSynID synId = smdb::smGetAppl().GetRtSynonymID(path, termInst->iGetTarget()->iGetNet());
         if (synId == crt::smSynID_invalid)
            continue;
         crt::smNetID netId = crt::GetNetBySynonym(synId);
         netId = crt::IsIdValid(netId);
         if (crt::IsIdConstant(netId) || netId == crt::smNetID_invalid)
            continue;
         netId = rel_smNetID(netId);
         uint64_t gateId = m_swfvDB.lookupNet(netId);
         const char *qtPinName = term.GetName();
         assert(m_qtCellPinNameToId.find(qtCellName) != m_qtCellPinNameToId.end());
         const auto &pinIds = m_qtCellPinNameToId.at(qtCellName);
         assert(pinIds.find(qtPinName) != pinIds.end());
         uint32_t pinId = pinIds.at(qtPinName);
         m_cellDB.addQtInstPinId(gateId, m_activeStdInstId, qtInstId, pinId);
         fvDEBUGl(10, PWRDB, 0, "-add QTRef database: gateId=%lu, stdInstId=%lu, qtInstId=%ld, pinId=%u\n",
            gateId, m_activeStdInstId, qtInstId, pinId);
      }
   }
}

void PowerParserRTL::dump(const char *folder) const
{
   assert(access(folder, F_OK) == 0);

   dumpLibCache(folder);
   dumpInstId(folder);
}

void PowerParserRTL::dumpLibCache(const char *folder) const
{
   std::string log = std::string(folder) + "/swfvCellDB_weights.log";
   fvDEBUGl(10, PWRDB, 0, "-dump power and cap in swfvCellDB to: %s\n", log.c_str());
   std::ofstream fout(log);
   assert(fout.good());

   const uint32_t pinWidth = 8;
   const uint32_t lineWidth = 100;
   for (const auto &[cell, terms] : m_powerCache)
   {
      fout << "Cell: " << cell->GetName() << std::endl;
      fout << std::setw(pinWidth) << std::left << "PinName"
           << "Energy(fJ)" << std::endl;
      fout << std::setfill('-') << std::setw(lineWidth) << "-" << std::endl;
      fout << std::setfill(' ') << std::fixed;

      for (const auto &[term, power] : terms)
      {
         fout << std::setw(pinWidth) << term->GetName()
              << std::setprecision(5) << std::scientific
              << std::setw(pinWidth) << power << std::endl;
      }
      fout << std::endl;

      fout << std::setw(pinWidth * 2) << std::left << "InputPin"
         << std::left << "InputCap(ff)" << std::endl;
      assert(m_capCache.find(cell) != m_capCache.end());
      for (const auto &[term, cap] : m_capCache.at(cell))
      {
         fout << std::setw(pinWidth * 2) << term->GetName()
            << std::setprecision(5) << std::scientific << cap << std::endl;
      }
      fout << std::endl;
   }
}

void PowerParserRTL::dumpInstId(const char *folder) const
{
   std::string log = std::string(folder) + "/swfvCellDB_instId.log";
   fvDEBUGl(10, PWRDB, 0, "-dump std/qtref cell instance id to: %s\n", log.c_str());
   std::ofstream fout(log);
   assert(fout.good());

   const uint32_t MAX_ONE_LINE_NUM = 20;
   fout << "---- STD Cell Instance Id ----\n\n";
   std::unordered_map<uint32_t, std::set<uint64_t>> stdCellIdToInstId;
   for (uint64_t stdInstId = 0; stdInstId < m_cellDBInst.m_stdInstIdToStdCellId.size(); stdInstId++)
   {
      stdCellIdToInstId[m_cellDBInst.m_stdInstIdToStdCellId[stdInstId]].insert(stdInstId);
   }

   for (const auto &[stdCellId, stdInstIds] : stdCellIdToInstId)
   {
      assert(stdCellId < m_cellDBInst.m_stdCellIdToName.size());
      fout << "STD cell - " << m_cellDBInst.m_stdCellIdToName[stdCellId] << "(" << stdCellId << "):\n\t";
      uint32_t i = 0;
      for (const uint64_t stdInstId : stdInstIds)
      {
         fout << std::setw(3) << std::left << stdInstId << " ";
         if (++i == MAX_ONE_LINE_NUM)
            fout << "\n\t";
      }
      fout << std::endl;
   }

   fout << "\n\n---- QTRef Cell Instance Id ----\n\n";
   std::unordered_map<uint32_t, std::set<uint64_t>> qtCellIdToInstId;
   for (uint64_t qtInstId = 0; qtInstId < m_cellDBInst.m_qtInstIdToQtCellId.size(); qtInstId++)
   {
      qtCellIdToInstId[m_cellDBInst.m_qtInstIdToQtCellId[qtInstId]].insert(qtInstId);
   }

   for (const auto &[qtCellId, qtInstIds] : qtCellIdToInstId)
   {
      assert(qtCellId < m_cellDBInst.m_qtCellIdToName.size());
      fout << "QTRef cell - " << m_cellDBInst.m_qtCellIdToName[qtCellId] << "(" << qtCellId << "):\n\t";
      uint32_t i = 0;
      for (const uint64_t qtInstId : qtInstIds)
      {
         fout << std::setw(3) << std::left << qtInstId << " ";
         if (++i == MAX_ONE_LINE_NUM)
            fout << "\n\t";
      }
      fout << std::endl;
   }
}

}
