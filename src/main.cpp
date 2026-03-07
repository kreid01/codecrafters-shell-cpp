#include <iostream>
#include <string>

void repl() {
  std::string command;

  std::cout << "$ ";

  std::cin >> command;

  if (command == "exit")
    exit(0);
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
