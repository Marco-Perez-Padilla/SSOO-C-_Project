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
 *      27/11/2024 - Adicion de funcion send_response() y eliminacion de getenv()
**/

#include <sys/socket.h> // socket
#include <iostream>

#include "manage_connection.h"


/**
 * @brief Function to create a socket given a determined port
 * @param uint16_t number of the port
 * @param bool extended to show if the program is in extended mode
 * @return SafeFD file descriptor is no error has occurred. Code errno otherwise
 */
std::expected<SafeFD, int> make_socket(uint16_t port, bool extended) {
  SafeFD fd (socket(AF_INET, SOCK_STREAM, 0));
  if (fd.get() < 0) {
    return std::unexpected(errno);
  }
  if (extended) {
    std::cerr << "socket(): Se crea el socket" << std::endl;
  }

  int option=1;
  setsockopt(fd.get(), SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
  if (extended) {
    std::cerr << "setsockopt(): Se cambian las opciones del socket" << std::endl;
  }
  
  sockaddr_in local_address{};                            
  local_address.sin_family = AF_INET;                    
  local_address.sin_addr.s_addr = htonl(INADDR_ANY);      
  if (extended) {
    std::cerr << "htonl(): Se convierte la dirección IP en formato de red, Big Endian" << std::endl;
  }
  local_address.sin_port = htons(port);     
  if (extended) {
    std::cerr << "htons(): Se convierte el número del puerto en formato de red, Big Endian" << std::endl;
  }                 

  int result_fd (bind(fd.get(), reinterpret_cast<const sockaddr*>(&local_address), sizeof(local_address)));
  if (result_fd < 0) {
    return std::unexpected(errno);
  }
  if (extended) {
    std::cerr << "bind(): Se asocia una dirección de red al socket" << std::endl;
  }

  if (extended) {
    std::cerr << "close(): se cierra el archivo con descriptor de archivo \"" << fd.get() << "\" " << std::endl;
  }

  return fd;
}


/**
 * @brief Function to put a determined socket on listen
 * @param SafeFD socket to put on listen
 * @param bool extended to show if the program is in extended mode
 * @return errno code if any error has occurred. EXIT_SUCCESS code otherwise.
 */
int listen_connection(const SafeFD& socket, bool extended) {
  int result = listen(socket.get(), 5);
  if (result < 0) {
    return errno;
  }
  if (extended) {
    std::cerr << "listen(): Se pone el socket en modo escucha" << std::endl;
  }
  return EXIT_SUCCESS; 
}


/**
 * @brief Function to accept a connection
 * @param safeFD server socket 
 * @param sockaddr_in client socket address
 * @param bool extended to show if the program is in extended mode
 * @return SafeFD file descriptor is no error has occurred. Code errno otherwise
 */
std::expected<SafeFD, int> accept_connection(const SafeFD& socket,sockaddr_in& client_addr, bool extended) {
  socklen_t source_address_length;
  SafeFD new_fd (accept(socket.get(), reinterpret_cast<sockaddr*>(&client_addr), &source_address_length));
  if (new_fd.get() < 0) {
    return std::unexpected(errno);
  }
  if (extended) {
    std::cerr << "accept(): Se acepta la conexión en el socket" << std::endl;
  }

  if (extended) {
    std::cerr << "close(): se cierra el archivo con descriptor de archivo \"" << new_fd.get() << "\" " << std::endl;
  }

  return new_fd;
}


/**
 * @brief Sends the text to the client through the socket
 * @param SafeFD socket 
 * @param string_view header of the text
 * @param bool extended to show if the program is in extended mode
 * @param string_view body of the text
 * @return int errno if there was any error. 0 otherwise
 */
int send_response(const SafeFD& socket, std::string_view header, bool extended, std::string_view body) {
  ssize_t bytes_sent = send(socket.get(), header.data(), header.size(), 0);
  if (bytes_sent < 0) {      
    return errno;
  }
  if (extended) {
    std::cerr << "send(): Se envían los datos de la cabecera a través del socket" << std::endl;
  }
  
  if (!body.empty()) {
    bytes_sent = send(socket.get(), body.data(), body.size(), 0);
    if (bytes_sent < 0) {          
      return errno; 
    }
    if (extended) {
      std::cerr << "send(): Se envían los datos del cuerpo a través del socket" << std::endl;
    }
  }
  return EXIT_SUCCESS; 
}
