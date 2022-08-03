#include "../core/log.h"

void Console::Log(std::string message, color type) { std::cout << __TIME__ << ": " << type << message << "\033[0m\n" << std::endl; }

struct textMod {
	const color blue = "\033[0;34m";
	const color red = "\033[1;31m";
	const color green = "\033[1;32m";
	const color yellow = "\033[0;33m";
	const color white = "\033[0;37m";
	const color black = "\033[0;30m";
	const color purple = "\033[0;35m";
	const color cyan = "\033[0;36m";
};