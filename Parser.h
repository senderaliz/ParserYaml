#pragma once
#include <string>
#include <unordered_map>


class configReader
{
public:
	static std::string readFileToString(const std::string& filePath);
};

class YamlParser
{
public:
	std::unordered_map<std::string, std::string> parse(const std::string& stringConfig);
};