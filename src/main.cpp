#include <cstdint>
#include <iostream>
#include <string>

std::string erase_command(std::string command, std::string built_in) {
  std::int32_t length = built_in.length();
  return command.erase(0, length + 1);
}

void echo(std::string command) {
  std::string response = erase_command(command, "echo");
  std::cout << response << "\n";
}

void repl() {
  std::string command;

  std::cout << "$ ";

  std::getline(std::cin, command);

  if (command.find("exit") == 0)
    exit(0);
  if (command.find("echo") == 0)
    echo(command);
  else
    std::cout << command << ": command not found\n";
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (1) {
    repl();
  }
}
