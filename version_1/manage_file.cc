
#include "manage_file.h"
#include "safeFD.h"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

std::expected<SafeMap, int> read_all(const std::string& path, bool extended) {
  SafeFD fd (open(path.c_str(), O_RDONLY)); 
  if (fd.get() < 0) {
    return std::unexpected(404);
  }
  if (extended) {
    std::cerr << "open(): se abre el archivo \"" << path << "\" " << std::endl;
  }

  int length = lseek(fd.get(), 0, SEEK_END);
  if (extended) {
    std::cerr << "lseek(): se obtiene el tamaño en bytes del archivo \"" << path << "\": " << length << " bytes" << std::endl;
  }

  void* mem = mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd.get(), 0);
  if (extended) {
    std::cerr << "mmap(): se mapea el archivo \"" << path << "\" " << std::endl;
  }
  
  if (extended) {
    std::cerr << "close(): se cierra el archivo \"" << path << "\" " << std::endl;
  }
  return SafeMap(static_cast<char*>(mem), length);
}


void send_response(std::string_view header, std::string_view body) {
  std::cout << header;  
  if (!body.empty()) {
    std::cout << "\n" << body << "\n";  
  }
}