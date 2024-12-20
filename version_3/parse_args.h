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
 *      27/11/2024 - Adicion de funcion getenv()
 *      28/11/2024 - Adicion de opcion -b y funcion get_cwd
 *      28/11/2024 - Adicion de los booleanos b_port y port, y del string BASE_DIR
**/

#include <string>
#include <expected>

constexpr std::size_t max_path_size = 4096;

enum class parse_args_errors {
 missing_argument,
 unknown_option,
 cwd_error,
 port_error,
};

struct program_options {
 bool show_help = false;
 bool extended_mode = false;
 bool b_port = false;
 bool base = false;
 int port = 8080;             // Testear el código con -p 51400
 bool modification = false;
 std::string output_filename;
 std::string BASE_DIR;
};

std::expected<program_options, parse_args_errors> parse_args(int argc, char* argv[]);
void print_usage ();
std::string get_env(const std::string& name, bool extended);
std::expected<std::string, int> get_cwd(bool extended);