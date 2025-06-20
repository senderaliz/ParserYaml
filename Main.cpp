#include "Parser.h"
#include <iostream>


int main()
{
	std::string ContentConfig = configReader::readFileToString("Config.yml");
	YamlParser check;
	check.parse(ContentConfig);
}