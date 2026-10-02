#include <cstdint>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <vector>

#include "utils.h"

namespace tokenizer {
std::vector<std::string> format_single_quotes(std::string command) {
  std::vector<std::string> result = std::vector<std::string>();
  std::string curr = std::string();
  bool in_quotes = false;

  for (std::uint8_t i = 0; i < command.length(); i++) {
    char current_char = command[i];
    char next_char = command[i + 1];

    if (current_char == '\'') {
      if (in_quotes) {
        result.push_back(curr);
        curr = std::string();
      }

      in_quotes = !in_quotes;
      continue;
    }

    curr.push_back(command[i]);
  }

  if (curr != std::string()) {
    result.push_back(curr);
  }

  return result;
}

std::vector<std::string> format_no_quotes(std::string command, bool echo) {
  std::vector<std::string> result = std::vector<std::string>();
  std::string curr = std::string();

  for (std::uint8_t i = 0; i < command.length(); i++) {
    char current_char = command[i];
    char next_char = command[i + 1];

    std::vector<char> escapes = std::vector<char>{'\\', '\'', '\"'};

    if (current_char == '\\') {
      if (utils::char_in_array(next_char, escapes)) {
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

    if (current_char == '\'') {
      result.push_back(curr);
      curr = std::string();
      continue;
    }

    if (current_char == '\x20' && !echo) {
      result.push_back(curr);
      curr = std::string();
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
      if (in_quotes) {
        result.push_back(curr);
        curr = std::string();
      }

      in_quotes = !in_quotes;

      continue;
    }

    curr.push_back(current_char);
  }

  if (curr != std::string()) {
    result.push_back(curr);
  }

  return result;
}

std::vector<std::string> format_string(std::string arg, bool echo) {
  if (arg[0] == '\'') {
    return format_single_quotes(arg);
  } else if (arg[0] == '\"') {
    return format_double_quotes(arg);
  } else {
    return format_no_quotes(arg, echo);
  }
}

} // namespace tokenizer
