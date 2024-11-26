#include <unistd.h>

#include "safeFD.h"

 

  
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

SafeFD::~SafeFD() noexcept {
  if (fd_ >= 0) {
    close(fd_);
  }
}  