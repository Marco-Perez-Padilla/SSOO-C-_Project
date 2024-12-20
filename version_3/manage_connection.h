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

 * Archivo mannage_connection.h: Contiene las declaraciones necesarias para manejar la comunicación mediante sockets

 * Historial de revisiones:
 *      26/11/2024 - Primera version (creacion) del codigo
 *      27/11/2024 - Adicion de funcion send_response() y eliminacion de getenv()
 *      28/11/2024 - Adicion de funciones receive_request() y process_petition()
 *      28/11/2024 - Adicion de enum para manejar errores en process_petition()
 *      29/11/2024 - Eliminacion del enum
**/

#ifndef MANAGE_CONNECTION_H
#define MANAGE_CONNECTION_H

#include <string>
#include <expected>
#include <cstdint> // uint16_t
#include <netinet/in.h> // sockaddr_in

#include "safeFD.h"

struct exec_environment {
    std::string request_path;    // REQUEST_PATH
    std::string server_basedir;  // SERVER_BASEDIR
    std::string remote_ip;       // REMOTE_IP
    int remote_port;             // REMOTE_PORT
};

struct execute_program_error {
 std::string exit_code;
 int error_code;
};

std::expected<SafeFD, int> make_socket(uint16_t port, bool extended);
int listen_connection(const SafeFD& socket, bool extended);
std::expected<SafeFD, int> accept_connection(const SafeFD& socket,sockaddr_in& client_addr, bool extended);
int send_response(const SafeFD& socket, std::string_view header, bool extended, std::string_view body = {});
std::expected<std::string, int> receive_request(const SafeFD& socket,size_t max_size, bool extended);
std::expected<std::string, int> process_petition (const std::string& request);
bool file_exists(const std::string& path);
bool is_executable(const std::string& path);
std::expected<std::string, execute_program_error> execute_program(const std::string& path, const exec_environment& env, bool extended);

#endif