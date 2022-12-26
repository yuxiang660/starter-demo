#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>

#include <unordered_map>
#include <vector>

#include "envVar.h"

namespace
{

const char *DB_DEFAULT_FOLDER = "./dbFiles";

}

namespace NPE
{

inline bool dumpJsonCellDB()
{
   static env::EnvVar<bool> e("CDN_NPE20_DUMP_JSON_CELLDB", false, false,
      "Internal env to dump CellDB in json format");
   return e.value();
}

class swfvCellDB
{
public:
   swfvCellDB() = default;
   ~swfvCellDB() = default;
   swfvCellDB(const swfvCellDB &) = delete;
   swfvCellDB& operator=(const swfvCellDB&) = delete;

   void save(const char *folder = DB_DEFAULT_FOLDER)
   {
      std::string dbPath = std::string(folder) + "/" + std::string(DB_FILENAME);
      std::ofstream fout(dbPath);
      assert(fout.good());
      cereal::BinaryOutputArchive oarchive(fout);
      oarchive(*this);
   }

   void saveToJSON(const char *folder = DB_DEFAULT_FOLDER)
   {
      std::string dbPath = std::string(folder) + "/" + std::string(DB_FILENAME) + ".json";
      std::ofstream fout(dbPath);
      assert(fout.good());
      cereal::JSONOutputArchive oarchive(fout);
      oarchive(*this);
   }

   void load(const char *folder = DB_DEFAULT_FOLDER)
   {
      std::string dbPath = std::string(folder) + "/" + std::string(DB_FILENAME);
      std::ifstream fin(dbPath);
      assert(fin.good());
      cereal::BinaryInputArchive iarchive(fin);
      iarchive(*this);
   }

   template <class Archive>
   void serialize(Archive &archive)
   {
      archive(m_maxQtCellPinNum, m_stdInstPowerW, m_stdInstPinCapW, m_qtInstPinIds);
   }

public:
   static const uint64_t WEIGHT_SCALE = 1000;

public:
   void setMaxQtCellPinNum(uint32_t maxPinNum)
   {
      m_maxQtCellPinNum = maxPinNum;
   }

   void addInternalPowerWeight(uint64_t gateId, uint64_t stdInstId, double energyfJ)
   {
      m_stdInstPowerW[gateId][stdInstId] += (uint64_t)(energyfJ * WEIGHT_SCALE);
   }

   void addPinCapWeight(uint64_t gateId, uint64_t stdInstId, double pinCapff)
   {
      m_stdInstPinCapW[gateId][stdInstId] += (uint64_t)(pinCapff * WEIGHT_SCALE);
   }

   void addQtInstPinId(uint64_t gateId, uint64_t stdInstId, uint64_t qtInstId, uint32_t pinId)
   {
      m_qtInstPinIds[gateId][stdInstId][qtInstId].push_back(pinId);
   }

public:
   uint32_t getMaxQtCellPinNum() const { return m_maxQtCellPinNum; }

   // key1 is swfvDB gateId, key2 is stdInstId, value is stdInst weight
   typedef std::unordered_map<uint64_t, std::unordered_map<uint64_t, uint64_t>> StdInstWeightMap;
   const StdInstWeightMap &getInternalPowerWeightMap() const { return m_stdInstPowerW; }
   const StdInstWeightMap &getPinCapWeightMap() const { return m_stdInstPinCapW; }

   typedef std::unordered_map<uint64_t, std::vector<uint32_t>> QtInstPins;
   // key1 is swfvDB gateId, key2 is stdInstId, key3 is qtInstId, value is qtCellPinIds
   typedef std::unordered_map<uint64_t, std::unordered_map<uint64_t, QtInstPins>> QtInstPinMap;
   const QtInstPinMap &getQtInstPinMap() const { return m_qtInstPinIds; }

private:
   static constexpr const char *DB_FILENAME = "swfvCellDB";

private:
   uint32_t m_maxQtCellPinNum = 0;
   // find out all the stdInsts that contributed by the net
   StdInstWeightMap m_stdInstPowerW;
   StdInstWeightMap m_stdInstPinCapW;
   // find out all the qtInsts that contributed by the net
   QtInstPinMap m_qtInstPinIds;
};

class swfvCellDBInst
{
   friend class PowerParserRTL; // used to initialized the class members
public:
   swfvCellDBInst() = default;
   ~swfvCellDBInst() = default;
   swfvCellDBInst(const swfvCellDBInst &) = delete;
   swfvCellDBInst& operator=(const swfvCellDBInst&) = delete;

   void saveToJSON(const char *folder = DB_DEFAULT_FOLDER)
   {
      std::string dbPath = std::string(folder) + "/" + std::string(DB_FILENAME) + ".json";
      std::ofstream fout(dbPath);
      assert(fout.good());
      cereal::JSONOutputArchive oarchive(fout);
      oarchive(*this);
   }

   template <class Archive>
   void serialize(Archive &archive)
   {
      archive(m_stdCellIdToName, m_qtCellIdToName, m_qtCellPinIdToName, m_stdInstIdToStdCellId, m_qtInstIdToQtCellId);
   }

private:
   static constexpr const char *DB_FILENAME = "swfvCellDB.inst";

private:
   /**
    * id to name members, which are used in post-process
   */
   std::vector<std::string> m_stdCellIdToName;
   std::vector<std::string> m_qtCellIdToName;
   // key: qtCellId, value: qtCell pin names (the vector index is the pinId)
   std::unordered_map<uint32_t, std::vector<std::string>> m_qtCellPinIdToName;

   // value is stdCellId
   std::vector<uint32_t> m_stdInstIdToStdCellId;
   // value is qtCellId
   std::vector<uint32_t> m_qtInstIdToQtCellId;
};

}
