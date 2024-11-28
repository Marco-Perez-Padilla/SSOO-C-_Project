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

 * Archivo mannage_file.cc: Contiene las implementaciones necesarias para manejar los archivos (Archivo momentaneo para ayudar a debuggear)

 * Historial de revisiones:
 *      23/11/2024 - Primera version (creacion) del codigo
 *      27/11/2024 - Eliminacion de funcion send_response()
**/

#include "manage_file.h"
#include "safeFD.h"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>


/**
 * @brief Reads the file given through command lines
 * @param string name of the file
 * @param bool to show if the program is in extended mode
 * @return SafeMap is there was no error. An int with the error code otherwise
 */
std::expected<SafeMap, int> read_all(const std::string& path, bool extended) {
  SafeFD fd (open(path.c_str(), O_RDONLY)); 
  if (extended) {
    std::cerr << "open(): se abre el archivo \"" << path << "\" " << std::endl;
  }
  if (fd.get() < 0) {
    return std::unexpected(404);
  }

  int length = lseek(fd.get(), 0, SEEK_END);
  if (extended) {
    std::cerr << "lseek(): se obtiene el tamaño en bytes del archivo \"" << path << "\": " << length << " bytes" << std::endl;
  }

  // Se mapea el archivo completo en memoria para solo lectura y de forma privada.
  void* mem = mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd.get(), 0);
  if (extended) {
    std::cerr << "mmap(): se mapea el archivo \"" << path << "\" " << std::endl;
  }
  if (mem == MAP_FAILED) {
    return std::unexpected(403);
  }

  if (extended) {
    std::cerr << "close(): se cierra el archivo \"" << path << "\" " << std::endl;
  }

  return SafeMap(static_cast<char*>(mem), length);
}
