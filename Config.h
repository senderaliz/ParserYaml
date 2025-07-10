#pragma once

#include <string>
#include <unordered_map>

namespace Tools {
class Config {
public:
   Config(const std::unordered_map<std::string, std::string> configMap);

   bool HasValue(const std::string& key) const;
   std::string GetAsString(const std::string& key);
   int GetAsInt(const std::string& key);
   bool GetAsBool(const std::string& kye);
   double GetAsDouble(const std::string& key);


private:
   std::unordered_map<std::string, std::string> _configMap;
};
}