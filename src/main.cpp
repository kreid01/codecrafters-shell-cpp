#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

#include "exe.h"
#include "tokenizer.h"
#include "utils.h"

namespace fs = std::filesystem;

std::vector<std::string> built_ins = {"exit", "echo", "type", "pwd"};

namespace tokenizer {
std::vector<std::string> format_string(std::string, bool);
}

void type(std::string command) {
  std::string response = utils::erase_command(command, "type");

  if (utils::in_array(response, built_ins)) {
    std::cout << response << " is a shell builtin\n";
  } else if (exe::is_executable(response)) {
    std::string path = exe::get_exe_path(response);
    std::cout << response << " is " << path << std::endl;

  } else {
    std::cout << response << ": not found" << std::endl;
  }
}

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
  std::string response = utils::erase_command(command, "cd");
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

void create_and_write_to_file(std::string file_name, std::string contents) {
  std::filesystem::path path(file_name);
  std::filesystem::create_directories(path.parent_path());

  std::ofstream file(file_name);

  if (!file) {
    return;
  }

  file << contents;
}

std::vector<std::string> echo(std::string command, bool echo) {
  std::string response = utils::erase_command(command, "echo");
  return tokenizer::format_string(response, echo);
}

std::string trim(std::string str) {
  std::string result = std::string();
  for (int i = 0; i < str.length(); i++) {
    if ((i == 0 || i == str.length() - 1) && str[i] == ' ') {
      continue;
    } else {
      result.push_back(str[i]);
    }
  }

  return result;
}

enum Redirect { OUTPUT, NONE };

struct RedirectOptions {
  std::string Value;
  std::string Destination;
  enum Redirect Redirect;
};

std::string append(std::string a, std::string b) {
  std::string value = a;

  if (value.length() == 0) {
    return b;
  }

  value += " ";
  value += b;

  return value;
}

RedirectOptions get_redirect_options(std::vector<std::string> args) {
  std::string value = std::string();
  Redirect redirect = Redirect::NONE;

  std::string destination = std::string();
  bool dest = false;

  for (int i = 0; i < args.size(); i++) {
    std::string curr = args[i];

    if (curr == "1" || curr == "1>") {
      redirect = Redirect::OUTPUT;
      dest = true;
    } else {
      if (!dest) {
        value = append(value, curr);
      } else {
        destination = curr;
      }
    }
  }

  value += "\n";

  return {value, destination, redirect};
}

void repl() {
  std::string command;

  std::cout << "$ ";

  std::getline(std::cin, command);

  if (command.find("exit") == 0)
    exit(0);
  else if (command.find("echo") == 0) {
    std::vector<std::string> args = echo(command, false);
    RedirectOptions redirectOptions = get_redirect_options(args);

    if (redirectOptions.Redirect == Redirect::OUTPUT) {
      create_and_write_to_file(redirectOptions.Destination,
                               redirectOptions.Value);
    } else {

      std::vector<std::string> ech = echo(command, true);
      for (int i = 0; i < ech.size(); i++) {
        std::cout << ech[i];
      }
      std::cout << std::endl;
    }
  } else if (command.find("type") == 0) {
    type(command);
  } else if (command.find("pwd") == 0) {
    pwd(command);
  } else if (command.find("cd") == 0) {
    cd(command);
  } else {
    exe::try_run(command);
  }
}

int main() {
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (1) {
    repl();
  }
}
