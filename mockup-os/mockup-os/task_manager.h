#pragma once

#include "palette.h"
#include <string>
#include <vector>

struct Process {
    int pid;
    std::string type;
    std::string name;
    std::string mem_usage;
};

std::string formatProcessName(const std::string& name, size_t maxLen);
void DrawTaskManager(bool& show, float sw, float sh, const std::vector<Process>& processes);