#pragma once
#include <string>

namespace exe {
bool is_executable(std::string);
std::string get_exe_path(std::string);
void try_run(std::string);
} // namespace exe
