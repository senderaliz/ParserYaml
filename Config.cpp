#include "Config.h"
#include "Tools/Assert.h"

Tools::Config::Config(const std::unordered_map<std::string, std::string> configMap)
{
   _configMap = configMap;
}


bool Tools::Config::HasValue(const std::string& key) const
{
   return _configMap.count(key) != 0;
}


std::string Tools::Config::GetAsString(const std::string& key)
{
   auto it = _configMap.find(key);
   if (it != _configMap.end()) {
      auto& str = it->second;
      ASSERT(str.front() == '\"' && str.back() == '\"');
      return str.substr(1, str.size() - 2);
   }
   ASSERT_FAIL();
   return {};
}


int Tools::Config::GetAsInt(const std::string& key)
{
   auto result = _configMap.find(key);
   if (result != _configMap.end()) {
      std::string& value = result->second;
      int finalResult = 0;

      for (int i = 0; i < value.size(); i++) {
         char symbol = value[i];
         ASSERT(!(symbol < '0' || symbol > '9'));
         finalResult = finalResult * 10 + (value[i] - '0');
      }
      return finalResult;
   }
   ASSERT_FAIL();
   return 0;
}


bool Tools::Config::GetAsBool(const std::string& key)
{
   auto result = _configMap.find(key);
   if (result != _configMap.end()) {
      std::string& value = result->second;
      if (value == "true" || value == "True") {
         return true;
      }
      if (value == "false" || value == "False") {
         return false;
      }
   }
   ASSERT_FAIL();
   return false;
}


double Tools::Config::GetAsDouble(const std::string& key)
{
   auto result = _configMap.find(key);
   if (result != _configMap.end()) {
      std::string& value = result->second;
      double wholePart = 0;
      double fractionalPart = 0;
      bool presencePoint = false;
      double divider = 10;

      for (int i = 0; i < value.size(); i++) {
         if (value[i] == '.') {
            ASSERT(!presencePoint);
            presencePoint = true;
         } else if (value[i] >= '0' && value[i] <= '9') {
            if (!presencePoint) {
               wholePart = wholePart * 10 + (value[i] - '0');
            } else {
               fractionalPart += (value[i] - '0') / divider;
               divider *= 10.0;
            }
         } else {
            ASSERT(false);
         }
      }
      return wholePart + fractionalPart;
   }
   ASSERT_FAIL();
   return 0.0;
}
