
#include "manage_file.h"
#include "safeFD.h"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

std::expected<SafeMap, int> read_all(const std::string& path, bool extended) {
  SafeFD fd (open(path.c_str(), O_RDONLY)); 
  if (extended) {
    std::cerr << "open(): se abre el archivo \"" << path << "\" " << std::endl;
  }
  if (fd.get() < 0) {
    return std::unexpected(404);
  }

  // Para mapear una archivo completo es necesario conocer su tamaño.
  // Una forma es usar fstat() y otra es usar lseek().
  // La función lseek() sirve para mover el puntero de lectura/escritura de un archivo y retorna la posición
  // a la que se ha movido. Por tanto, si se mueve al final del archivo, se obtiene el tamaño de este.
  int length = lseek(fd.get(), 0, SEEK_END);
  if (extended) {
    std::cerr << "lseek(): se obtiene el tamaño en bytes del archivo \"" << path << "\": " << length << " bytes" << std::endl;
  }

  // Se mapea el archivo completo en memoria para solo lectura y de forma privada.
  void* mem = mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd.get(), 0);
  if (extended) {
    std::cerr << "mmap(): se mapea el archivo \"" << path << "\" " << std::endl;
  }
  if (mem == MAP_FAILED) {
    return std::unexpected(403);
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