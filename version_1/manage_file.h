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

 * Archivo mannage_file.h: Contiene las declaraciones necesarias para manejar los archivos 

 * Historial de revisiones:
 *      23/11/2024 - Primera version (creacion) del codigo
**/

#ifndef MANAGE_FILE_H
#define MANAGE_FILE_H

#include <string>
#include <expected>

#include "safeMap.h"

std::expected<SafeMap, int> read_all(const std::string& path, bool extended);
void send_response(std::string_view header, std::string_view body = {});

#endif