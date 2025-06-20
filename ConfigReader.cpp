#include "Parser.h"
#include <string>
#include <fstream>
#include <cassert>

void checkIsOpen(const std::ifstream& file)
{
	assert(file.is_open());
}

std::string readFile( std::ifstream& file)
{
	std::string buffer;
	std::string line;
	
	bool firstLine = true;

	while (std::getline(file, line))
	{
		if (firstLine)
		{
			firstLine = false;
		}
		else
		{
			buffer += "\n";
		}
		buffer += line;
	}
	return buffer;
}

std::string configReader::readFileToString(const std::string& filePath)
{
	std::ifstream file(filePath);

	checkIsOpen(file);
	
	std::string content = readFile(file);
	
	file.close();

	return content;
}
