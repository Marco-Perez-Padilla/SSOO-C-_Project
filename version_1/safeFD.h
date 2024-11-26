#ifndef SAFE_FD_H
#define SAFE_FD_F

#include <unistd.h>

class SafeFD {
 private:
  int fd_;
 public:
  explicit SafeFD(int fd) noexcept : fd_{fd} {}
  explicit SafeFD() noexcept : fd_{-1} {}
  SafeFD(const SafeFD&) = delete; 
  SafeFD& operator=(const SafeFD&) = delete; 
  SafeFD(SafeFD&& other) noexcept : fd_{other.fd_} {other.fd_ = -1;}
  SafeFD& operator=(SafeFD&& other) noexcept;
  ~SafeFD() noexcept;

  [[nodiscard]] bool is_valid() const noexcept {return fd_ >= 0;}
  [[nodiscard]] int get() const noexcept {return fd_;}
};

#endif