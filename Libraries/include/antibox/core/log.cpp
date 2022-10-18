#include "../core/log.h"

void Console::Log(std::string message, color type = "\033[0;37m")
{ 
	std::cout << "[ Line " << __LINE__ << " ]: " << type << message << "\033[0m\n" << std::endl; 
}
