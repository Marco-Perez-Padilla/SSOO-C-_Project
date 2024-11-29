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

 * Archivo parse_args.cc: Contiene las implementaciones necesarias para el procesamiento de argumentos pasados por líneas de comandos

 * Historial de revisiones:
 *      23/11/2024 - Primera version (creacion) del codigo
 *      23/11/2024 - Adicion de opcion -p
 *      25/11/2024 - Arreglo de opcion -h y mejora de opcion -p
 *      27/11/2024 - Adicion de funcion getenv()
 *      28/11/2024 - Adicion de opcion -b y funcion get_cwd
**/

#include <vector>
#include <iostream>
#include <algorithm>
#include <array>
#include <unistd.h>

#include "parse_args.h"

/**
 * @brief Function that processes the arguments given through command line
 * @param argc
 * @param argv
 * @return Program options struct with all the options if no error has occured. Enum parse args error otherwise with the specified error
 */
std::expected<program_options, parse_args_errors> parse_args(int argc, char* argv[]) {
  bool port = false;
  bool base = false;

  std::vector<std::string_view> args(argv + 1, argv + argc);
  program_options options;

  for (auto it = args.begin(), end = args.end(); it != end; ++it) {
    if (*it == "-h" || *it == "--help") {
      options.show_help = true;
    } else if (*it == "-v" || *it == "--verbose") {
      options.extended_mode = true;
    } else if (*it == "-p" || *it == "--port") {
      options.port = true;
      port = true;
      continue;
    } else if (port == true) {
      port = false;
      if (std::all_of(it->begin(), it->end(), isdigit)) {
        options.b_port = std::stoi(std::string(*it));
      } else {
        return std::unexpected(parse_args_errors::port_error);
      }
    } else if (*it == "-b" || *it == "--base") {
      options.base = true;
      base = true;
      continue;
    } else if (base == true){
      base = false;
      options.BASE_DIR = std::string(*it);
    } else {
      return std::unexpected(parse_args_errors::unknown_option); 
    }
  }

  if (options.b_port == false) {   // If -p hasn't been selected
    std::string env_port = get_env("DOCSERVER_PORT", options.extended_mode); // Gets $DOCSERVER_PORT
    if (env_port.empty()) {
      options.port = 8080;     // If empty then the default port is 8080
    } else {
      options.port = std::stoi(env_port); // If not empty then the port is $DOCSERVER_PORT
    }
  }

  if (options.base == false) {   // If -b hasn't been used
    std::string env_basedir = get_env("DOCSERVER_BASEDIR", options.extended_mode); // Gets $DOCSERVER_BASEDIR
    if (env_basedir.empty()) {
      auto cwd = get_cwd(options.extended_mode);    // If empty then get the cwd
      if (!cwd) {
        return std::unexpected(parse_args_errors::cwd_error); 
      } else {
        options.BASE_DIR = cwd.value();
      }
    } else { 
      options.BASE_DIR = env_basedir;    // If not empty then options.BASE_DIR is $DOCSERVER_BASEDIR
    }
  } 

  return options; 
}


/**
 * @brief Help function. Prints the help
 */
void print_usage () {
  std::cout << "docserver [-v | --verbose] [-h | --help] [-p <puerto> | --port <puerto>] FILE\n"
            << "    [-v | --verbose]: Optional argument. If activated, every call to system library functions will be printed in the error stream\n"
            << "    [-h | --help]: Optional argument. Prints this help\n"
            << "    [-p <puerto> | --port <puerto>]: Number of port to put on listen\n"
            << "      If -p isn't selected, the default port will be DOCSERVER_PORT. if it does not have a value, the default port will be 8080\n"
            << "    FILE: File to be used with this program\n"
            << "      If more than one file is specified, it will be interpreted as a wrong argument\n";
}


/**
 * @brief Function to get any environment variable
 * @param string name of the variable. For instance: DOCSERVER_PORT.
 * @param bool extended to show if the program is in extended mode
 * @return String with the result. Empty if the variable hasn't been found.
 */
std::string get_env(const std::string& name, bool extended) {
  char* value = getenv(name.c_str());
  if (extended) {
    std::cerr << "getenv(): Se obtiene la variable de entorno DOCSERVER_PORT" << std::endl;
  }
  if (value) {
    return std::string(value);
  } else {
  return std::string();
  }
}


/**
 * @brief Function to get the working directory
 * @param bool extended to show if the program is in extended mode
 * @return String with the result. A code error if any error occurred
 */
std::expected<std::string, int> get_cwd(bool extended) {
  std::array <char, max_path_size> buff;
  char* result = getcwd(buff.data(), max_path_size);   
  if (extended) {
    std::cerr << "getcwd(): Se obtiene el directorio de trabajo" << std::endl;
  } 
  if (result == NULL) {
    return std::unexpected(errno);
  }
  else {
    return buff.data();
  }
}