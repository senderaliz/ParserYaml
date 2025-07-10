#include "YamlParser.h"
#include <unordered_map>
#include <vector>
#include "Tools/Assert.h"

void ReadingLine(int& position, const int& length, const std::string& StringConfig, std::string& Line)
{
   int lineStart = position;
   int lineEnd;
   Line.clear();

   while (position < length && StringConfig[position] != '\n') {
      position++;
   }
   lineEnd = position;

   for (int i = lineStart; i < lineEnd; i++) {
      Line += StringConfig[i];
   }

   if (position < length && StringConfig[position] == '\n') {
      position++;
   }
}


bool ShouldSkipLine(const std::string& Line)
{
   for (int i = 0; i < Line.size(); i++) {
      char c = Line[i];

      if (c == ' ' || c == '\t') {
         continue;
      }

      if (c == '#') {
         return true;
      }

      return false;
   }
   return true;
}


int CountIndentation(const std::string& Line)
{
   int count = 0;

   while (count < Line.size() && Line[count] == ' ') {
      count++;
   }

   return count / 2;
}


void DetermineNestingLevel(std::string Line, std::vector<std::string>& VectorKey, int& indent)
{
   indent = CountIndentation(Line);
   if (indent > VectorKey.size()) {
      ASSERT(!"Error");
   }
   if (VectorKey.size() > indent) {
      VectorKey.resize(indent);
   }
}


static bool SkipNewLine(char symbol)
{
   return symbol == '\n';
}


static bool SkipSpace(char symbol)
{
   return symbol == ' ';
}


void SkipWhiteSpaces(int& position, const int& length, const std::string& StringConfig)
{
   while (position < length && (SkipSpace(StringConfig[position]) || SkipNewLine(StringConfig[position]))) {
      position++;
   }
}


void KeyReader(int& position, const std::string& line, std::string& key)
{
   while (position < line.size() && line[position] != ':' && line[position] != '\n') {
      if (!SkipSpace(line[position])) {
         key += line[position];
      }
      position++;
   }
}


void SkipColon(int& position, const std::string& Line)
{
   if (position < Line.size() && Line[position] == ':') {
      position++;
   }
}


void SkipLineWhiteSpaces(int& position, const std::string& Line)
{
   while (position < Line.size() && (SkipSpace(Line[position]) || SkipNewLine(Line[position]))) {
      position++;
   }
}


void ValueReader(int& position, const std::string& Line, std::string& Value)
{
   while (position < Line.size()) {
      if (!(Value.empty() && SkipSpace(Line[position]))) {
         Value += Line[position];
      }
      position++;
   }
}


void AddToConfigMap(std::string& Key, std::unordered_map<std::string, std::string>& HashConfig, std::string& Value)
{
   if (!Key.empty() && !Value.empty()) {
      if (HashConfig.count(Key) != 0) {
         ASSERT(!"Error");
      }
      HashConfig[Key] = Value;
   }
}


void ParseLine(int& currentIndentLevel, std::string Line, std::vector<std::string>& VectorKey, std::unordered_map<std::string, std::string>& HashConfig)
{
   int localPos = currentIndentLevel * 2;
   std::string Key, Value;
   KeyReader(localPos, Line, Key);
   SkipColon(localPos, Line);
   SkipLineWhiteSpaces(localPos, Line);
   ValueReader(localPos, Line, Value);

   if (VectorKey.size() > currentIndentLevel) {
      VectorKey.resize(currentIndentLevel);
   } else if (VectorKey.size() < currentIndentLevel) {
      ASSERT(!"Error");
   }

   VectorKey.push_back(Key);

   std::string FullKey;
   for (int i = 0; i < VectorKey.size(); i++) {
      if (i > 0)
         FullKey += ".";
      FullKey += VectorKey[i];
   }

   if (Value.empty()) {
      if (HashConfig.count(FullKey) != 0) {
         ASSERT(!"Duplicate section key");
      }
      return;
   }

   AddToConfigMap(FullKey, HashConfig, Value);

   VectorKey.pop_back();
}


std::unordered_map<std::string, std::string> YamlParser::Parse(const std::string& StringConfig)
{
   configMap.clear();
   std::vector<std::string> VectorKey;

   int position = 0;
   const int length = StringConfig.length();
   const std::string This_stringConfig = StringConfig;
   int sumIndents;
   std::string Line;

   while (position < length) {
      ReadingLine(position, length, This_stringConfig, Line);

      if (ShouldSkipLine(Line)) {
         continue;
      }

      DetermineNestingLevel(Line, VectorKey, sumIndents);

      ParseLine(sumIndents, Line, VectorKey, configMap);
   }

   return configMap;
}


std::unordered_map<std::string, std::string>& YamlParser::GetConfigMap()
{
   return configMap;
}


std::string YamlParser::GetValue(const std::string& key)
{
   auto it = configMap.find(key);
   if (it != configMap.end()) {
      return it->second;
   } else {
      return "";
   }
}


std::string YamlParser::SettingValue(const std::string& key, std::string value)
{
   return configMap[key] = value;
}


bool YamlParser::HasKey(std::string& key)
{
   if (configMap.count(key) != 0) {
      return true;
   }
   return false;
}
