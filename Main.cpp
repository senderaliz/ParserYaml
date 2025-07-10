#include "YamlParser.h"
#include "Tools/Config.h"
#include <iostream>

int main()
{
const std::string yaml_config =
   "aa:\n"
   "  ab:\n"
   "    ac: 4\n"
   "ba: 1\n"
   "bb:\n"
   "bc: true\n"
   "bd: \"string\"\n"
   "be: 1.234";

Tools::YamlParser parse(yaml_config);
Tools::Config config(parse.GetConfigMap());

int testCheck1 = config.GetAsInt("aa.ab.ac");

if (testCheck1 != 4) {
   ASSERT(!"Error GetAsString ");
}

bool testCheck2 = config.GetAsBool("bc");

if (testCheck2 != true) {
   ASSERT(!"Error GetAsBool");
}

int testCheck3 = config.GetAsInt("ba");

if (testCheck3 != 1) {
   ASSERT(!"Error GetAsInt");
}

std::string testCheck4 = config.GetAsString("bd");

if (testCheck4 != "string") {
   ASSERT(!"Error GetAsString");
}

double testCheck6 = config.GetAsDouble("be");
double tempNumber = 1.234;

if (testCheck6 != tempNumber) {
   ASSERT(!"Error: GetAsDouble");
}
}