#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> built_ins = {"exit", "echo", "type"};

std::string erase_command(std::string command, std::string built_in) {
  std::int32_t length = built_in.length();
  return command.erase(0, length + 1);
}

void echo(std::string command) {
  std::string response = erase_command(command, "echo");
  std::cout << response << "\n";
}

bool in_array(const std::string &value, const std::vector<std::string> &array) {
  return std::find(array.begin(), array.end(), value) != array.end();
}

void not_found(std::string command) {
  std::cout << command << ": command not found\n";
}

void type(std::string command) {
  std::string response = erase_command(command, "type");

  if (in_array(response, built_ins)) {
    std::cout << response << " is a shell builtin\n";
  } else {
    std::cout << response << ": not found\n";
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
