#include "Parser.h"
#include <unordered_map>
#include <vector>
#include <iostream>//Debugging

void readingLine(int& position, const int& length, const std::string& stringConfig, std::string& line)
{
	int lineStart = position;
	int lineEnd;
	line.clear();

	while (position < length && stringConfig[position] != '\n')
	{
		position++;
	}
	lineEnd = position;

	for (int i = lineStart; i < lineEnd; i++)
	{
		line += stringConfig[i];
	}

	if (position < length && stringConfig[position] == '\n')
	{
		position++;
	}
}


bool skipEmptyLines(const std::string& line)
{
	for (int i = 0; i <line.size(); i++)
	{
		char c = line[i];

		if (c == ' ' || c == '\t')
		{
			continue;
		}

		if (c == '#')
		{
			return true;
		}

		return false;
	}
	return true;
}


int countIndent(const std::string& line)
{
	int count = 0;

	while (count < line.size() && line[count] == ' ')
	{
		count++;
	}

	return count / 2;
}


void DeterminingNestingLevel(std::string line, std::vector<std::string>& vectorKey, int& indent)
{
	indent = countIndent(line);

	if (vectorKey.size() > indent)
	{
		vectorKey.resize(indent);
	}

}

static bool skipNewLine(char symbol)
{
	return symbol == '\n';
}

static bool skipSpace(char symbol)
{
	return symbol == ' ';
}

void skipCharacters(int& position, const int& length, const std::string& stringConfig)
{
	while (position < length && (skipSpace(stringConfig[position]) || skipNewLine(stringConfig[position])))
	{
		position++;
	}
}

void keyReader(int& position, const std::string& line, std::string& key)
{
	while (position < line.size() && line[position] != ':' && line[position] != '\n')
	{
		if (!skipSpace(line[position]))
		{
			key += line[position];
		}
		position++;
	}
}

void skipColon(int& position, const std::string& line)
{
	if (position < line.size() && line[position] == ':')
	{
		position++;
	}
}

void skipCharactersLine(int& position, const std::string& line)
{
	while (position < line.size() && (skipSpace(line[position]) || skipNewLine(line[position])))
	{
		position++;
	}
}

void valueReader(int& position, const std::string& line, std::string& value)
{
	while (position < line.size())
	{
		if (!(value.empty() && skipSpace(line[position])))
		{
			value += line[position];
		}
		position++;
	}
}

void addHash(std::string& key, std::unordered_map<std::string, std::string>& hashConfig, std::string& value)
{
	if (!key.empty() && !value.empty())
	{
		hashConfig[key] = value;
	}
}

void ParsingProcess(int& sumIndents, std::string line, std::vector<std::string>& vectorKey, std::unordered_map<std::string, std::string>& hashConfig)
{
	int localPos = sumIndents * 2;
	std::string key, value;
	keyReader(localPos, line, key);
	skipColon(localPos, line);
	skipCharactersLine(localPos, line);
	valueReader(localPos, line, value);

	vectorKey.push_back(key);

	if (!value.empty())
	{
		std::string fullKey;

		for (int i = 0; i < vectorKey.size(); i++)
		{
			if (i > 0)
			{
				fullKey += ".";
			}
			fullKey += vectorKey[i];
		}

		addHash(fullKey, hashConfig, value);
		std::cout << "key [" << fullKey << "] value [" << value << "]" << std::endl; //Debugging
		vectorKey.pop_back();
	}
}

std::unordered_map<std::string, std::string> YamlParser::parse(const std::string& stringConfig)
{
	std::unordered_map<std::string, std::string> hashConfig;
	std::vector<std::string> vectorKey;

	int position = 0;
	const int length = stringConfig.length();
	const std::string this_stringConfig = stringConfig;
	int sumIndents;
	std::string line;

	while (position < length)
	{
		readingLine(position, length, this_stringConfig, line);

		if (skipEmptyLines(line))
		{
			continue;
		}

		DeterminingNestingLevel(line, vectorKey, sumIndents);

		ParsingProcess(sumIndents, line, vectorKey, hashConfig);

	}

	return hashConfig;
}
