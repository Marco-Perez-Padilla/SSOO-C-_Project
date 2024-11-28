/**
 * Universidad de La Laguna
 * Escuela Superior de Ingenieria y Tecnologia
 * Grado en Ingenieria Informatica
 * Asignatura: Sistemas Operativos
 * Curso: 2º
 * Proyecto de C++: Programación de aplicaciones — Servidor de documentación
 * Autor: Marco Perez Padilla

 * Correo: alu0101469348@ull.edu.es
 * Fecha: 23/11/2024

 * Archivo mannage_connection.cc: Contiene las implementaciones necesarias para manejar la comunicación mediante sockets

 * Historial de revisiones:
 *      23/11/2024 - Primera version (creacion) del codigo
 *      25/11/2024 - Arreglo de casos -h y -p
 *      26/11/2024 - Adicion e integracion de la clase SafeMap
**/

#include <iostream>
#include <vector>
#include <string_view>
#include <format>
#include <cstring> //strerror

#include "parse_args.h"
#include "manage_file.h"
#include "manage_connection.h"

int main(int argc, char* argv[]) {
  auto options = parse_args(argc, argv); 
  if (!options.has_value()) {
    // Usar options.error() para comprobar el motivo del error...
    if (options.error() == parse_args_errors::missing_argument) {
      std::cerr << "fatal error: Missing arguments" << std::endl;
    } else if (options.error() == parse_args_errors::unknown_option) {
      std::cerr << "fatal error: Unknown option" << std::endl;;
    }
    return EXIT_FAILURE;
  }
  // Usar options.value() para acceder a las opciones...
  if (options.value().show_help) {
    print_usage(); 
    return EXIT_SUCCESS;
  }
  
  if (options.value().extended_mode) {
    std::cout << "Program is in extended mode. Every call to system library functions will be printed in the standard error stream." << std::endl;
    std::cout << std::endl;
  }

    auto socket = make_socket(options.value().port, options.value().extended_mode);
    if (!socket) { 
      std::cerr << "fatal error: Error creating the socket " << std::endl;
      return EXIT_FAILURE;
    }

    int listen = listen_connection(socket.value(), options.value().extended_mode);
    if (listen != 0) {
      std::cerr << "fatal error: It was not possible to put the socket on listening" << std::endl;
      return EXIT_FAILURE;
    }

    while (true) {
      sockaddr_in local_address{};                            
      local_address.sin_family = AF_INET;                    
      local_address.sin_addr.s_addr = htonl(INADDR_ANY);      
      local_address.sin_port = htons(0);  
      auto accepted_socket = accept_connection(socket.value(), local_address, options.value().extended_mode);
      if (!accepted_socket) {
        std::cerr << "fatal error: Error accepting the connection" << std::endl;
        return EXIT_FAILURE;
      }

      //Receive_request

      auto result = read_all(options.value().output_filename, options.value().extended_mode);
      if (!result) { 
        std::cerr << "Error " << result.error() << ": ";
        if (result.error() == 404) {
          std::cerr << "Not Found" << std::endl;
        } else if (result.error() == 403) {
          std::cerr << "Forbidden" << std::endl;
        } else {
          std::cerr << "fatal error: Unknown error reading the file" << std::endl;
          return EXIT_FAILURE;
        }
      }
      std::string_view header = std::format("FileSize: {}\n", result.value().get().size());
      int sent = send_response(accepted_socket.value(), header, options.value().extended_mode, result.value().get());
      if (sent != 0) {
        if (sent == ECONNRESET) {
          std::cerr << "Warning: A minor error has occurred while sending the response" << std::endl;
        } else {
          std::cerr << "fatal error: Error sending the response" << strerror(sent) << std::endl;
          return EXIT_FAILURE;  
        }
      }
    }
  
  return EXIT_SUCCESS;
}
