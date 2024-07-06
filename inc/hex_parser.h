#pragma once

#include "headers.h"
#include "structs.h"
#include "update.h"
#include "imgui_functions.h"
#include "file.h"
#include "draw.h"

namespace str_parsing
{
std::string strip_string(std::string line,std::string stripped) {
std::unordered_set<char> stripped_set(stripped.begin(), stripped.end());
std::string result;
for(char ch:line) 
{
if(stripped_set.find(ch)==stripped_set.end()) 
{result+=ch;}
}
return result;
}

std::string char_strip_string(std::string line,std::string stripped) {
std::unordered_set<char> chars_to_remove(stripped.begin(), stripped.end());    
std::string result;
for(char c:line) 
{
if(chars_to_remove.find(c)==chars_to_remove.end())
{result+=c;}
}    
return result;
}


std::vector<std::string>split_string(std::string line,std::string separator)
{
std::vector<std::string>result;
size_t start=0;
size_t end=0;
while((end=line.find(separator,start))!=std::string::npos) 
{
result.push_back(line.substr(start,end-start));
start=end+separator.length();
}
result.push_back(line.substr(start));
return result;
}

int index_string(std::vector<std::string>index,std::string string)
{
int pos=0;
for(std::string str:index)
{
if(str==string)
{return pos;}
else
{pos++;}
}

return -1;
}

int str_number_to_int(std::string str)
{
if(str.substr(0,2)=="0b"||str.substr(0,2)=="0B") 
{return std::stoi(str.substr(2),nullptr,2);}
else if(str.substr(0,2)=="0x"||str.substr(0,2)=="0X") 
{return std::stoi(str.substr(2),nullptr,16);} 
else 
{return std::stoi(str,nullptr,10);}
return 0;
}


}


void parse_hex(Uint8*buffer,std::string hex)
{

if(buffer==nullptr){return;}

std::vector<std::string>splitted=str_parsing::split_string(hex,"\n");

int last_address;
std::vector<Uint8>last_bytes;

for (std::string line:splitted)
{

if(!line.empty())
{
if(line[0]=='*')
{
for (int i=0;i<last_bytes.size();i++)
{buffer[last_address+1+i]=last_bytes[i];}
}
else
{
int address=str_parsing::str_number_to_int("0x"+line.substr(0,4));
std::vector<Uint8>bytes;
std::vector<std::string>split=str_parsing::split_string(str_parsing::split_string(str_parsing::char_strip_string(line.substr(6),"\n\r\t"),";")[0]," ");
for (std::string hex:split)
{if(!hex.empty()){bytes.push_back(str_parsing::str_number_to_int("0x"+hex));}}


for (int i=0;i<bytes.size();i++)
{
buffer[address+i]=bytes[i];
}

last_bytes=bytes;
last_address=address;
}
}


}


}