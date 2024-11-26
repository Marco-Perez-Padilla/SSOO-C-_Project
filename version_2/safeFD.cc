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

 * Archivo safeFD.cc: Contiene las implementaciones de la clase SafeFD

 * Historial de revisiones:
 *      23/11/2024 - Primera version (creacion) del codigo
**/

#include <unistd.h>

#include "safeFD.h"

/**
 * @brief operator =, we assure that a SafeFD can be created from an older one as an identical copy and destruying the previous one
 * @param 
 * @return 
 */
SafeFD& SafeFD::operator=(SafeFD&& other) noexcept {
  if (this != &other && fd_ != other.fd_) {
  // Cerrar el descriptor de archivo actual
    close(fd_);
    // Mover el descriptor de archivo de 'other' a este objeto
    fd_ = other.fd_;
    other.fd_ = -1;
  }
  return *this;
}


/**
 * @brief Destructor of SafeFD to close the file automatically when out of scope
 */
SafeFD::~SafeFD() noexcept {
  if (fd_ >= 0) {
    close(fd_);
  }
}  