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
    }
    if (*it == "-v" || *it == "--verbose") {
      options.extended_mode = true;
    }
    // Procesar otras opciones...
    else if(!it->starts_with("-") && file == false) {
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
  std::cout << "This is the help for using this program" << std::endl;
}
