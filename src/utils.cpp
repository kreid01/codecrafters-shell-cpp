#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

#include "utils.h"

namespace utils {
std::string erase_command(std::string command, std::string built_in) {
  std::int32_t length = built_in.length();
  return command.erase(0, length + 1);
}

bool in_array(const std::string &value, const std::vector<std::string> &array) {
  return std::find(array.begin(), array.end(), value) != array.end();
}

bool char_in_array(const char &value, const std::vector<char> &array) {
  return std::find(array.begin(), array.end(), value) != array.end();
}

void not_found(std::string command) {
  std::cout << command << ": command not found" << std::endl;
}

bool string_contains(std::string string, char sub) {
  for (std::int8_t i = 0; i < string.length(); i++) {
    if (string[i] == sub) {
      return true;
    }
  }

  return false;
}

std::vector<std::string> get_args(std::string arg) {
  return utils::split(arg, ' ');
}

std::vector<std::string> filter_on_match(std::vector<std::string> arr,
                                         std::string match) {
  std::vector<std::string> result = std::vector<std::string>();
  for (int i = 0; i < arr.size(); i++) {
    if (arr[i] != match) {
      result.push_back(arr[i]);
    }
  }

  return result;
}

std::vector<std::string> split(std::string string, char delimiter) {
  if (!string_contains(string, delimiter)) {
    return std::vector<std::string>{string};
  }

  std::vector<std::string> array;
  std::string tempWord = "";

  for (std::uint8_t i = 0; i < string.length(); i++) {
    if (string[i] == delimiter && tempWord != std::string()) {
      array.push_back(tempWord);
      tempWord = "";
    } else {
      tempWord += string[i];
    }
  }

  return array;
}

} // namespace utils
