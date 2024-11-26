#include <string_view>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#include "safeMap.h"

// Destructor
SafeMap::~SafeMap() {
  if (!sv_.empty()) {
    munmap(const_cast<char*>(sv_.data()), size_);
  }
}

// Constructor de movimiento
SafeMap::SafeMap(SafeMap&& other) noexcept : sv_(other.sv_), size_(other.size_) {
  other.sv_ = {};  // Invalida el string_view en el objeto original
  other.size_ = 0; // Invalida el tamaño en el objeto original
}

// Operador de movimiento
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