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

 * Archivo safeFD.h: Contiene las declaraciones de la clase SafeFD. Se asegura de cerrar automaticamente un archivo al salirse de ámbito

 * Historial de revisiones:
 *      23/11/2024 - Primera version (creacion) del codigo
**/

#ifndef SAFE_FD_H
#define SAFE_FD_H

#include <unistd.h>

class SafeFD {
 private:
  int fd_;
 public:
  // Constructors
  explicit SafeFD(int fd) noexcept : fd_{fd} {}
  explicit SafeFD() noexcept : fd_{-1} {}
  // Deleting copy and assignation constructors
  SafeFD(const SafeFD&) = delete; 
  SafeFD& operator=(const SafeFD&) = delete; 
  // Declaring and implementing the new constructor
  SafeFD(SafeFD&& other) noexcept : fd_{other.fd_} {other.fd_ = -1;}
  // Assignation operator = 
  SafeFD& operator=(SafeFD&& other) noexcept;
  // Destructor
  ~SafeFD() noexcept;
  // Methods
  [[nodiscard]] bool is_valid() const noexcept {return fd_ >= 0;}
  [[nodiscard]] int get() const noexcept {return fd_;}
};

#endif