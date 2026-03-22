#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <ostream>
#include <string>
#include <unistd.h>
#include <vector>

namespace fs = std::filesystem;

std::vector<std::string> built_ins = {"exit", "echo", "type"};

std::string erase_command(std::string command, std::string built_in) {
  std::int32_t length = built_in.length();
  return command.erase(0, length + 1);
}

void echo(std::string command) {
  std::string response = erase_command(command, "echo");
  std::cout << response << std::endl;
}

bool in_array(const std::string &value, const std::vector<std::string> &array) {
  return std::find(array.begin(), array.end(), value) != array.end();
}

void not_found(std::string command) {
  std::cout << command << ": command not found" << std::endl;
}

std::vector<std::string> split(std::string string) {
  std::vector<std::string> array;
  std::string tempWord = "";

  for (std::uint8_t i = 0; i < string.length(); i++) {
    if (string[i] == ':') {
      array.push_back(tempWord);
      tempWord = "";
    } else {
      tempWord += string[i];
    }
  }

  return array;
}

std::string get_exe_path(std::string command) {
  char *path = std::getenv("PATH");
  std::vector<std::string> arr = split(path);

  for (const auto &path : arr)
    for (const auto &entry : fs::directory_iterator(path)) {
      std::filesystem::path full_path = std::filesystem::path(path) / command;
      if (access(full_path.c_str(), X_OK) == 0) {
        return full_path.c_str();
      }
    }

  return std::string();
}

bool is_executable(std::string command) {
  return !get_exe_path(command).empty();
}

void type(std::string command) {
  std::string response = erase_command(command, "type");

  if (in_array(response, built_ins)) {
    std::cout << response << " is a shell builtin\n";
  } else if (is_executable(response)) {
    std::string path = get_exe_path(response);
    std::cout << response << " is " << path << std::endl;

  } else {
    std::cout << response << ": not found" << std::endl;
  }
}

void repl() {
  std::string command;

  std::cout << "$ ";

  std::getline(std::cin, command);

  if (command.find("exit") == 0)
    exit(0);
  else if (command.find("echo") == 0)
    echo(command);
  else if (command.find("type") == 0) {
    type(command);
  } else
    not_found(command);
}

int main() {
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (1) {
    repl();
  }
}
