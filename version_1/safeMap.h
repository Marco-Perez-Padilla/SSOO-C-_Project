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

 * Archivo safeMap.h: Contiene las declaraciones de la clase SafeMap. Se asegura de desmapear automaticamente la memoria al salirse de ámbito

 * Historial de revisiones:
 *      26/11/2024 - Primera version (creacion) del codigo
**/

#ifndef SAFE_MAP_H
#define SAFE_MAP_H

#include <string_view>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

class SafeMap {
 private:
  std::string_view sv_;
  size_t size_;
 public:
  // Constructor
  SafeMap(void* addr, size_t size) : sv_(static_cast<char*>(addr), size), size_(size) {}
  // Deleting copy and assignation constructors
  SafeMap(const SafeMap&) = delete;
  SafeMap& operator=(const SafeMap&) = delete;
  // New constructor
  SafeMap(SafeMap&& other)noexcept;
  // Assignation operator = 
  SafeMap& operator=(SafeMap&& other) noexcept;
  // Destructor
  ~SafeMap();
  // Methods
  std::string_view get() const { return sv_; }
};

#endif