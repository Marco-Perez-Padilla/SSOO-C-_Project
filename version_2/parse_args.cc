#include "parse_args.h"
#include <vector>
#include <iostream>
#include <algorithm>

std::expected<program_options, parse_args_errors> parse_args(int argc, char* argv[]) {
  bool file = false;
  bool port = false;
  std::vector<std::string_view> args(argv + 1, argv + argc);
  program_options options;
  for (auto it = args.begin(), end = args.end(); it != end; ++it) {
    if (*it == "-h" || *it == "--help") {
      options.show_help = true;
    } else if (*it == "-v" || *it == "--verbose") {
      options.extended_mode = true;
    } else if (*it == "-p" || *it == "--port") {
      port = true;
      continue;
    } else if (port == true) {
      port = false;
      if (std::all_of(it->begin(), it->end(), isdigit)) {
        options.port = std::stoi(std::string(*it));
      } else {
        options.output_filename = std::string(*it);
        file = true;
      }
    } else if(!it->starts_with("-") && file == false) {
      options.output_filename = std::string(*it); 
      file = true;
    } else {
      return std::unexpected(parse_args_errors::unknown_option); 
    }
  }

  if (file == false && options.show_help == false) {
    return std::unexpected(parse_args_errors::missing_argument);
  }

  return options; 
}


void print_usage () {
  std::cout << "This is the help for using this program" << std::endl;
}
