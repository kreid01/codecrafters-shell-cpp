#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <ostream>
#include <string>
#include <unistd.h>
#include <vector>

namespace fs = std::filesystem;

std::vector<std::string> built_ins = {"exit", "echo", "type", "pwd"};

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

std::vector<std::string> split(std::string string, char sub) {
  if (!string_contains(string, sub)) {
    return std::vector<std::string>{string};
  }

  std::vector<std::string> array;
  std::string tempWord = "";

  for (std::uint8_t i = 0; i < string.length(); i++) {
    if (string[i] == sub) {
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
  std::vector<std::string> arr = split(path, ':');

  for (const auto &path : arr)
    for (const auto &entry : fs::directory_iterator(path)) {
      std::filesystem::path full_path = std::filesystem::path(path) / command;
      if (access(full_path.c_str(), X_OK) == 0 &&
          std::filesystem::exists(full_path.c_str())) {
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

std::vector<std::string> get_args(std::string args) { return split(args, ' '); }

void pwd(std::string command) {
  std::string current_dir = fs::current_path();
  std::cout << current_dir << std::endl;
}

void home() {
  char *home = std::getenv("HOME");
  std::filesystem::current_path(home);
}

void cd(std::string command) {
  std::string current_dir = fs::current_path();
  std::string response = erase_command(command, "cd");
  std::string new_path = std::filesystem::path(current_dir) / response;

  if (response == "~") {
    home();
  } else if (std::filesystem::exists(new_path)) {
    std::filesystem::current_path(new_path);
  } else {
    std::cout << "cd: " << response << ": No such file or directory"
              << std::endl;
  }
}

std::vector<std::string> format_single_quotes(std::string command) {
  std::vector<std::string> result = std::vector<std::string>();
  std::string curr = std::string();

  for (std::uint8_t i = 0; i < command.length(); i++) {
    if (command[i] == '\'') {
      result.push_back(curr);
      curr = std::string();
      continue;
    }

    curr.push_back(command[i]);
  }

  if (curr != std::string()) {
    result.push_back(curr);
  }

  return result;
}

std::vector<std::string> format_no_quotes(std::string command) {
  std::vector<std::string> result = std::vector<std::string>();
  std::string curr = std::string();

  for (std::uint8_t i = 0; i < command.length(); i++) {
    char current_char = command[i];
    char next_char = command[i + 1];

    std::vector<char> escapes = std::vector<char>{'\\', '\'', '\"'};

    if (current_char == '\\') {
      if (char_in_array(next_char, escapes)) {
        curr.push_back(next_char);
        i++;
      } else if (next_char == '\x20') {
        curr.push_back(' ');
        i++;
      }

      continue;
    }

    if (current_char == '\x20' && next_char == '\x20') {
      continue;
    }

    curr.push_back(current_char);
  }

  result.push_back(curr);

  return result;
}

std::vector<std::string> format_double_quotes(std::string command) {
  std::vector<std::string> result = std::vector<std::string>();
  std::string curr = std::string();
  bool in_quotes = false;

  for (std::uint8_t i = 0; i < command.length(); i++) {
    char current_char = command[i];
    char next_char = command[i + 1];

    bool backslash = current_char == '\\';

    if (!in_quotes && current_char == '\x20' && next_char == '\x20') {
      continue;
    }

    if (backslash) {
      if (next_char == '\"' || next_char == '\\') {
        curr.push_back(next_char);
      }

      i++;
      continue;
    }

    if (current_char == '\"') {
      in_quotes = !in_quotes;

      result.push_back(curr);
      curr = std::string();

      continue;
    }

    curr.push_back(current_char);
  }

  if (curr != std::string()) {
    result.push_back(curr);
  }

  return result;
}

std::vector<std::string> format_string(std::string arg) {
  if (arg[0] == '\'') {
    return format_single_quotes(arg);
  } else if (arg[0] == '\"') {
    return format_double_quotes(arg);
  } else {
    return format_no_quotes(arg);
  }
}

void try_run(std::string command) {
  std::vector<std::string> args = get_args(command);

  std::vector<std::string> formatted_arg = format_string(args[0]);

  if (is_executable(formatted_arg[0])) {
    std::system(command.c_str());
  } else {
    not_found(command);
  }
}

void echo(std::string command) {
  std::string response = erase_command(command, "echo");
  std::vector<std::string> formatted_command = format_string(response);

  for (int i = 0; i < formatted_command.size(); i++) {
    std::cout << formatted_command[i];
  }

  std::cout << std::endl;
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
  } else if (command.find("pwd") == 0) {
    pwd(command);
  } else if (command.find("cd") == 0) {
    cd(command);
  } else if (command.find("echo") == 0) {
    echo(command);
  } else
    try_run(command);
}

int main() {
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (1) {
    repl();
  }
}
