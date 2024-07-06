#pragma once

#include "headers.h"

void write_file(std::string path,std::string buffer)
{
std::ofstream file=std::ofstream(path);
file<<buffer;
file.close();
}

std::string read_file(std::string path)
{
std::ostringstream output;
std::ifstream file=std::ifstream(path);
output << file.rdbuf();
return output.str();
}
