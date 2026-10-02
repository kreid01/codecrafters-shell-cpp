#pragma once
#include <string>
#include <vector>

namespace utils {
std::vector<std::string> split(std::string string, char delimiter);
void not_found(std::string);
std::string erase_command(std::string, std::string);
bool char_in_array(const char &value, const std::vector<char> &array);
bool in_array(const std::string &value, const std::vector<std::string> &array);
std::vector<std::string> get_args(std::string arg);
std::vector<std::string> filter_on_match(std::vector<std::string> arr,
                                         std::string match);
} // namespace utils
