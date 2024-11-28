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

 * Archivo mannage_file.cc: Contiene las implementaciones necesarias para manejar los archivos 

 * Historial de revisiones:
 *      23/11/2024 - Primera version (creacion) del codigo
**/

#include "manage_file.h"
#include "safeFD.h"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>


/**
 * @brief Opens the given file, reads its size and maps it in memory
 * @param string path name of the file
 * @param bool extended to know if the code is in extended mode or not
 * @return SafeMap if there was no error, an int error code if it has happened any error
 */
std::expected<SafeMap, int> read_all(const std::string& path, bool extended) {
  SafeFD fd (open(path.c_str(), O_RDONLY)); 
  if (fd.get() < 0) {
    return std::unexpected(404);
  }
  if (extended) {
    std::cerr << "open(): se abre el archivo \"" << path << "\" " << std::endl;
  }

  int length = lseek(fd.get(), 0, SEEK_END);
  if (extended) {
    std::cerr << "lseek(): se obtiene el tamaño en bytes del archivo \"" << path << "\": " << length << " bytes" << std::endl;
  }
  
  void* mem = mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd.get(), 0);
  if (extended) {
    std::cerr << "mmap(): se mapea el archivo \"" << path << "\" " << std::endl;
  }
  
  if (extended) {
    std::cerr << "close(): se cierra el archivo \"" << path << "\" " << std::endl;
  }
  return SafeMap(static_cast<char*>(mem), length);
}


/**
 * @brief prints the content of given header and body
 * @param string_view header
 * @param string_view body
 */
void send_response(std::string_view header, std::string_view body) {
  std::cout << header;  
  if (!body.empty()) {
    std::cout << "\n" << body << "\n";  
  }
}