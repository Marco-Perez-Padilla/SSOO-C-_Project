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
 *      29/11/2024 - Adicion e integracion de la nueva funcionalidad en base a la opcion -b
**/

#include <iostream>
#include <vector>
#include <string_view>
#include <format>
#include <cstring> //strerror
#include <regex>

#include "parse_args.h"
#include "manage_file.h"
#include "manage_connection.h"


#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
  auto options = parse_args(argc, argv); 
  if (!options.has_value()) {

    if (options.error() == parse_args_errors::missing_argument) {
      std::cerr << "fatal error: Missing arguments" << std::endl;
    } else if (options.error() == parse_args_errors::unknown_option) {
      std::cerr << "fatal error: Unknown option" << std::endl;;
    }
    return EXIT_FAILURE;
  }

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
    if (options.value().extended_mode) {
      std::cerr << "htonl(): Se convierte la dirección IP en formato de red, Big Endian" << std::endl;
    } 
    local_address.sin_port = htons(0); 
    if (options.value().extended_mode) {
      std::cerr << "htons(): Se convierte el número del puerto en formato de red, Big Endian" << std::endl;
    }  
    auto accepted_socket = accept_connection(socket.value(), local_address, options.value().extended_mode);
    if (!accepted_socket) {
      std::cerr << "fatal error: Error accepting the connection" << std::endl;
      return EXIT_FAILURE;
    }

    //Receive_request
    auto received = receive_request(accepted_socket.value(), max_path_size, options.value().extended_mode);
    if (!received) {
      if (received.error() != ECONNRESET) {
        std::cerr << "fatal error: Error receiving the request" << std::endl;
        return EXIT_FAILURE;
      } else {
        std::cerr << "Warning: Minor error receiving the request" << std::endl;
      }
    }

    //Process_petition
    auto processed = process_petition(received.value());
    if (!processed) {
      if (processed.error() == 400) {
        std::string header ("Error 400: Bad Request\n");
        int sent = send_response(accepted_socket.value(), header, options.value().extended_mode);
        if (sent != 0) {
          if (sent == ECONNRESET) {
            std::cerr << "Warning: A minor error has occurred while sending the response" << std::endl;
          } else {
            std::cerr << "fatal error: Error sending the response" << strerror(sent) << std::endl;
            return EXIT_FAILURE;  
          }
        }
        continue;
      }
    }

    // vting the whole name
    options.value().output_filename = (options.value().BASE_DIR + processed.value());
    std::string name = options.value().output_filename;

    //Regex to identify "bin" in the name
    std::smatch reslt;
    std::regex pattern (R"(.*/bin/.*)");
    bool reg_result = std::regex_search(name, reslt, pattern);

    // If there is "bin"
    if (reg_result) {
      // setting the env
      exec_environment env;
      env.request_path = options.value().output_filename;
      env.server_basedir = options.value().BASE_DIR;
      env.remote_port = local_address.sin_port;
      
      char ip_buffer[INET_ADDRSTRLEN];
      if (inet_ntop(AF_INET, &local_address.sin_addr, ip_buffer, INET_ADDRSTRLEN)) {
        env.remote_ip = std::string(ip_buffer);
        if (options.value().extended_mode) {
          std::cerr << "inet_ntop(): se convierte la direccion IP a texto" << std::endl;
        }
      } else {
        std::cerr << "Error converting IP address." << std::endl;
        return EXIT_FAILURE;
      }

      // Execute
      auto result = execute_program(name, env, options.value().extended_mode);
      // If there was an error
      if (!result) {
        std::cerr << "Error en execute_program: " << result.error().exit_code << " (code: " << result.error().error_code << ")" << std::endl;
        if (result.error().exit_code == "Not found" || result.error().exit_code == "Forbidden") {
          continue;
        } else if (result.error().exit_code != "Error creating the pipeline" || result.error().exit_code != "Error creating the child process" || result.error().exit_code != "Error creating the dup") {
          std::string_view header = result.error().exit_code + "\n";
          int sent = send_response(accepted_socket.value(), header, options.value().extended_mode);
          if (sent != 0) {
            if (sent == ECONNRESET) {
              std::cerr << "Warning: A minor error has occurred while sending the response" << std::endl;
            } else {
              std::cerr << "fatal error: Error sending the response" << strerror(sent) << std::endl;
              return EXIT_FAILURE;  
            }
          }
          return EXIT_FAILURE;
        } else { // Si el error es desconocido
          std::string_view header = "500 Internal Server Error\n";
          int sent = send_response(accepted_socket.value(), header, options.value().extended_mode);
          if (sent != 0) {
            if (sent == ECONNRESET) {
              std::cerr << "Warning: A minor error has occurred while sending the response" << std::endl;
            } else {
              std::cerr << "fatal error: Error sending the response" << strerror(sent) << std::endl;
              return EXIT_FAILURE;  
            }
          }
          return EXIT_FAILURE;
        }
      }

      std::string_view header = std::format("FileSize: {}\n\n", result.value().size());

      int sent = send_response(accepted_socket.value(), header, options.value().extended_mode, result.value().data());
      if (sent != 0) {
        if (sent == ECONNRESET) {
          std::cerr << "Warning: A minor error has occurred while sending the response" << std::endl;
        } else {
          std::cerr << "fatal error: Error sending the response" << strerror(sent) << std::endl;
          return EXIT_FAILURE;  
        }
      }
    } else { // If not /bin
      auto result = read_all(options.value().output_filename, options.value().extended_mode);
      if (!result) { 
        std::cerr << "Error " << result.error() << ": ";
        if (result.error() == 404) {
          std::cerr << "Not Found" << std::endl;
          continue;
        } else if (result.error() == 403) {
          std::cerr << "Forbidden" << std::endl;
          continue;
        } else {
          std::cerr << "fatal error: Unknown error reading the file" << std::endl;
          std::string_view header = "500 Internal Server Error\n";
          int sent = send_response(accepted_socket.value(), header, options.value().extended_mode);
          if (sent != 0) {
            if (sent == ECONNRESET) {
              std::cerr << "Warning: A minor error has occurred while sending the response" << std::endl;
            } else {
              std::cerr << "fatal error: Error sending the response" << strerror(sent) << std::endl;
              return EXIT_FAILURE;  
            }
          }
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
  }
  return EXIT_SUCCESS;
}
