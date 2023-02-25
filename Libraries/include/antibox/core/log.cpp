#include "../core/log.h"
#include "string"
#include "mathlib.h"

void Console::Log(std::string message, textColor type = "\033[0;37m", int lineNum = -1)
{ 
	std::cout << "[ Line " << lineNum << " ]: " << type << message << "\033[0m\n" << std::endl; 
}
void Console::Log(uint32_t message, textColor type = "\033[0;37m", int lineNum = -1)
{
	std::cout << "[ Line " << lineNum << " ]: " << type << std::to_string(message) << "\033[0m\n" << std::endl;
}
void Console::Log(bool tf, textColor type = "\033[0;37m", int lineNum = -1)
{
	std::string message = tf ? "True" : "False";
	std::cout << "[ Line " << lineNum << " ]: " << type << message << "\033[0m\n" << std::endl;
}
void Console::Log(Vector2 vec, textColor type = "\033[0;37m", int lineNum = -1)
{
	std::cout << "[ Line " << lineNum << " ]: " << type << "Vector2 {" << std::to_string(vec.x) + ", " + std::to_string(vec.y) << "}" << "\033[0m\n" << std::endl;
}
void Console::Log(Vector2_I vec, textColor type = "\033[0;37m", int lineNum = -1)
{
	std::cout << "[ Line " << lineNum << " ]: " << type << "Vector2 {" << std::to_string(vec.x) + ", " + std::to_string(vec.y) << "}" << "\033[0m\n" << std::endl;
}
void Console::Log(unsigned char* message, textColor type = "\033[0;37m", int lineNum = -1)
{
	std::cout << "[ Line " << lineNum << " ]: " << type << message << "\033[0m\n" << std::endl;
}
