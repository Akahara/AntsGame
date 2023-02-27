#include "client.h"
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <optional>
#include <thread>
#include "JSON.h"

#if defined(WIN32) || defined(_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>

#define close closesocket
#else
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/fcntl.h>
#endif

bool VERBOSE = true;

using namespace Server;

Client::Client(std::string address, unsigned short port) {
  // Connect client to the server
  p_uuid = NULL_UUID;

  fd = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in addr = {0};
  memset(&addr, 0, sizeof(struct sockaddr_in));

  addr.sin_family = AF_INET;
  
  if (inet_pton(AF_INET, address.c_str(), &addr.sin_addr) == -1) {
    std::cerr << "Invalid ip address ?" << std::endl;
    return;
  }
  
  addr.sin_port = htons(port);

  if (connect(fd, (const struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) {
    std::cerr << "Can't connect to " << address << std::endl;
    return;
  }

  // Make socket non-blocking
  #if defined(WIN32) || defined(_WIN32)
  unsigned long mode = 1;
  ioctlsocket(fd, FIONBIO, &mode);
  #else
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags == -1) {
    std::cerr << "fcntl() retourned -1" << std::endl;
    return;
  }

  if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
    perror("fcntl");
  }
  #endif
}

Client::~Client() {
  close(fd);
}

void Client::join(int _difficulty) {
  // Create the JSON message to join a game
  std::vector<uint8_t> message_to_join = JSON::createJoin(std::nullopt, _difficulty);
  std::string str((char *)message_to_join.data(), message_to_join.size());

  if (VERBOSE) {
    std::cout << "on envoi ca :" << str << "\n";
  }

  int ret = send(fd, (const char *)message_to_join.data(), message_to_join.size(), 0);

  if (ret != -1) {
    std::cout << "JOIN SENT" << std::endl;
  } else {
    std::cout << "send failed" << std::endl;
  }
}

void Client::move(MOVE_LIST _move) {
  // Create the JSON message of the player's move
  std::vector<uint8_t> message_move = JSON::createMove(p_uuid, _move);
  std::string str((char *)message_move.data(), message_move.size());

  if (VERBOSE) {
    std::cout << "on envoi ca :" << str << "\n";
  }

  int ret = send(fd, (const char *)message_move.data(), message_move.size(), 0);

  if (ret == -1) {
    std::cout << "send failed" << std::endl;
  }
}

void Client::listenClient() {
  // Extract the data from buffer to a const buffer
  std::vector<uint8_t> buffer(8192);
  ssize_t count = recv(fd, (char *)buffer.data(), buffer.size(), 0);

  #if defined(WIN32) || defined(_WIN32)
  if (WSAGetLastError() == WSAEWOULDBLOCK)
  #else
  if ((count == -1 || count == 0) && errno == EWOULDBLOCK)
  #endif
    return;

  if (count == -1) {
    std::cout << "recv failed" << std::endl;
    return;
  }

  // truncate buffer to match appropriate size
  buffer.resize(count);

  for (auto response : handler.ProcessData(buffer)) {
    std::cout << "received " << std::string(response.begin(), response.end()) << std::endl;
    auto js = json::parse(response);

    // Extract the type of message
    std::string type = JSON::getType(js);

    if (type == "okMaze") {
      // Extract the UUID
      p_uuid = JSON::getUUID(js);

      // Create the maze of the game
      maze.emplace(JSON::getMaze(js));
    }
    if (type == "info") {

      // Update the pheromons vector with the new one
      p_pheromons = JSON::getPheromons(js);
    }
  }
}
