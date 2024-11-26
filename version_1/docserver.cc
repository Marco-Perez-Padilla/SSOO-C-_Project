#include <iostream>
#include <vector>
#include <string_view>
#include <format>

#include "parse_args.h"
#include "manage_file.h"


int main(int argc, char* argv[]) {
  auto options = parse_args(argc, argv); 
  if (!options.has_value()) {
    // Usar options.error() para comprobar el motivo del error...
    if (options.error() == parse_args_errors::missing_argument) {
      std::cerr << "fatal error: Missing arguments" << std::endl;
    } else if (options.error() == parse_args_errors::unknown_option) {
      std::cerr << "fatal error: Unknown option" << std::endl;;
    }
    // ...
    return EXIT_FAILURE;
  }
  
  // Usar options.value() para acceder a las opciones...
  if (options.value().show_help == true) {
    print_usage(); 
    return EXIT_SUCCESS;
  }
  
  if (options.value().extended_mode) {
    std::cout << "Program is in extended mode. Every call to system library functions will be printed in the standard error stream." << std::endl;
    std::cout << std::endl;
  }

  if (!options.value().output_filename.empty()) {
    
    auto result = read_all(options.value().output_filename, options.value().extended_mode);
    if (!result) { 
      std::cerr << "Error " << result.error() << ": ";
      if (result.error() == 404) {
        std::cerr << "Not Found" << std::endl;
      } else if (result.error() == 403) {
        std::cerr << "Forbidden" << std::endl;
      }
      return EXIT_SUCCESS;
    }

    std::string_view header = std::format("FileSize: {}\n", result.value().get().size()); // Si pongo un espacio o "-" salen cosas como: �z����Y��'}
    send_response(header, result.value().get());
  }
  
  return EXIT_SUCCESS;
}
