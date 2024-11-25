#include <string_view>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

class SafeSV {
 private:
  std::string_view sv_;
 public:
  explicit SafeSV(char* mem, int length) noexcept : sv_{std::string_view(mem, length)} {}
  ~SafeSV() noexcept {
    if (sv_.size() > 0) {
      munmap(const_cast<void*>(static_cast<const void*>(sv_.data())), sv_.size());
    }
  }
  [[nodiscard]] std::string_view get() const noexcept {return sv_;}
};
