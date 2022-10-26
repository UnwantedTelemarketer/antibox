#include "../core/log.h"
#include "string"

void Console::Log(std::string message, color type = "\033[0;37m")
{ 
	std::cout << "[ Line " << __LINE__ << " ]: " << type << message << "\033[0m\n" << std::endl; 
}
void Console::Log(uint32_t message, color type = "\033[0;37m")
{
	std::cout << "[ Line " << __LINE__ << " ]: " << type << std::to_string(message) << "\033[0m\n" << std::endl;
}
void Console::Log(unsigned char* message, color type = "\033[0;37m")
{
	std::cout << "[ Line " << __LINE__ << " ]: " << type << message << "\033[0m\n" << std::endl;
}
