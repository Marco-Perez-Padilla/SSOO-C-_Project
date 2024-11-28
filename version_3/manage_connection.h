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
**/

#ifndef MANAGE_CONNECTION_H
#define MANAGE_CONNECTION_H

#include <string>
#include <expected>
#include <cstdint> // uint16_t
#include <netinet/in.h> // sockaddr_in

#include "safeFD.h"

std::expected<SafeFD, int> make_socket(uint16_t port, bool extended);
int listen_connection(const SafeFD& socket, bool extended);
std::expected<SafeFD, int> accept_connection(const SafeFD& socket,sockaddr_in& client_addr, bool extended);
int send_response(const SafeFD& socket, std::string_view header, bool extended, std::string_view body = {});

#endif