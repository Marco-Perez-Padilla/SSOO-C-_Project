#include "parse_args.h"
#include <vector>
#include <iostream>

std::expected<program_options, parse_args_errors> parse_args(int argc, char* argv[]) {
  bool file = false;
  std::vector<std::string_view> args(argv + 1, argv + argc);
  program_options options;
  for (auto it = args.begin(), end = args.end(); it != end; ++it) {
    if (*it == "-h" || *it == "--help") {
      options.show_help = true;
    } else if (*it == "-v" || *it == "--verbose") {
      options.extended_mode = true;
    } else if(!it->starts_with("-") && file == false) {
      options.output_filename = (std::string(*it)); // String de it
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
  std::cout << "docserver [-v | --verbose] [-h | --help] FILE\n"
            << "    [-v | --verbose]: Optional argument. If activated, every call to system library functions will be printed in the error stream\n"
            << "    [-h | --help]: Optional argument. Prints this help\n"
            << "    FILE: File to be used with this program\n"
            << "      If more than one file is specified, it will be interpreted as a wrong argument\n";
}
