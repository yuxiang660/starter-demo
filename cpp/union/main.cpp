#include <iostream>
#include <memory>
#include <cstddef>
#include <stdio.h>
#include <string.h>

struct ChData_U
{
   // channel data for TC
   struct TC
   {
      uint64_t sumT = 0, minT = 0, maxT = 0, sumH = 0, minH = 0, maxH = 0;
      int offset = 0, size = 0;
   };
   // channel data for HW-NPE
   struct HWNPE
   {
      uint64_t sumT = 0, sumTSeq = 0, sumTCom = 0, sumTBb = 0;
      uint64_t minT = 0, minTSeq = 0, minTCom = 0, minTBb = 0;
      uint64_t maxT = 0, maxTSeq = 0, maxTCom = 0, maxTBb = 0;
      int offset = 0, tSeqOff = 0, tComOff = 0, tBbOff = 0;
   };
   // channel data for SW-NPE
   struct SWNPE
   {
      uint64_t sumT = 0, minT = 0, maxT = 0, sumTSeq = 0, sumTCom = 0;
      uint64_t sumSWT = 0, sumSWSeq = 0, sumSWCom = 0;
      uint64_t sumTClk = 0, sumStClk = 0;
      int offset = 0, tSeqOff = 0, tComOff = 0;
      int spoffset = 0, spSeqOff = 0, spComOff = 0;
      int tClkOff = 0, stClkOff = 0;
   };

   // use union to save memory since we only one type at a time
   union U
   {
      TC tc;
      HWNPE hn;
      SWNPE sn;
      U() { memset((void *)this, 0, sizeof(U)); }
   } d;

   TC &tc() { return d.tc; }
   HWNPE &hn() { return d.hn; }
   SWNPE &sn() { return d.sn; }
};

struct ChData
{
   // channel data for TC
   struct TC
   {
      uint64_t sumT = 0, minT = 0, maxT = 0, sumH = 0, minH = 0, maxH = 0;
      int offset = 0, size = 0;
   } tc;
   // channel data for HW-NPE
   struct HWNPE
   {
      uint64_t sumT = 0, sumTSeq = 0, sumTCom = 0, sumTBb = 0;
      uint64_t minT = 0, minTSeq = 0, minTCom = 0, minTBb = 0;
      uint64_t maxT = 0, maxTSeq = 0, maxTCom = 0, maxTBb = 0;
      int offset = 0, tSeqOff = 0, tComOff = 0, tBbOff = 0;
   } hn;
   // channel data for SW-NPE
   struct SWNPE
   {
      uint64_t sumT = 0, minT = 0, maxT = 0, sumTSeq = 0, sumTCom = 0;
      uint64_t sumSWT = 0, sumSWSeq = 0, sumSWCom = 0;
      uint64_t sumTClk = 0, sumStClk = 0;
      int offset = 0, tSeqOff = 0, tComOff = 0;
      int spoffset = 0, spSeqOff = 0, spComOff = 0;
      int tClkOff = 0, stClkOff = 0;
   } sn;
};

int main()
{
   std::cout << "sizeof(ChData_U)" << sizeof(ChData_U) << std::endl;
   std::cout << "sizeof(ChData)" << sizeof(ChData) << std::endl;
   std::cout << "sizeof(SWNPE)" << sizeof(ChData::SWNPE) << std::endl;
   ChData_U u;
   std::cout << u.tc().sumT << std::endl;
   u.hn().sumT = 1;
   std::cout << u.hn().sumT << std::endl;
   std::cout << u.sn().sumT << std::endl;
   return 0;
}
