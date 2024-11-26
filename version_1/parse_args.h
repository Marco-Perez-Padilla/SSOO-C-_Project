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

 * Archivo parse_args.cc: Contiene las declaraciones necesarias para el procesamiento de argumentos pasados por 
                          líneas de comandos, el enum parse_args_errors, y el struct program options

 * Historial de revisiones:
 *      23/11/2024 - Primera version (creacion) del codigo
 *      23/11/2024 - Adicion de int port
**/

#include <string>
#include <expected>

enum class parse_args_errors {
 missing_argument,
 unknown_option,
 // ...
};

struct program_options {
 bool show_help = false;
 bool extended_mode = false;
 std::string output_filename;
 // ...
 //std::vector<std::string> additional_args; 
};

std::expected<program_options, parse_args_errors> parse_args(int argc, char* argv[]);
void print_usage ();