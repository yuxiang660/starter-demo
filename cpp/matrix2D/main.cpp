#include <iostream>
#include <memory>
#include <cstddef>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include <numeric>

// template <typename uint64_t>
// class matrix2D
// {
// public:
//    matrix2D(size_t row, size_t nChannels, size_t nSamples) : m_data(row * nChannels * nSamples), m_row(row), m_col(col) {}

//    span<uint64_t> operator[](int row) { return span<uint64_t>(&m_data[row * m_col], m_col); }
//    const span<uint64_t> operator[] (int row) const { return span<uint64_t>(&m_data[row * m_col], m_col); }

//    size_t size() const
//    {
//       return m_row;
//    }

// private:
//    std::vector<uint64_t> m_data;
//    const size_t m_row;
//    const size_t m_col;
// };

class PPF_CYC_DATA
{
   class View
   {
      uint64_t *m_addr;
      const PPF_CYC_DATA * m_data;
      const size_t m_len;

   public:
      View(uint64_t *addr, const PPF_CYC_DATA *data)
         : m_addr(addr), m_nChannels(nChannels), m_nSamples(nSamples), m_len(nChannels * nSamples)
      {
      }

      uint64_t &operator[](int i)
      {
         return m_addr[i];
      }

      uint64_t const &operator[](int i) const
      {
         return m_addr[i];
      }

      size_t size() const
      {
         return m_nChannels * m_nSamples;
      }

      uint64_t *begin()
      {
         return m_addr;
      }

      uint64_t *end()
      {
         return m_addr + m_len;
      }

      const uint64_t *cbegin() const
      {
         return m_addr;
      }

      const uint64_t *cend() const
      {
         return m_addr + m_len;
      }
   };

public:
   PPF_CYC_DATA(size_t nTypes, size_t nChannels, size_t nSamples) : m_data(nTypes * nChannels * nSamples),
      m_nTypes(nTypes), m_nChannels(nChannels), m_nSamples(nSamples), m_nDataInOneType(nChannels * nSamples) {}

   span operator[](int type) { return span(&m_data[type * m_nDataInOneType], m_nChannels, m_nSamples); }
   const span operator[] (int type) const { return span(&m_data[type * m_nDataInOneType], m_nChannels, m_nSamples); }

   size_t size() const
   {
      return m_nTypes;
   }

private:
   std::vector<uint64_t> m_data;
   const size_t m_nTypes;
   const size_t m_nChannels;
   const size_t m_nSamples;
   const size_t m_nDataInOneType;
};

int main()
{
   size_t NUM_TYPES = 3;
   int nChannels = 2;
   int nSamples = 3;
   matrix2D<int> buf(NUM_TYPES, nChannels, nSamples);
   matrix2D<int> ch0Buf(NUM_TYPES, nSamples);
   ch0Buf[0][0] = 1;
   ch0Buf[0][1] = 2;
   ch0Buf[0][2] = 3;
   ch0Buf[1][0] = 4;
   ch0Buf[1][1] = 5;
   ch0Buf[1][2] = 6;
   int type = 0;
   int ch = 0;
   buf[type].copy(ch, ch0Buf[type].cbegin(), ch0Buf[type].cend());
   ch = 1;
   buf[type].copy(ch, ch0Buf[type].cbegin(), ch0Buf[type].cend());

   std::cout << "NUM_TYPES: " << buf.size() << std::endl;
   std::cout << "num elements: " << buf[0].size() << std::endl;

   for (int &i : buf[0])
   {
      std::cout << i << "\n";
      i++;
   }

   for (const int &i : buf[0])
   {
      std::cout << i << "\n";
   }

   std::cout << "sum:" << std::accumulate(buf[0].cbegin(), buf[0].cend(), 0) << std::endl;
   std::cout << "min:" << *std::min_element(buf[0].cbegin(), buf[0].cend()) << std::endl;
   std::cout << "max:" << *std::max_element(buf[0].cbegin(), buf[0].cend()) << std::endl;
   return 0;
}
