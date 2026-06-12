#pragma once
#include <string>
#include <vector>

struct Process {
    int pid;
    std::string type;
    std::string name;
    std::string mem_usage;
};

std::string formatProcessName(const std::string& name, size_t max_width);
void printProcesses(const std::vector<Process>& processes);