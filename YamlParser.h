#pragma once
#include <string>
#include <unordered_map>


class YamlParser {
public:

   std::unordered_map<std::string, std::string> Parse(const std::string& stringConfig);

   std::unordered_map<std::string, std::string>& GetConfigMap();

   std::string GetValue(const std::string& key);

   std::string YamlParser::SettingValue(const std::string& key, std::string value);

   bool HasKey(std::string& key);

private:
   std::unordered_map<std::string, std::string> configMap;
};