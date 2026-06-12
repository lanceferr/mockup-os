#include "task_manager.h"
#include <iostream>
#include <iomanip>

static const std::string SEPARATOR = "+-----------------------------------------------------------------------------------------+";
static const std::string DIVIDER = "|=========================================+========================+======================|";
static const std::string PROC_DIV = "|=========================================================================================|";

std::string formatProcessName(const std::string& name, size_t max_width) {
    if (name.length() <= max_width) {
        return name;
    }
    return "..." + name.substr(name.length() - (max_width - 3));
}

void printProcesses(const std::vector<Process>& processes) {
    std::cout << SEPARATOR << "\n";
    std::cout << "| Processes:                                                                              |\n";
    std::cout << "|    PID    Type Process name                                                       Usage |\n";
    std::cout << PROC_DIV << "\n";

    for (const auto& proc : processes) {
        std::cout << "| "
            << std::setw(7) << std::right << proc.pid << "  "
            << std::setw(4) << std::right << proc.type << "  "
            << std::setw(62) << std::left << formatProcessName(proc.name, 62) << " "
            << std::setw(9) << std::right << proc.mem_usage
            << " |\n";
    }

    std::cout << SEPARATOR << "\n";
}