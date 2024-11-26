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

  // Destructor
  ~SafeMap();

    // Constructor de movimiento
  SafeMap(SafeMap&& other)noexcept;

  // Operador de movimiento
  SafeMap& operator=(SafeMap&& other) noexcept;

  // Eliminar constructor de copia y operador de copia
  SafeMap(const SafeMap&) = delete;
  SafeMap& operator=(const SafeMap&) = delete;

  // Método para acceder al contenido mapeado
  std::string_view get() const { return sv_; }
};

#endif