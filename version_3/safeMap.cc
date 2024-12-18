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

 * Archivo safeMap.cc: Contiene las implementaciones de la clase SafeFD

 * Historial de revisiones:
 *      26/11/2024 - Primera version (creacion) del codigo
**/

#include <string_view>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#include "safeMap.h"

/**
 * @brief operator =, we assure that a SafeFD can be created from an older one as an identical copy and destruying the previous one
 * @param SafeMap&&: Referencia a un rvalue de tipo SafeMap
 * @return *this: referencia al objeto actual 
 */
SafeMap& SafeMap::operator=(SafeMap&& other) noexcept {
  if (this != &other) {
    // Liberar la memoria existente si está asignada
    if (!sv_.empty()) {
      munmap(const_cast<char*>(sv_.data()), size_);
    }
    // Mover los datos
    sv_ = other.sv_;
    size_ = other.size_;
    other.sv_ = {};  // Invalida el stringview en el objeto original
    other.size_ = 0; // Invalida el tamaño en el objeto original
  }
  return *this;
}


/**
 * @brief Destructor. It assures to unmap the memory automatically when out of scope
 */
SafeMap::~SafeMap() {
  if (!sv_.empty() && sv_.data()!=nullptr) {
    munmap(const_cast<char*>(sv_.data()), size_);
  }
}


/**
 * @brief New constructor
 * @param SafeMap&&: Referencia a un rvalue de tipo SafeMap
 */
SafeMap::SafeMap(SafeMap&& other) noexcept : sv_(other.sv_), size_(other.size_) {
  other.sv_ = {};  // Invalida el string_view en el objeto original
  other.size_ = 0; // Invalida el tamaño en el objeto original
}
