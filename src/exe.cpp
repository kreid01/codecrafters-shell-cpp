#include <cstdlib>
#include <filesystem>
#include <string>
#include <unistd.h>
#include <vector>

#include "tokenizer.h"
#include "utils.h"

#include "exe.h"

namespace exe {
std::string get_exe_path(std::string command) {
  char *path = std::getenv("PATH");
  std::vector<std::string> arr = utils::split(path, ':');

  for (const auto &path : arr)
    for (const auto &entry : std::filesystem::directory_iterator(path)) {
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

std::vector<std::string> get_args(std::string arg) {
  return utils::split(arg, ' ');
}

void try_run(std::string command) {
  std::vector<std::string> args = tokenizer::format_string(command, false);

  if (is_executable(args[0])) {
    std::system(command.c_str());
  } else {
    utils::not_found(command);
  }
}
} // namespace exe
