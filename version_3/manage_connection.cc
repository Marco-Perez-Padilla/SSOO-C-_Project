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

 * Archivo mannage_connection.cc: Contiene las implementaciones necesarias para manejar la comunicación mediante sockets

 * Historial de revisiones:
 *      26/11/2024 - Primera version (creacion) del codigo
 *      27/11/2024 - Adicion de funcion send_response() y eliminacion de getenv()
**/

#include <sys/socket.h> // socket
#include <iostream> 
#include <sstream> // isstringstream
#include <filesystem>
#include <sys/types.h>
#include <sys/wait.h>
#include <array>

#include "manage_connection.h"


/**
 * @brief Function to create a socket given a determined port
 * @param uint16_t number of the port
 * @param bool extended to show if the program is in extended mode
 * @return SafeFD file descriptor is no error has occurred. Code errno otherwise
 */
std::expected<SafeFD, int> make_socket(uint16_t port, bool extended) {
  SafeFD fd (socket(AF_INET, SOCK_STREAM, 0));
  if (fd.get() < 0) {
    return std::unexpected(errno);
  }
  if (extended) {
    std::cerr << "socket(): Se crea el socket" << std::endl;
  }

  sockaddr_in local_address{};                            
  local_address.sin_family = AF_INET;                    
  local_address.sin_addr.s_addr = htonl(INADDR_ANY);      
  if (extended) {
    std::cerr << "htonl(): Se convierte la dirección IP en formato de red, Big Endian" << std::endl;
  }
  local_address.sin_port = htons(port);     
  if (extended) {
    std::cerr << "htons(): Se convierte el número del puerto en formato de red, Big Endian" << std::endl;
  }                 

  int result_fd (bind(fd.get(), reinterpret_cast<const sockaddr*>(&local_address), sizeof(local_address)));
  if (result_fd < 0) {
    return std::unexpected(errno);
  }
  if (extended) {
    std::cerr << "bind(): Se asocia una dirección de red al socket" << std::endl;
  }

  if (extended) {
    std::cerr << "close(): se cierra el archivo con descriptor de archivo \"" << fd.get() << "\" " << std::endl;
  }

  return fd;
}


/**
 * @brief Function to put a determined socket on listen
 * @param SafeFD socket to put on listen
 * @param bool extended to show if the program is in extended mode
 * @return errno code if any error has occurred. EXIT_SUCCESS code otherwise.
 */
int listen_connection(const SafeFD& socket, bool extended) {
  int result = listen(socket.get(), 5);
  if (result < 0) {
    return errno;
  }
  if (extended) {
    std::cerr << "listen(): Se pone el socket en modo escucha" << std::endl;
  }
  return EXIT_SUCCESS; 
}


/**
 * @brief Function to accept a connection
 * @param safeFD server socket 
 * @param sockaddr_in client socket address
 * @param bool extended to show if the program is in extended mode
 * @return SafeFD file descriptor is no error has occurred. Code errno otherwise
 */
std::expected<SafeFD, int> accept_connection(const SafeFD& socket,sockaddr_in& client_addr, bool extended) {
  socklen_t source_address_length;
  SafeFD new_fd (accept(socket.get(), reinterpret_cast<sockaddr*>(&client_addr), &source_address_length));
  if (new_fd.get() < 0) {
    return std::unexpected(errno);
  }
  if (extended) {
    std::cerr << "accept(): Se acepta la conexión en el socket" << std::endl;
  }

  if (extended) {
    std::cerr << "close(): se cierra el archivo con descriptor de archivo \"" << new_fd.get() << "\" " << std::endl;
  }

  return new_fd;
}


/**
 * @brief Function to read the client petition
 * @param SafeFD receives in this socket, result of accept_connection()
 * @param size_t max_size bytes of the client petition
 * @param bool extended to show if the program is in extended mode
 * @return string with the client petition if no error has occurred. Code errno otherwise
 */
std::expected<std::string, int> receive_request(const SafeFD& socket,size_t max_size, bool extended) {
  std::string request {}; //resize con max_size
  request.resize(max_size);
  size_t bytes_read = recv(socket.get(), request.data(), request.size(), 0);
  if (extended) {
    std::cerr << "recv(): Leer datos entrantes en el socket" << std::endl;
  }
  if (bytes_read < 0) {
    return std::unexpected(errno);
  }
  request.resize(bytes_read);
  return request;
}


/**
 * @brief Function that processes the client petition
 * @param string request
 * @return String with the route to the file if no error has occurred. A code error otherwise
 */
std::expected<std::string, int> process_petition (const std::string& request) {
  std::istringstream iss (request);
  std::string command;    // GET
  std::string path;       // "/..."
  if (!(iss >> command >> path)) {
    return std::unexpected(400);  
  } 
  if (command != "GET") {
    return std::unexpected(400);
  } 
  if (path.empty() || path.at(0) != '/') {
    return std::unexpected(400);
  }

  return path;
}


/**
 * @brief Sends the text to the client through the socket
 * @param SafeFD socket 
 * @param string_view header of the text
 * @param bool extended to show if the program is in extended mode
 * @param string_view body of the text
 * @return int errno if there was any error. 0 otherwise
 */
int send_response(const SafeFD& socket, std::string_view header, bool extended, std::string_view body) {
  ssize_t bytes_sent = send(socket.get(), header.data(), header.size(), 0);
  if (bytes_sent < 0) {      
    return errno;
  }
  if (extended) {
    std::cerr << "send(): Se envían los datos de la cabecera a través del socket" << std::endl;
  }
  
  if (!body.empty()) {
    bytes_sent = send(socket.get(), body.data(), body.size(), 0);
    if (bytes_sent < 0) {          
      return errno; 
    }
    if (extended) {
      std::cerr << "send(): Se envían los datos del cuerpo a través del socket" << std::endl;
    }
  }
  
  return EXIT_SUCCESS; 
}


/**
 * @brief Auxiliar method to check if a file exists
 * @param string with the path to the file to be checked
 * @return bool-type. True if found, false otherwise
 */
bool file_exists(const std::string& path) {
  bool result = std::filesystem::exists(path) && std::filesystem::is_regular_file(path);
  return result;
}


/**
 * @brief Auxiliar method to check if a file has permissions to be executed
 * @param string with the path to the file to be checked
 * @return bool-type. True if it has the permissions, false otherwise
 */
bool is_executable(const std::string& path) {
  if (!file_exists(path)) {
    return false;
  }
  bool result = (std::filesystem::status(path).permissions() & std::filesystem::perms::owner_exec) != std::filesystem::perms::none;
  return result;
}


/**
 * @brief Given a path to a file, executes that file if it is possible 
 * @param string
 * @param exec_environment
 * @param
 * @return string with the result of the execution if everything goes well, execute program_error otherwise
 */
std::expected<std::string, execute_program_error> execute_program(const std::string& path, const exec_environment& env, bool extended) {

  if (!file_exists(path)) {
    return std::unexpected(execute_program_error{"Not found", 404});
  } else if (!is_executable(path)) {
    return std::unexpected(execute_program_error{"Forbidden", 403});
  }

  const size_t tam_buffer (256);

  int pipefd[2];   // en 0 tenemos el fd del extremo de lectura y en 1 escritura
  int pipe_result = pipe(pipefd);
  if (extended) {
    std::cerr << "pipe(): se crea una tubería" << std::endl;
  }
  if (pipe_result < 0) {
    return std::unexpected(execute_program_error{"Error creating the pipeline", errno});
  }

  pid_t pid = fork();
  if (extended) {
    std::cerr << "fork(): se crea un proceso hijo" << std::endl;
  }

  if (pid < 0) {
    close(pipefd[0]);
    if (extended) {
      std::cerr << "close(): se cierra el extremo de lectura de la tubería" << std::endl;
    }
    close(pipefd[1]);
    if (extended) {
      std::cerr << "close(): se cierra el extremo de escritura de la tubería" << std::endl;
    }
    return std::unexpected(execute_program_error{"Error creating the child process", errno});
  }

  if (pid == 0) {    //  Para el hijo es 0. Para el padre es mayor a 0 (es el pid del hijo)
    // Bloque proceso hijo

    close (pipefd[0]);   // Cerramos el extremo de lectura (del proceso hijo nada más)
    if (extended) {
      std::cerr << "close(): se cierra el extremo de lectura de la tubería" << std::endl;
    }

    int dup2_result = dup2(pipefd[1], 1);
    if (extended) {
      std::cerr << "dup2(): se duplica el descriptor de archivo" << std::endl;
    }

    if (!dup2_result) {
      int error = errno;
      close(pipefd[1]);
      if (extended) {
      std::cerr << "close(): se cierra el extremo de escritura de la tubería" << std::endl;
      }
      return std::unexpected(execute_program_error{"Error creating the dup", error});
    }

    setenv("REQUEST_PATH", env.request_path.c_str(), 1);
    if (extended) {
      std::cerr << "setenv(): se setea la variable de entorno REQUEST_PATH" << std::endl;
    }
    setenv("SERVER_BASEDIR", env.server_basedir.c_str(), 1);
    if (extended) {
      std::cerr << "setenv(): se setea la variable de entorno SERVER_BASEDIR" << std::endl;
    }
    setenv("REMOTE_PORT", std::to_string(env.remote_port).c_str(), 1);
    if (extended) {
      std::cerr << "setenv(): se setea la variable de entorno REMOTE_PORT" << std::endl;
    }
    setenv("REMOTE_IP", env.remote_ip.c_str(), 1);
    if (extended) {
      std::cerr << "setenv(): se setea la variable de entorno REMOTE_IP" << std::endl;
    }

    if (extended) {
      std::cerr << "execl(): se reemplaza el código por el del archivo señalado" << std::endl;
    }
    execl(path.c_str(), path.c_str(), nullptr);
    
    if (extended) {
      std::cerr << "exit(): se retorna con código de error en el proceso hijo" << std::endl;
    }
    exit (EXIT_FAILURE);

  } else if (pid > 0) { // Bloque proceso padre
    close(pipefd[1]);
    if (extended) {
      std::cerr << "close(): se cierra el extremo de escritura de la tubería" << std::endl;
    }

    // waitpid (pid, int* status, int option), option = 0 ===> waitpid se vuelve bloqueante
    int status {};
    int waitpid_result = waitpid (pid, &status, 0);
    if (extended) {
      std::cerr << "waitpid(): se espera a la finalización del proceso hijo" << std::endl;
    }

    if (waitpid_result == -1) {
      int error = errno;
      close(pipefd[0]);
      if (extended) {
        std::cerr << "close(): se cierra el extremo de lectura de la tubería" << std::endl;
      }
      return std::unexpected (execute_program_error{"Error waiting", error});
    } 

    // Si termina, sea con éxito o error, se ha terminado de forma normal. Si termina por algo externo como una señal (ej ctrl c), termina anormalmente.
    if (WIFEXITED(status)) {
      // El proceso terminó normalmente
      if (WEXITSTATUS(status) == EXIT_SUCCESS) {
        std::array<char, tam_buffer> buffer;
        bool flag = true;
        std::string listen {};
        while (flag) {
          const size_t nbytes = read(pipefd[0], buffer.data(), tam_buffer);
          if (extended) {
            std::cerr << "read(): se leen los datos del extremo de lectura de la tubería con el tamaño máximo de buffer" << std::endl;
          }
          if (nbytes < 0) {
            int error = errno;
            close(pipefd[0]);
            if (extended) {
              std::cerr << "close(): se cierra el extremo de lectura de la tubería" << std::endl;
            }
            return std::unexpected(execute_program_error{"Invalid size", error});
          } else {
            if (nbytes < tam_buffer) {
              flag = false;
            }
            listen.append(buffer.data(), 0, static_cast<size_t>(nbytes));
            if (extended) {
              std::cerr << "append(): se añaden los datos al array de lectura" << std::endl;
            }
          }
        }
        close(pipefd[0]);
        if (extended) {
          std::cerr << "close(): se cierra el extremo de lectura de la tubería" << std::endl;
        }
        
        return listen;
      } else {
        close(pipefd[0]);
        if (extended) {
          std::cerr << "close(): se cierra el extremo de lectura de la tubería" << std::endl;
        }

        return std::unexpected(execute_program_error{"Error ending the process", WEXITSTATUS(status)});
      }
    } else {
      // El proceso terminó anormalmente
      close(pipefd[0]);
      if (extended) {
        std::cerr << "execl(): se reemplaza el código por el del archivo señalado" << std::endl;
      }

      std::string cout = "Proceso hijo termino anormalmente\n" ;
      return cout; 
    }

  } else {
    // Bloque de error en fork()
    int error = errno;
    close(pipefd[0]);
    if (extended) {
      std::cerr << "close(): se cierra el extremo de lectura de la tubería" << std::endl;
    }
    close (pipefd[1]);
    if (extended) {
      std::cerr << "close(): se cierra el extremo de escritura de la tubería" << std::endl;
    }

    return std::unexpected(execute_program_error{"Error in fork", error});
  }
}
