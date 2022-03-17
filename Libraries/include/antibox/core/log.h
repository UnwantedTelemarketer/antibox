#pragma once
#include <iostream>


#define ANTIBOX_ERROR(string) std::cout << "Error at line " << __LINE__ << " in file " << __FILE__ << " at " << __TIME__ << ". "; std::cout << "\033[1;31m" << string << "\033[0m\n" << std::endl;
#define ANTIBOX_ERRLOG(string, log) std::cout << "Error at line " << __LINE__ << " in file " << __FILE__ << " at " << __TIME__ << ". "; std::cout << "\033[1;31m" << string << "\033[0m\n" << std::endl; std::cout << log << std::endl;
#define PRINT_GREEN(string) std::cout << "\033[1;32m" << string << "\033[0m\n" << std::endl;
