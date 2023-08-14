#include <iostream>
#include <cassert>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

std::vector<std::string> parse_MBIO_data()
{
   std::ifstream cFile("MBIO.data");
   if (!cFile.is_open())
      return {};

   std::vector<std::string> data;

   std::string line;
   while (getline(cFile, line))
   {
      line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
      if (line[0] == '#' || line.empty())
         continue;

      auto delimiterPos = line.find("=");
      auto name = line.substr(0, delimiterPos);
      auto value = line.substr(delimiterPos + 1);
      std::cout << name << " " << value << std::endl;

      data.emplace_back(value);
   }

   return data;
}

std::vector<std::string> parse_BPIO_data()
{
   std::ifstream cFile("BPIO.data");
   if (!cFile.is_open())
      return {};

   std::vector<std::string> data;

   std::string line;
   while (getline(cFile, line))
   {
      line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
      if (line[0] == '#' || line.empty())
         continue;

      auto delimiterPos = line.find("=");
      auto name = line.substr(0, delimiterPos);
      auto value = line.substr(delimiterPos + 1);
      std::cout << name << " " << value << std::endl;

      data.emplace_back(value);
   }

   return data;
}

std::vector<int> stov(const std::string &str)
{
   std::stringstream ss(str);
   std::vector<int> data;
   for (int i; ss >> i;)
   {
      data.push_back(i);
      if (ss.peek() == ',')
         ss.ignore();
   }
   return data;
}

int main()
{
   auto mbio_data = parse_MBIO_data();
   assert(mbio_data.size() == 3);
   int sd_index = std::stoi(mbio_data[0]);
   int sd_rx_lane = std::stoi(mbio_data[1]);
   int expected_delay = std::stoi(mbio_data[2]);
   std::cout << "mbio sd_index: " << sd_index << std::endl;
   std::cout << "mbio sd_rx_lane: " << sd_rx_lane << std::endl;
   std::cout << "mbio expected_delay: " << expected_delay << std::endl;

   auto bpio_data = parse_BPIO_data();
   assert(bpio_data.size() == 4);
   int bpio_sd_index = std::stoi(bpio_data[0]);
   int bpio_sd_rx_lane = std::stoi(bpio_data[1]);
   auto bpio_all_sds = stov(bpio_data[2]);
   int bpio_expected_delay = std::stoi(bpio_data[3]);
   std::cout << "bpio sd_index: " << bpio_sd_index << std::endl;
   std::cout << "bpio sd_rx_lane: " << bpio_sd_rx_lane << std::endl;
   std::cout << "bpio expected_delay: " << bpio_expected_delay << std::endl;
   std::cout << "bpio bpio_all_sds: " << std::endl;
   for (auto &&sd : bpio_all_sds)
   {
      std::cout << sd << ", ";
   }
   std::cout << std::endl;
}
