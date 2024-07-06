#pragma once

#include "headers.h"
#include "structs.h"
#include "update.h"
#include "imgui_functions.h"
#include "file.h"
#include "draw.h"
#include "hex_parser.h"

TTF_Font*font;
int char_count;
int line_count;
char font_name[64];
int font_size;

bool sim_running=0;
bool sim_step=0;
Uint64 sim_tick=0;
float sim_clock_speed=1;
std::string current_instruction;
std::string term_output;

float sim_delta_time=0;
Uint32 sim_lastTime=0;
float tick_counter=0;
Uint16 counter=0;

char memory_file_path[64];

bool interrupt=0;

std::array<Uint8,0xffff>memory;

std::array<Uint8,16>registers;

Uint8 rol(Uint8 n,Uint8 d) 
{
d=d%(sizeof(n)*CHAR_BIT);
return(n<<d)|(n>>(sizeof(n)*CHAR_BIT-d));
}

Uint8 ror(Uint8 n,Uint8 d) 
{
d=d%(sizeof(n)*CHAR_BIT);
return(n<<d)|(n>>(sizeof(n)*CHAR_BIT-d));
}


void run_instruction(Uint8 ins,Uint8 byte0,Uint8 byte1)
{
switch (ins & 0xf0)
{
case 0x00:break;

case 0x10:
{

}
break;

case 0x20:
{
switch (ins&0x0f)
{
case 0x0:{registers[(byte0&0xf0)>>4]=  registers[(byte0&0x0f)]+registers[(byte1&0x0f)];}break;
case 0x1:{registers[(byte0&0xf0)>>4]=  registers[(byte0&0x0f)]+registers[(byte1&0x0f)]+1;}break;
case 0x2:{registers[(byte0&0xf0)>>4]=  registers[(byte0&0x0f)]-registers[(byte1&0x0f)];}break;
case 0x3:{registers[(byte0&0xf0)>>4]=  registers[(byte0&0x0f)]-registers[(byte1&0x0f)]-1;}break;
case 0x4:{registers[(byte0&0xf0)>>4]=  registers[(byte0&0x0f)]*registers[(byte1&0x0f)];}break;
case 0x5:{registers[(byte0&0xf0)>>4]=  registers[(byte0&0x0f)]&registers[(byte1&0x0f)];}break;
case 0x6:{registers[(byte0&0xf0)>>4]=  registers[(byte0&0x0f)]|registers[(byte1&0x0f)];}break;
case 0x7:{registers[(byte0&0xf0)>>4]=  registers[(byte0&0x0f)]^registers[(byte1&0x0f)];}break;
case 0x8:{registers[(byte0&0xf0)>>4]=!(registers[(byte0&0x0f)]&registers[(byte1&0x0f)]);}break;
case 0x9:{registers[(byte0&0xf0)>>4]=!(registers[(byte0&0x0f)]|registers[(byte1&0x0f)]);}break;
case 0xA:{registers[(byte0&0xf0)>>4]=!(registers[(byte0&0x0f)]^registers[(byte1&0x0f)]);}break;
case 0xB:{registers[(byte0&0xf0)>>4]= (registers[(byte0&0x0f)]>>registers[(byte1&0x0f)]);}break;
case 0xC:{registers[(byte0&0xf0)>>4]= (registers[(byte0&0x0f)]<<registers[(byte1&0x0f)]);}break;
case 0xD:{registers[(byte0&0xf0)>>4]= rol(registers[(byte0&0x0f)],registers[(byte1&0x0f)]);}break;
case 0xE:{registers[(byte0&0xf0)>>4]= ror(registers[(byte0&0x0f)],registers[(byte1&0x0f)]);}break;
case 0xF:{registers[(byte0&0xf0)>>4]= -registers[(byte0&0x0f)];}break;


default:break;
}
}
break;

case 0x30:
{
switch (ins&0x0f)
{
case 0x0:{registers[(byte0&0xf0)>>4]=  registers[(byte0&0x0f)]+byte1;}break;
case 0x1:{registers[(byte0&0xf0)>>4]=  registers[(byte0&0x0f)]+byte1+1;}break;
case 0x2:{registers[(byte0&0xf0)>>4]=  registers[(byte0&0x0f)]-byte1;}break;
case 0x3:{registers[(byte0&0xf0)>>4]=  registers[(byte0&0x0f)]-byte1-1;}break;
case 0x4:{registers[(byte0&0xf0)>>4]=  registers[(byte0&0x0f)]*byte1;}break;
case 0x5:{registers[(byte0&0xf0)>>4]=  registers[(byte0&0x0f)]&byte1;}break;
case 0x6:{registers[(byte0&0xf0)>>4]=  registers[(byte0&0x0f)]|byte1;}break;
case 0x7:{registers[(byte0&0xf0)>>4]=  registers[(byte0&0x0f)]^byte1;}break;
case 0x8:{registers[(byte0&0xf0)>>4]=!(registers[(byte0&0x0f)]&byte1);}break;
case 0x9:{registers[(byte0&0xf0)>>4]=!(registers[(byte0&0x0f)]|byte1);}break;
case 0xA:{registers[(byte0&0xf0)>>4]=!(registers[(byte0&0x0f)]^byte1);}break;
case 0xB:{registers[(byte0&0xf0)>>4]= (registers[(byte0&0x0f)]>>byte1);}break;
case 0xC:{registers[(byte0&0xf0)>>4]= (registers[(byte0&0x0f)]<<byte1);}break;
case 0xD:{registers[(byte0&0xf0)>>4]= rol(registers[(byte0&0x0f)],byte1);}break;
case 0xE:{registers[(byte0&0xf0)>>4]= ror(registers[(byte0&0x0f)],byte1);}break;
case 0xF:{registers[(byte0&0xf0)>>4]= -registers[(byte0&0x0f)];}break;

default:break;
}
}
break;

case 0x40:{registers[byte0>>4]=byte1;}break;

case 0x50:
{

}
break;

case 0x60:
{

}
break;

case 0x70:
{

}
break;

case 0x80:
{

}
break;

case 0x90:
{counter=((byte0<<4) | byte1);}
break;

case 0xA0:
{
Uint8 status_reg=registers[0xB];
if(ins&0x08){status_reg=~status_reg;}
switch (ins&0x07)
{
case 0x00:if(status_reg&0x01){counter=((byte0<<4) | byte1);}break;
case 0x01:if(status_reg&0x02){counter=((byte0<<4) | byte1);}break;
case 0x02:if(status_reg&0x04){counter=((byte0<<4) | byte1);}break;
case 0x03:if(status_reg&0x08){counter=((byte0<<4) | byte1);}break;
case 0x04:if(status_reg&0x10){counter=((byte0<<4) | byte1);}break;
case 0x05:if(status_reg&0x20){counter=((byte0<<4) | byte1);}break;
case 0x06:if(status_reg&0x40){counter=((byte0<<4) | byte1);}break;
case 0x07:if(status_reg&0x80){counter=((byte0<<4) | byte1);}break;

default:break;
}
}
break;

case 0xB0:
{

}
break;


case 0xC0:
{
memory[(registers[0xF]<<8) | (registers[0xE])]=counter+3;
registers[0xE]++;
counter=((byte0<<4)|byte1);
}

break;

case 0xD0:
{
registers[0xE]--;
counter=memory[(registers[0xF]<<8)|(registers[0xE])];
}
break;

case 0xE0:{while(!(interrupt==true)){std::this_thread::get_id();}}break;

case 0xF0:
{

}
break;

}


}

void sim_worker()
{

{
sim_lastTime=SDL_GetTicks();
sim_delta_time=0;
tick_counter=0;
}

while (running)
{
Uint32 sim_currentTime = SDL_GetTicks();
sim_delta_time = (sim_currentTime - sim_lastTime) / 1000.0f;
sim_lastTime = sim_currentTime;

if((sim_running&&tick_counter>=(1/sim_clock_speed))||sim_step)
{
//registers[0]=0;

Uint8 instruction=memory[(counter)%memory.size()];
Uint8 byte0  =memory[(counter+1)%memory.size()];
Uint8 byte1  =memory[(counter+2)%memory.size()];



switch (instruction & 0xf0)
{
case 0x00:current_instruction="nop"; break;
case 0x10:current_instruction="brk"; break;
case 0x20:current_instruction="alu"; break;
case 0x30:current_instruction="alui";break;
case 0x40:current_instruction="ldi"; break;
case 0x50:current_instruction="lod"; break;
case 0x60:current_instruction="str"; break;
case 0x70:current_instruction="push";break;
case 0x80:current_instruction="pop"; break;
case 0x90:current_instruction="jmp"; break;
case 0xa0:current_instruction="jf";  break;
case 0xb0:current_instruction="rjm"; break;
case 0xc0:current_instruction="call";break;
case 0xd0:current_instruction="ret"; break;
case 0xe0:current_instruction="wait";break;
case 0xf0:current_instruction=(((instruction&0x08)>>8)?"sta":"lda");break;
default:current_instruction="unknown";break;
}
current_instruction+=" "+std::to_string(byte0);
current_instruction+=" "+std::to_string(byte1);


run_instruction(instruction,byte0,byte1);

counter+=3;
sim_step=false;
sim_tick++;
tick_counter=0;
}



if(sim_running){tick_counter+=sim_delta_time;}
}
}
