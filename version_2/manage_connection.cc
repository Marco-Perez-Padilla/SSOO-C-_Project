/**
 * Universidad de La Laguna
 * Escuela Superior de Ingenieria y Tecnologia
 * Grado en Ingenieria Informatica
 * Asignatura: Sistemas Operativos
 * Curso: 2º
 * Proyecto de C++: Programación de aplicaciones — Servidor de documentación
 * Autor: Marco Perez Padilla

 * Correo: alu0101469348@ull.edu.es
 * Fecha: 26/11/2024

 * Archivo mannage_connection.cc: Contiene las implementaciones necesarias para manejar la comunicación mediante sockets

 * Historial de revisiones:
 *      26/11/2024 - Primera version (creacion) del codigo
**/

#include <sys/socket.h> // socket

#include "manage_connection.h"


/**
 * @brief Function to get any environment variable
 * @param string name of the variable. For instance: DOCSERVER_PORT.
 * @return String with the result. Empty if the variable hasn't been found.
 */
std::string getenv(const std::string& name) {
  char* value = getenv(name.c_str());
  if (value) {
    return std::string(value);
  } else {
  return std::string();
  }
}


/**
 * @brief Function to create a socket given a determined port
 * @param uint16_t number of the port
 * @return SafeFD file descriptor is no error has occurred. Code errno otherwise
 */
std::expected<SafeFD, int> make_socket(uint16_t port) {
  SafeFD fd (socket(AF_INET, SOCK_STREAM, 0));
  if (fd.get() < 0) {
    return std::unexpected(errno);
  }
  sockaddr_in local_address{};                            
  local_address.sin_family = AF_INET;                    
  local_address.sin_addr.s_addr = htonl(port);      
  local_address.sin_port = htons(0);                      

  SafeFD result_fd (bind(fd.get(), reinterpret_cast<const sockaddr*>(&local_address), sizeof(local_address)));

  if (result_fd.get() < 0) {
    return std::unexpected(errno);
  }
  return result_fd;
}


/**
 * @brief Function to put a determined socket on listen
 * @param SafeFD socket to put on listen
 * @return errno code if any error has occurred. EXIT_SUCCESS code otherwise.
 */
int listen_connection(const SafeFD& socket) {
  int result = listen(socket.get(), 5);
  if (result < 0) {
    return errno;
  }
  return EXIT_SUCCESS; //ESUCCESS??
}


/**
 * @brief Function to accept a connection
 * @param safeFD server socket 
 * @param sockaddr_in client socket address
 * @return SafeFD file descriptor is no error has occurred. Code errno otherwise
 */
std::expected<SafeFD, int> accept_connection(const SafeFD& socket,sockaddr_in& client_addr) {
  socklen_t source_address_length;
  SafeFD new_fd (accept(socket.get(), reinterpret_cast<sockaddr*>(&client_addr), &source_address_length));
  if (new_fd.get() < 0) {
    return std::unexpected(errno);
  }
  // Procesar la nueva conexión...
  return new_fd;
}


/**
 * @brief
 * @param
 * @param
 * @param
 * @return
 */
int send_response(const SafeFD& socket, std::string_view header, std::string_view body = {}) {

}
