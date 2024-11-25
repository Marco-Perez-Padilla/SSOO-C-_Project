#include <unistd.h>

class SafeFD {
 private:
  int fd_;
 public:
  explicit SafeFD(int fd) noexcept : fd_{fd} {}
  ~SafeFD() noexcept {
    if (fd_ >= 0) {
      close(fd_);
    }
  }
  [[nodiscard]] int get() const noexcept {return fd_;}
};